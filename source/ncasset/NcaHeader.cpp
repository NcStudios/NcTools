#include "NcaHeader.h"
#include "ImportedTypes.h"

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
    return baseSize + asset.vertices.size() * sizeof(Vector3) + asset.indices.size() * sizeof(uint32_t);
}
} // namsepace nc::asset
