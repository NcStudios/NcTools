#include "Serialize.h"

std::ostream& operator<<(std::ostream& os, const NcaHeader& header)
{
    os << "NcEngine Asset\n"
       << "Builer Version: " << header.version << '\n'
       << "Asset Type: " << static_cast<std::underlying_type_t<AssetType>>(header.assetType) << '\n';
    return os;
}

std::ostream& operator<<(std::ostream& os, const aiVector3D& vec)
{
    os << vec.x << ' ' << vec.y << ' ' << vec.z << ' ';
    return os;
}

std::ostream& operator<<(std::ostream& os, const MeshExtents& extents)
{
    os << "extents: " << extents.x << ' ' << extents.y << ' ' << extents.z << ' ' << extents.max;
    return os;
}

std::ostream& operator<<(std::ostream& os, const VertexStream& vertex)
{
    const auto i = vertex.index;
    auto& ver = vertex.mesh->mVertices[i];
    auto& nrm = vertex.mesh->mNormals[i];
    auto& tex = vertex.mesh->mTextureCoords[0][i];
    auto& tan = vertex.mesh->mTangents[i];
    auto& bit = vertex.mesh->mBitangents[i];
    os << ver << nrm << tex.x << ' ' << tex.y << ' ' << tan << bit;
    return os;
}