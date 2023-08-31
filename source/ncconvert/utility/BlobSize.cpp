#include "BlobSize.h"

#include "ncasset/Assets.h"

namespace
{
    auto GetBodySpaceTreeSize(const nc::asset::BoneParentOffset* parentNode, size_t* totalSize)
    {
        if (!parentNode)
        {
            return;
        }

        *totalSize += sizeof(std::string); // boneName
        *totalSize += (sizeof(float) * 16); // localSpace
        *totalSize += sizeof(size_t); // children count

        for (auto& child : parentNode->children)
        {
            GetBodySpaceTreeSize(&child, totalSize);
        }
    }

    auto GetBoneNamesToIdsSize(const std::unordered_map<std::string, uint32_t>& boneNamesToIds) -> size_t
    {
        auto size = size_t{};
        for (const auto& pair: boneNamesToIds)
        {
            size += pair.first.size() * sizeof(char);
            size += sizeof(size_t);
            size += sizeof(uint32_t);
        }
        return size;
    }

    auto GetBonesSize(const std::optional<nc::asset::BonesData>& bonesData) -> size_t
    {
        auto out = size_t{};
        if (bonesData.has_value())
        {
            out += sizeof(size_t);
            out += GetBoneNamesToIdsSize(bonesData.value().boneNamesToIds);
            out += bonesData.value().boneTransforms.size() * sizeof(float) * 16;
            GetBodySpaceTreeSize(&bonesData.value().bodySpaceOffsetTree, &out);
        }
        return out;
    }
}

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

auto GetBlobSize(const asset::Texture& asset) -> size_t
{
    constexpr auto baseSize = sizeof(asset::Texture::width) + sizeof(asset::Texture::height);
    return baseSize + asset.pixelData.size();
}
} // namsepace nc::asset
