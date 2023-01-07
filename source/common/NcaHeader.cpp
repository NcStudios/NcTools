#include "NcaHeader.h"
#include "ncasset/AssetTypes.h"

namespace nc::asset
{
auto GetBlobSize(const AudioClip& asset) -> size_t
{
    constexpr auto baseSize = sizeof(AudioClip::samplesPerChannel);
    return baseSize + asset.samplesPerChannel * sizeof(double) * 2ull;
}

auto GetBlobSize(const ConcaveCollider& asset) -> size_t
{
    constexpr auto baseSize = sizeof(ConcaveCollider::extents) + sizeof(ConcaveCollider::maxExtent) + sizeof(size_t);
    return baseSize + asset.triangles.size() * sizeof(Triangle);
}

auto GetBlobSize(const CubeMap& asset) -> size_t
{
    constexpr auto baseSize = sizeof(CubeMap::faceSideLength);
    return baseSize + asset.pixelData.size();
}

auto GetBlobSize(const HullCollider& asset) -> size_t
{
    constexpr auto baseSize = sizeof(HullCollider::extents) + sizeof(HullCollider::maxExtent) + sizeof(size_t);
    return  baseSize + asset.vertices.size() * sizeof(Vector3);
}

auto GetBlobSize(const Mesh& asset) -> size_t
{
    constexpr auto baseSize = sizeof(Mesh::extents) + sizeof(Mesh::maxExtent) + sizeof(size_t) + sizeof(size_t);
    return baseSize + asset.vertices.size() * sizeof(MeshVertex) + asset.indices.size() * sizeof(uint32_t);
}

auto GetBlobSize(const Texture& asset) -> size_t
{
    constexpr auto baseSize = sizeof(Texture::width) + sizeof(Texture::height);
    return baseSize + asset.pixelData.size();
}
} // namsepace nc::asset
