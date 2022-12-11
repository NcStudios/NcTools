#include "GeometryConverter.h"
#include "analysis/GeometryAnalysis.h"
#include "analysis/Sanitize.h"
#include "ncasset/AssetTypes.h"
#include "utility/Path.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "fmt/format.h"
#include "ncutility/NcError.h"

#include <algorithm>
#include <array>
#include <iostream>
#include <span>

namespace
{
constexpr auto concaveColliderFlags = aiProcess_Triangulate | aiProcess_ConvertToLeftHanded;
constexpr auto hullColliderFlags = concaveColliderFlags | aiProcess_JoinIdenticalVertices;
constexpr auto meshFlags = hullColliderFlags | aiProcess_GenNormals | aiProcess_CalcTangentSpace;
const auto supportedFileExtensions = std::array<std::string, 2> {".fbx", ".obj"};

auto ReadFbx(const std::filesystem::path& path, Assimp::Importer* importer, unsigned flags) -> const aiMesh*
{
    if (!nc::convert::ValidateInputFile(path, supportedFileExtensions))
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
        std::cerr << "Fbx has " << scene->mNumMeshes << " meshes. Only the first will be parsed.\n"
                  << "    file: " << path.string();
    }

    if (scene->mMeshes[0]->mNumVertices == 0)
    {
        throw nc::NcError("No vertices found for: ", path.string());
    }

    return scene->mMeshes[0];
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

auto ConvertToMeshVertices(const aiMesh* mesh) -> std::vector<nc::asset::MeshVertex>
{
    NC_ASSERT(mesh->mNormals && mesh->mTextureCoords && mesh->mTangents && mesh->mBitangents,
              "Not all required data was generated for mesh conversion");
    auto out = std::vector<nc::asset::MeshVertex>{};
    const auto nVertices = mesh->mNumVertices;
    out.reserve(nVertices);
    for (auto i = 0u; i < nVertices; ++i)
    {
        const auto uv = mesh->mTextureCoords[0][i];
        out.emplace_back(
            ToVector3(mesh->mVertices[i]), ToVector3(mesh->mNormals[i]), nc::Vector2{uv.x, uv.y},
            ToVector3(mesh->mTangents[i]), ToVector3(mesh->mBitangents[i])
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
            const auto mesh = ::ReadFbx(path, &m_importer, concaveColliderFlags);
            auto triangles = ::ConvertToTriangles(::ViewFaces(mesh), ::ViewVertices(mesh));
            if(auto count = Sanitize(triangles))
            {
                std::cerr << "    Warning: Bad values detected in mesh data. "
                          <<  count << " values have been set to 0.\n";
            }

            return asset::ConcaveCollider{
                GetMeshVertexExtents(triangles),
                FindFurthestDistanceFromOrigin(triangles),
                std::move(triangles)
            };
        }

        auto ImportHullCollider(const std::filesystem::path& path) -> asset::HullCollider
        {
            const auto mesh = ::ReadFbx(path, &m_importer, hullColliderFlags);
            auto convertedVertices = ::ConvertToVertices(::ViewVertices(mesh));
            if(auto count = Sanitize(convertedVertices))
            {
                std::cerr << "    Warning: Bad values detected in mesh data. "
                          << count << " values have been set to 0.\n";
            }

            return asset::HullCollider{
                GetMeshVertexExtents(convertedVertices),
                FindFurthestDistanceFromOrigin(convertedVertices),
                std::move(convertedVertices)
            };
        }

        auto ImportMesh(const std::filesystem::path& path) -> asset::Mesh
        {
            const auto mesh = ::ReadFbx(path, &m_importer, meshFlags);
            auto convertedVertices = ::ConvertToMeshVertices(mesh);
            if(auto count = Sanitize(convertedVertices))
            {
                std::cerr << "    Warning: Bad values detected in mesh data. "
                          << count << " values have been set to 0.\n";
            }

            return asset::Mesh{
                GetMeshVertexExtents(convertedVertices),
                FindFurthestDistanceFromOrigin(convertedVertices),
                std::move(convertedVertices),
                ::ConvertToIndices(::ViewFaces(mesh))
            };
        }

    private:
        Assimp::Importer m_importer;
};

GeometryConverter::GeometryConverter()
    : m_impl{std::make_unique<GeometryConverter::impl>()}
{
}

GeometryConverter::~GeometryConverter() = default;

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
