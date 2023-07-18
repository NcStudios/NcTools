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
#include <span>

namespace
{
constexpr auto concaveColliderFlags = aiProcess_Triangulate | aiProcess_ConvertToLeftHanded;
constexpr auto hullColliderFlags = concaveColliderFlags | aiProcess_JoinIdenticalVertices;
constexpr auto meshFlags = hullColliderFlags | aiProcess_GenNormals | aiProcess_CalcTangentSpace;
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
    else if (scene->mNumMeshes > 1)
    {
        LOG("Fbx {} has {} meshes. Only the first will be parsed.", path.string(), scene->mNumMeshes);
    }

    if (scene->mMeshes[0]->mNumVertices == 0)
    {
        throw nc::NcError("No vertices found for: ", path.string());
    }

    return scene;
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

auto GetBoneWeights(const aiMesh* mesh) -> std::unordered_map<uint32_t, nc::asset::PerVertexBones>
{
    auto out = std::unordered_map<uint32_t, nc::asset::PerVertexBones>();

    // Iterate through all the bones in the mesh
    for (auto i = 0u; i < mesh->mNumBones; i++)
    {
        auto* currentBone = mesh->mBones[i];
        
        // Iterate through all the vertex weights each bone has
        for (auto j = 0u; j < currentBone->mNumWeights; j++)
        {
            auto vertexId = currentBone->mWeights[j].mVertexId;
            if (out[vertexId].boneWeights[3] != -1)
            {
                throw nc::NcError("Cannot import a mesh with more than four bones influencing any single vertex.");
            }

            // Create a mapping from vertex to collection of bone weights and IDs
            out[vertexId].Add(i, static_cast<float>(currentBone->mWeights[j].mWeight));

            if (out[vertexId].boneWeights[3] != -1)
            {
                if (out[vertexId].boneWeights[0] +
                    out[vertexId].boneWeights[1] + 
                    out[vertexId].boneWeights[2] + 
                    out[vertexId].boneWeights[3] != 1)
                {
                    throw nc::NcError("The sum of bone weights affecting each vertex must equal 1.");
                }
            }
        }
    }
    return out;
}

void PopulateBodySpaceTree(nc::asset::BodySpaceNode* bodySpaceNode, const aiNode* inputNode, nc::asset::BodySpaceNode* parent)
{
    if (!inputNode)
    {
        return;
    }

    bodySpaceNode->boneName = std::string(inputNode->mName.data);
    auto& inputMatrix = inputNode->mTransformation;
    bodySpaceNode->localSpace = DirectX::XMMATRIX
    {
        inputMatrix.a1, inputMatrix.a2, inputMatrix.a3, inputMatrix.a4,
        inputMatrix.b1, inputMatrix.b2, inputMatrix.b3, inputMatrix.b4,
        inputMatrix.c1, inputMatrix.c2, inputMatrix.c3, inputMatrix.c4,
        inputMatrix.d1, inputMatrix.d2, inputMatrix.d3, inputMatrix.d4
    };
    bodySpaceNode->parent = parent;

    for (auto i = 0u; i < inputNode->mNumChildren; i++)
    {
        bodySpaceNode->children.emplace_back();
        PopulateBodySpaceTree(&(bodySpaceNode->children.back()), inputNode->mChildren[i], bodySpaceNode);
    }
}

auto GetBonesData(const aiMesh* mesh, const aiNode* rootNode) -> nc::asset::BonesData
{
    auto out = nc::asset::BonesData{};
    out.boneTransforms.reserve(mesh->mNumBones);

    for (auto i = 0u; i < mesh->mNumBones; i++)
    {
        auto boneIndex = 0u;
        auto* currentBone = mesh->mBones[i];
        auto boneName = std::string(currentBone->mName.data);

        if (out.boneNamesToIds.find(boneName) == out.boneNamesToIds.end())
        {
            boneIndex = i;
        }
        else
        {
            boneIndex = out.boneNamesToIds[boneName];
        }

        out.boneNamesToIds[boneName] = boneIndex;
        auto& offsetMatrix = currentBone->mOffsetMatrix;
        out.boneTransforms[boneIndex] = DirectX::XMMATRIX
        {
            offsetMatrix.a1, offsetMatrix.a2, offsetMatrix.a3, offsetMatrix.a4,
            offsetMatrix.b1, offsetMatrix.b2, offsetMatrix.b3, offsetMatrix.b4,
            offsetMatrix.c1, offsetMatrix.c2, offsetMatrix.c3, offsetMatrix.c4,
            offsetMatrix.d1, offsetMatrix.d2, offsetMatrix.d3, offsetMatrix.d4
        };
    }

    PopulateBodySpaceTree(&out.bodySpaceOffsetTree, rootNode, nullptr);
    return out;
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
} // anonymous namespace

namespace nc::convert
{
class GeometryConverter::impl
{
    public:
        auto ImportConcaveCollider(const std::filesystem::path& path) -> asset::ConcaveCollider
        {
            const auto mesh = ::ReadFbx(path, &m_importer, concaveColliderFlags)->mMeshes[0];
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

        auto ImportMesh(const std::filesystem::path& path) -> asset::Mesh
        {
            const auto scene = ::ReadFbx(path, &m_importer, meshFlags);
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

auto GeometryConverter::ImportMesh(const std::filesystem::path& path) -> asset::Mesh
{
    return m_impl->ImportMesh(path);
}
} // namespace nc::convert
