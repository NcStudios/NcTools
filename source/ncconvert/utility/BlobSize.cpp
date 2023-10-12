#include "BlobSize.h"

#include "ncasset/Assets.h"

namespace
{
constexpr size_t matrixSize = (sizeof(float) * 16);

auto GetBonesSize(const std::optional<nc::asset::BonesData>& bonesData) -> size_t
{
    auto out = size_t{0};
    if (bonesData.has_value())
    {
        out += sizeof(size_t);
        out += sizeof(size_t);

        for (const auto& vertexSpaceToBoneSpace : bonesData.value().vertexSpaceToBoneSpace)
        {
            out += sizeof(size_t) + vertexSpaceToBoneSpace.boneName.size() + matrixSize;
        }

        for (const auto& boneSpaceToParentSpace : bonesData.value().boneSpaceToParentSpace)
        {
            out += sizeof(size_t) + boneSpaceToParentSpace.boneName.size() + matrixSize + sizeof(uint32_t) + sizeof(uint32_t);
        }
    }
    return out;
}

auto GetSkeletalAnimationClipSize(const nc::asset::SkeletalAnimationClip& asset) -> size_t
{
    auto baseSize = sizeof(size_t) + asset.name.size() + sizeof(uint32_t) + sizeof(double) + sizeof(size_t); // name size, name, durationInTicks, ticksPerSecond, framesPerBone count
    for (auto framesPerBonesIt=asset.framesPerBone.begin(); framesPerBonesIt!=asset.framesPerBone.end(); framesPerBonesIt++)
    {
        baseSize+=sizeof(size_t); // name size (unordered_map key)
        baseSize+=framesPerBonesIt->first.size(); // name (unordered_map key)
        const auto& frames = framesPerBonesIt->second;
        baseSize+=sizeof(size_t); // std::vector<nc::asset::PositionFrame> count
        baseSize+=frames.positionFrames.size() * sizeof(nc::asset::PositionFrame); // std::vector<nc::asset::PositionFrame>
        baseSize+=sizeof(size_t); // std::vector<nc::asset::RotationFrame> count
        baseSize+=frames.rotationFrames.size() * sizeof(nc::asset::RotationFrame); // std::vector<nc::asset::RotationFrame>
        baseSize+=sizeof(size_t); // std::vector<nc::asset::ScaleFrame> count
        baseSize+=frames.scaleFrames.size() * sizeof(nc::asset::ScaleFrame); // std::vector<nc::asset::ScaleFrame>
    }
    return baseSize;
}
}  // anonymous namespace

namespace nc::convert
{
auto GetBlobSize(const asset::AudioClip& asset) -> size_t
{
    constexpr auto baseSize = sizeof(asset::AudioClip::samplesPerChannel);
    return baseSize + asset.samplesPerChannel * sizeof(double) * 2ull;
}

auto GetBlobSize(const asset::ConcaveCollider& asset) -> size_t
{
    constexpr auto baseSize = sizeof(asset::ConcaveCollider::extents) + sizeof(asset::ConcaveCollider::maxExtent) + sizeof(size_t);
    return baseSize + asset.triangles.size() * sizeof(Triangle);
}

auto GetBlobSize(const asset::CubeMap& asset) -> size_t
{
    constexpr auto baseSize = sizeof(asset::CubeMap::faceSideLength);
    return baseSize + asset.pixelData.size();
}

auto GetBlobSize(const asset::HullCollider& asset) -> size_t
{
    constexpr auto baseSize = sizeof(asset::HullCollider::extents) + sizeof(asset::HullCollider::maxExtent) + sizeof(size_t);
    return  baseSize + asset.vertices.size() * sizeof(Vector3);
}

auto GetBlobSize(const asset::Mesh& asset) -> size_t
{
    constexpr auto baseSize = sizeof(asset::Mesh::extents) + sizeof(asset::Mesh::maxExtent) + sizeof(size_t) + sizeof(size_t);
    return baseSize + asset.vertices.size() * sizeof(asset::MeshVertex) + asset.indices.size() * sizeof(uint32_t) + sizeof(bool) + GetBonesSize(asset.bonesData);
}

auto GetBlobSize(const asset::SkeletalAnimationClip& asset) -> size_t
{
    return GetSkeletalAnimationClipSize(asset);
}

auto GetBlobSize(const asset::Texture& asset) -> size_t
{
    constexpr auto baseSize = sizeof(asset::Texture::width) + sizeof(asset::Texture::height);
    return baseSize + asset.pixelData.size();
}
} // namsepace nc::asset
