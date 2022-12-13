#include "NcaHeader.h"
#include "ncasset/AssetTypes.h"

namespace nc::asset
{
auto GetBlobSize(const HullCollider& asset) -> size_t
{
    // extents + maxExtent + vertexCount
    constexpr auto baseSize = sizeof(Vector3) + sizeof(float) + sizeof(size_t);
    return  baseSize + asset.vertices.size() * sizeof(Vector3);
}

auto GetBlobSize(const ConcaveCollider& asset) -> size_t
{
    // extents + maxExtent + triCount
    constexpr auto baseSize = sizeof(Vector3) + sizeof(float) + sizeof(size_t);
    return baseSize + asset.triangles.size() * sizeof(Triangle);
}

auto GetBlobSize(const Mesh& asset) -> size_t
{
    // extents + maxExtent + vertexCount + indexCount
    constexpr auto baseSize = sizeof(Vector3) + sizeof(float) + sizeof(size_t) + sizeof(size_t);
    return baseSize + asset.vertices.size() * sizeof(MeshVertex) + asset.indices.size() * sizeof(uint32_t);
}

auto GetBlobSize(const Texture& asset) -> size_t
{
    // width + height
    constexpr auto baseSize = sizeof(uint32_t) + sizeof(uint32_t);
    return baseSize + asset.pixels.size();
}
} // namsepace nc::asset
