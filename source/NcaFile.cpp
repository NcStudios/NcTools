#include "NcaFile.h"
#include "MeshAnalysis.h"
#include "Serialize.h"
#include "Version.h"

NcaFile::NcaFile(const std::filesystem::path& outPath)
    : m_file{outPath},
      m_ncaPath{outPath}
{
    if (!m_file.is_open())
    {
        throw std::runtime_error("Failure opening: " + outPath.string());
    }

    m_file << "NcEngine Asset v" << NC_ASSET_TOOLS_VERSION << '\n';
}

void NcaFile::AddMesh(aiMesh* mesh)
{
    const auto extents = GetMeshVertexExtents(mesh->mVertices, mesh->mNumVertices);
    const auto vertexCount = mesh->mNumVertices;
    const auto faceCount = mesh->mNumFaces;
    m_file << extents << '\n'
           << "vertext_count: " << vertexCount << '\n'
           << "index_count: " << faceCount * 3 << '\n';

    auto vertex = VertexStream{mesh, 0u};
    for (size_t i = 0u; i < mesh->mNumVertices; ++i)
    {
        m_file << vertex << '\n';
        ++vertex.index;
    }

    for (size_t i = 0u; i < faceCount; ++i)
    {
        const auto& face = mesh->mFaces[i];
        if(face.mNumIndices != 3)
            throw std::runtime_error("Failure parsing indices");

        m_file << face.mIndices[0] << ' ' << face.mIndices[1] << ' ' << face.mIndices[2] << '\n';
    }
}

void NcaFile::AddConcaveCollider(aiMesh* mesh)
{
    const auto extents = GetMeshVertexExtents(mesh->mVertices, mesh->mNumVertices);

    m_file << mesh->mNumFaces << '\n'
           << extents.max << '\n';

    const auto* vertices = mesh->mVertices;

    for (size_t i = 0u; i < mesh->mNumFaces; ++i)
    {
        const auto& face = mesh->mFaces[i];
        if(face.mNumIndices != 3)
            throw std::runtime_error("Failure parsing indices");

        auto& a = vertices[face.mIndices[0]];
        auto& b = vertices[face.mIndices[1]];
        auto& c = vertices[face.mIndices[2]];

        m_file << a << ' ' << b << ' ' << c << '\n';
    }
}

void NcaFile::AddConvexHull(aiMesh* mesh)
{
    const auto extents = GetMeshVertexExtents(mesh->mVertices, mesh->mNumVertices);

    m_file << extents << '\n'
           << mesh->mNumVertices << '\n';

    for (size_t i = 0u; i < mesh->mNumVertices; ++i)
    {
        m_file << mesh->mVertices[i] << '\n';
    }
}