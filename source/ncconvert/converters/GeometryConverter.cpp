#include "GeometryConverter.h"
#include "analysis/GeometryAnalysis.h"
#include "analysis/Sanitize.h"
#include "utility/Path.h"
#include "utility/Log.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "fmt/format.h"
#include "ncasset/Assets.h"
#include "ncutility/NcError.h"

#include <algorithm>
#include <array>
#include <queue>
#include <span>
#include <unordered_map>
#include <ctime>

namespace
{
constexpr auto concaveColliderFlags = aiProcess_Triangulate | aiProcess_ConvertToLeftHanded;
constexpr auto hullColliderFlags = concaveColliderFlags | aiProcess_JoinIdenticalVertices;
constexpr auto meshFlags = hullColliderFlags | aiProcess_GenNormals | aiProcess_CalcTangentSpace;
constexpr auto skeletalAnimationFlags = meshFlags | aiProcess_LimitBoneWeights;
const auto supportedFileExtensions = std::array<std::string, 2> {".fbx", ".obj"};

auto ReadFbx(const std::filesystem::path& path, Assimp::Importer* importer, unsigned flags) -> const aiScene*
{
    if (!nc::convert::ValidateInputFileExtension(path, supportedFileExtensions))
    {
        throw nc::NcError("Invalid input file: ", path.string());
    }

    if (!importer->ValidateFlags(flags))
    {
        throw nc::NcError("Unsupported import flags");
    }

    auto scene = importer->ReadFile(path.string(), flags);
    if (!scene)
    {
        throw nc::NcError(fmt::format("Assimp failure: {} \n    file: {}",
            importer->GetErrorString(), path.string()
        ));
    }

    if (scene->mNumMeshes == 0)
    {
        throw nc::NcError("Fbx contains no mesh data\n    file: ", path.string());
    }

    return scene;
}

auto ParseSkeletalAnimationClipName(const aiString& name) -> std::string
{
    // Blender adds a prefix of '<armature name>|' to animation clips. We just want the clip/action name.
    auto nameAsString = std::string(name.C_Str());
    auto pipeDelimiterPos = nameAsString.find_last_of('|');

    if (pipeDelimiterPos == std::string::npos) // Allows support for non-blender imported files.
    {
        return nameAsString;
    }

    if (pipeDelimiterPos == nameAsString.size()) // Returns name in format like: clip_2023_10_13_18-33-59 if no name is given.
    {
        auto currentDateTime = time(0);
        auto utcDateTime = gmtime(&currentDateTime);
        std::string formattedTime = std::string("clip") + "_" +
                                    std::to_string(1900 + utcDateTime->tm_year) + "_" +
                                    std::to_string(1 + utcDateTime->tm_mon)  + "_" +
                                    std::to_string(utcDateTime->tm_mday) + "_" +
                                    std::to_string(utcDateTime->tm_hour) + "-" +
                                    std::to_string(utcDateTime->tm_min) + "-" +
                                    std::to_string(utcDateTime->tm_sec);
        return formattedTime;
    }

    return nameAsString.substr(pipeDelimiterPos+1, nameAsString.size()-pipeDelimiterPos);
}

auto ToVector3(const aiVector3D& in) -> nc::Vector3
{
    return nc::Vector3{in.x, in.y, in.z};
}

auto ViewVertices(const aiMesh* mesh) -> std::span<const aiVector3D>
{
    return {mesh->mVertices, mesh->mNumVertices};
}

auto ViewFaces(const aiMesh* mesh) -> std::span<const aiFace>
{
    return {mesh->mFaces, mesh->mNumFaces};
}

auto ConvertToVertices(std::span<const aiVector3D> vertices) -> std::vector<nc::Vector3>
{
    auto out = std::vector<nc::Vector3>{};
    out.reserve(vertices.size());
    std::ranges::transform(vertices, std::back_inserter(out), [](auto&& vertex)
    {
        return ::ToVector3(vertex);
    });

    return out;
}

auto ConvertToIndices(std::span<const aiFace> faces) -> std::vector<uint32_t>
{
    auto out = std::vector<uint32_t>{};
    out.reserve(faces.size() * 3);
    for (const auto& face : faces)
    {
        NC_ASSERT(face.mNumIndices == 3, "Found non-triangular face.");
        out.push_back(face.mIndices[0]);
        out.push_back(face.mIndices[1]);
        out.push_back(face.mIndices[2]);
    }

    return out;
}

auto ConvertToTriangles(std::span<const aiFace> faces, std::span<const aiVector3D> vertices) -> std::vector<nc::Triangle>
{
    auto out = std::vector<nc::Triangle>{};
    out.reserve(faces.size());
    std::ranges::transform(faces, std::back_inserter(out), [&vertices](auto&& face)
    {
        NC_ASSERT(face.mNumIndices == 3, "Found non-triangular face.");
        return nc::Triangle{
            ::ToVector3(vertices[face.mIndices[0]]),
            ::ToVector3(vertices[face.mIndices[1]]),
            ::ToVector3(vertices[face.mIndices[2]])
        };
    });

    return out;
}

auto ConvertToXMMATRIX(const aiMatrix4x4* inputMatrix) -> DirectX::XMMATRIX
{
    return DirectX::XMMATRIX
    {
        inputMatrix->a1, inputMatrix->a2, inputMatrix->a3, inputMatrix->a4,
        inputMatrix->b1, inputMatrix->b2, inputMatrix->b3, inputMatrix->b4,
        inputMatrix->c1, inputMatrix->c2, inputMatrix->c3, inputMatrix->c4,
        inputMatrix->d1, inputMatrix->d2, inputMatrix->d3, inputMatrix->d4
    };
}

auto GetBoneWeights(const aiMesh* mesh) -> std::unordered_map<uint32_t, nc::asset::PerVertexBones>
{
    auto perVertexBones = std::unordered_map<uint32_t, nc::asset::PerVertexBones>();

    // Iterate through all the bones in the mesh
    for (auto boneIndex = 0u; boneIndex < mesh->mNumBones; boneIndex++)
    {
        auto* currentBone = mesh->mBones[boneIndex];
        
        // Iterate through all the vertex weights each bone has
        for (auto boneWeightIndex = 0u; boneWeightIndex < currentBone->mNumWeights; boneWeightIndex++)
        {
            auto vertexId = currentBone->mWeights[boneWeightIndex].mVertexId;
            auto& currentPerVertexBones = perVertexBones[vertexId];
            if (currentPerVertexBones.boneWeights[3] != -1)
            {
                throw nc::NcError("Cannot import a mesh with more than four bones influencing any single vertex.");
            }

            // Create a mapping from vertex to collection of bone weights and IDs
            for (auto i = 0u; i < currentPerVertexBones.boneIds.size(); i++)
            {
                if (currentPerVertexBones.boneWeights[i] == -1)
                {
                    currentPerVertexBones.boneIds[i] = boneIndex;
                    currentPerVertexBones.boneWeights[i] = currentBone->mWeights[boneWeightIndex].mWeight;
                    break;
                }
            }

            if (currentPerVertexBones.boneWeights[3] != -1)
            {
                auto boneWeightTotal = 
                currentPerVertexBones.boneWeights[0] +
                currentPerVertexBones.boneWeights[1] + 
                currentPerVertexBones.boneWeights[2] + 
                currentPerVertexBones.boneWeights[3];

                if (!nc::FloatEqual(boneWeightTotal, 1.0f))
                {
                    throw nc::NcError("The sum of bone weights affecting each vertex must equal 1. Current weight: ", std::to_string(boneWeightTotal));
                }
            }
        }
    }
    return perVertexBones;
}

/**
 * @brief Converts the given aiNode* tree of bone spaces into a flattened vector where all siblings are contiguous.
 * Tree:
 * A
 *    A1
 *       A1A   A1B
 *    B1
 * 
 * Vector:
 * [A, A1, B1, A1A, A1B]
 */
auto GetBoneSpaceToParentSpaceMatrices(const aiNode* inputNode) -> std::vector<nc::asset::BoneSpaceToParentSpace>
{
    auto out = std::vector<nc::asset::BoneSpaceToParentSpace>();
    auto unprocessedNodes = std::queue<const aiNode*>{};

    if (!inputNode)
    {
        return out;
    }

    unprocessedNodes.push(inputNode);
    const aiNode* currentNode = nullptr;

    while (!unprocessedNodes.empty())
    {
        currentNode = unprocessedNodes.front();

        // Get the index of the next available slot after the current node and it's children have been placed. (Siblings are always contiguous.)
        auto nextAvailableSlot = static_cast<uint32_t>(unprocessedNodes.size() + out.size());
        auto boneSpaceToParentSpace = nc::asset::BoneSpaceToParentSpace
        {
            .boneName = std::string(currentNode->mName.data),
            .transformationMatrix = ConvertToXMMATRIX(&currentNode->mTransformation),
            .numChildren = currentNode->mNumChildren,
            .indexOfFirstChild = nextAvailableSlot
        };

        unprocessedNodes.pop();
        out.push_back(std::move(boneSpaceToParentSpace));

        for (auto childIndex = 0u; childIndex < currentNode->mNumChildren; childIndex++)
        {
            unprocessedNodes.push(currentNode->mChildren[childIndex]);
        }
    }
    return out;
}

auto GetVertexToBoneSpaceMatrices(const aiMesh* mesh) -> std::vector<nc::asset::VertexSpaceToBoneSpace>
{
    auto out = std::vector<nc::asset::VertexSpaceToBoneSpace>();
    out.reserve(mesh->mNumBones);

    for (auto boneIndex = 0u; boneIndex < mesh->mNumBones; boneIndex++)
    {
        auto* currentBone = mesh->mBones[boneIndex];
        auto boneName = std::string(currentBone->mName.data);
        out.emplace(out.begin() + boneIndex, std::string{currentBone->mName.data}, ConvertToXMMATRIX(&currentBone->mOffsetMatrix));
    }
    return out;
}

auto GetBonesData(const aiMesh* mesh, const aiNode* rootNode) -> nc::asset::BonesData
{
    return nc::asset::BonesData
    {
        GetVertexToBoneSpaceMatrices(mesh),
        GetBoneSpaceToParentSpaceMatrices(rootNode)
    };
}

auto ConvertToMeshVertices(const aiMesh* mesh) -> std::vector<nc::asset::MeshVertex>
{
    NC_ASSERT(static_cast<bool>(mesh->mNormals) &&
              static_cast<bool>(mesh->mTextureCoords) &&
              static_cast<bool>(mesh->mTangents) &&
              static_cast<bool>(mesh->mBitangents),
              "Not all required data was generated for mesh conversion");
    auto out = std::vector<nc::asset::MeshVertex>{};
    const auto nVertices = mesh->mNumVertices;
    out.reserve(nVertices);

    if (mesh->HasBones())
    {
        const auto perVertexBones = GetBoneWeights(mesh);
        for (auto i = 0u; i < nVertices; ++i)
        {
            const auto uv = mesh->mTextureCoords[0][i];
            auto boneWeights = nc::Vector4(perVertexBones.at(i).boneWeights[0],
                                        perVertexBones.at(i).boneWeights[1],
                                        perVertexBones.at(i).boneWeights[2],
                                        perVertexBones.at(i).boneWeights[3]);
            out.emplace_back(
                ToVector3(mesh->mVertices[i]), ToVector3(mesh->mNormals[i]), nc::Vector2{uv.x, uv.y},
                ToVector3(mesh->mTangents[i]), ToVector3(mesh->mBitangents[i]), boneWeights, perVertexBones.at(i).boneIds
            );
        }
        return out;
    }

    for (auto i = 0u; i < nVertices; ++i)
    {
        const auto uv = mesh->mTextureCoords[0][i];
        out.emplace_back(
            ToVector3(mesh->mVertices[i]), ToVector3(mesh->mNormals[i]), nc::Vector2{uv.x, uv.y},
            ToVector3(mesh->mTangents[i]), ToVector3(mesh->mBitangents[i]), nc::Vector4{-1, -1, -1, -1}, std::array<uint32_t, 4>{UINT32_MAX, UINT32_MAX, UINT32_MAX, UINT32_MAX}
        );
    }
    return out;
}

auto ConvertToSkeletalAnimationClips(const aiScene* scene) -> std::vector<nc::asset::SkeletalAnimationClip>
{
    auto skeletalAnimationClips = std::vector<nc::asset::SkeletalAnimationClip>{};
    
    if (scene->mNumAnimations == 0)
    {
        return skeletalAnimationClips;
    }

    skeletalAnimationClips.reserve(scene->mNumAnimations);

    // There can be multiple animation clips in a single scene.
    for (const auto* animationClip : std::span(scene->mAnimations, scene->mNumAnimations))
    {
        auto skeletalAnimationClip = nc::asset::SkeletalAnimationClip{};
        skeletalAnimationClip.name = ParseSkeletalAnimationClipName(animationClip->mName);
        skeletalAnimationClip.ticksPerSecond = animationClip->mTicksPerSecond == 0 ? 25.0f : animationClip->mTicksPerSecond; // Ticks per second is not required to be set in animation software.
        skeletalAnimationClip.durationInTicks = static_cast<uint32_t>(animationClip->mDuration * skeletalAnimationClip.ticksPerSecond);
        skeletalAnimationClip.framesPerBone = std::unordered_map<std::string, nc::asset::SkeletalAnimationFrames>{};
        skeletalAnimationClip.framesPerBone.reserve(animationClip->mNumChannels);

        // A single channel represents one bone and all of its transformations for the animation clip.
        for (const auto* channel : std::span(animationClip->mChannels, animationClip->mNumChannels))
        {
            auto frames = nc::asset::SkeletalAnimationFrames{};
            frames.positionFrames.reserve(channel->mNumPositionKeys);
            frames.rotationFrames.reserve(channel->mNumRotationKeys);
            frames.scaleFrames.reserve(channel->mNumScalingKeys);
            
            for (const auto& positionKey : std::span(channel->mPositionKeys, channel->mNumPositionKeys))
            {
                frames.positionFrames.emplace_back(positionKey.mTime, nc::Vector3(positionKey.mValue.x, positionKey.mValue.y, positionKey.mValue.z));
            }

            for (const auto& rotationKey : std::span(channel->mRotationKeys, channel->mNumRotationKeys))
            {
                frames.rotationFrames.emplace_back(rotationKey.mTime, nc::Quaternion(rotationKey.mValue.x, rotationKey.mValue.y, rotationKey.mValue.z, rotationKey.mValue.w));
            }

            for (const auto& scaleKey : std::span(channel->mScalingKeys, channel->mNumScalingKeys))
            {
                frames.scaleFrames.emplace_back(scaleKey.mTime, nc::Vector3(scaleKey.mValue.x, scaleKey.mValue.y, scaleKey.mValue.z));
            }
            skeletalAnimationClip.framesPerBone.emplace(std::string(channel->mNodeName.C_Str()), std::move(frames));
        }
        skeletalAnimationClips.push_back(std::move(skeletalAnimationClip));
    }
    return skeletalAnimationClips;
}

} // anonymous namespace

namespace nc::convert
{
class GeometryConverter::impl
{
    public:
        auto ImportConcaveCollider(const std::filesystem::path& path) -> asset::ConcaveCollider
        {
            const auto mesh = ::ReadFbx(path, &m_importer, concaveColliderFlags)->mMeshes[0];

            if (mesh->mNumVertices == 0)
            {
                throw nc::NcError("No vertices found for: ", path.string());
            }

            auto triangles = ::ConvertToTriangles(::ViewFaces(mesh), ::ViewVertices(mesh));
            if(auto count = Sanitize(triangles))
            {
                LOG("Warning: Bad values detected in mesh. {} values have been set to 0.", count);
            }

            return asset::ConcaveCollider{
                GetMeshVertexExtents(triangles),
                FindFurthestDistanceFromOrigin(triangles),
                std::move(triangles)
            };
        }

        auto ImportHullCollider(const std::filesystem::path& path) -> asset::HullCollider
        {
            const auto mesh = ::ReadFbx(path, &m_importer, hullColliderFlags)->mMeshes[0];

            if (mesh->mNumVertices == 0)
            {
                throw nc::NcError("No vertices found for: ", path.string());
            }

            auto convertedVertices = ::ConvertToVertices(::ViewVertices(mesh));
            if(auto count = Sanitize(convertedVertices))
            {
                LOG("Warning: Bad values detected in mesh. {} values have been set to 0.", count);
            }

            return asset::HullCollider{
                GetMeshVertexExtents(convertedVertices),
                FindFurthestDistanceFromOrigin(convertedVertices),
                std::move(convertedVertices)
            };
        }

        auto ImportMesh(const std::filesystem::path& path, const std::optional<std::string>& internalName) -> asset::Mesh
        {
            const auto scene = ::ReadFbx(path, &m_importer, meshFlags);

            aiMesh* mesh = nullptr;

            if (internalName.has_value())
            {
                for (auto* sceneMesh : std::span(scene->mMeshes, scene->mNumMeshes))
                {
                    if (std::string{sceneMesh->mName.C_Str()} == internalName)
                    {
                        mesh = sceneMesh;
                        break;
                    }
                }
                if (mesh == nullptr) throw nc::NcError("An internal mesh name was provided but no mesh was found by that name: {}. No asset will be created.", internalName.value());
            }
            else 
            {
                mesh = scene->mMeshes[0];
            }

            if (mesh->mNumVertices == 0)
            {
                throw nc::NcError("No vertices found for: ", path.string());
            }

            auto convertedVertices = ::ConvertToMeshVertices(scene->mMeshes[0]);
            if(auto count = Sanitize(convertedVertices))
            {
                LOG("Warning: Bad values detected in mesh. {} values have been set to 0.", count);
            }

            return asset::Mesh{
                GetMeshVertexExtents(convertedVertices),
                FindFurthestDistanceFromOrigin(convertedVertices),
                std::move(convertedVertices),
                ::ConvertToIndices(::ViewFaces(scene->mMeshes[0])),
                GetBonesData(scene->mMeshes[0], scene->mRootNode)
            };
        }

        auto ImportSkeletalAnimations(const std::filesystem::path& path) -> std::vector<asset::SkeletalAnimationClip>
        {
            const auto scene = ::ReadFbx(path, &m_importer, skeletalAnimationFlags);
            return ::ConvertToSkeletalAnimationClips(scene);
        }

    private:
        Assimp::Importer m_importer;
};

GeometryConverter::GeometryConverter()
    : m_impl{std::make_unique<GeometryConverter::impl>()}
{
}

GeometryConverter::~GeometryConverter() noexcept = default;

auto GeometryConverter::ImportConcaveCollider(const std::filesystem::path& path) -> asset::ConcaveCollider
{
    return m_impl->ImportConcaveCollider(path);
}

auto GeometryConverter::ImportHullCollider(const std::filesystem::path& path) -> asset::HullCollider
{
    return m_impl->ImportHullCollider(path);
}

auto GeometryConverter::ImportMesh(const std::filesystem::path& path, const std::optional<std::string>& internalName) -> asset::Mesh
{
    return m_impl->ImportMesh(path, internalName);
}

auto GeometryConverter::ImportSkeletalAnimations(const std::filesystem::path& path) -> std::vector<asset::SkeletalAnimationClip>
{
    return m_impl->ImportSkeletalAnimations(path);
}
} // namespace nc::convert
