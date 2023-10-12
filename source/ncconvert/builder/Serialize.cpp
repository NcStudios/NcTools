#include "Serialize.h"
#include "builder/BonesWriter.h"
#include "builder/DataWriter.h"
#include "builder/SkeletalAnimationWriter.h"
#include "utility/BlobSize.h"
#include "ncasset/Assets.h"
#include "ncasset/NcaHeader.h"

namespace nc::convert
{
void Serialize(std::ostream& stream, const asset::AudioClip& data, size_t assetId)
{
    const auto assetSize = GetBlobSize(data);
    WriteHeader(stream, asset::MagicNumber::audioClip, assetId, assetSize);
    Write(stream, data.samplesPerChannel);
    Write(stream, data.leftChannel.data(), data.leftChannel.size() * sizeof(double));
    Write(stream, data.rightChannel.data(), data.rightChannel.size() * sizeof(double));
}

void Serialize(std::ostream& stream, const asset::ConcaveCollider& data, size_t assetId)
{
    const auto assetSize = GetBlobSize(data);
    WriteHeader(stream, asset::MagicNumber::concaveCollider, assetId, assetSize);
    Write(stream, data.extents);
    Write(stream, data.maxExtent);
    Write(stream, data.triangles.size());
    Write(stream, data.triangles.data(), data.triangles.size() * sizeof(Triangle));
}

void Serialize(std::ostream& stream, const asset::CubeMap& data, size_t assetId)
{
    const auto assetSize = GetBlobSize(data);
    WriteHeader(stream, asset::MagicNumber::cubeMap, assetId, assetSize);
    Write(stream, data.faceSideLength);
    Write(stream, data.pixelData.data(), data.pixelData.size());
}

void Serialize(std::ostream& stream, const asset::HullCollider& data, size_t assetId)
{
    const auto assetSize = GetBlobSize(data);
    WriteHeader(stream, asset::MagicNumber::hullCollider, assetId, assetSize);
    Write(stream, data.extents);
    Write(stream, data.maxExtent);
    Write(stream, data.vertices.size());
    Write(stream, data.vertices.data(), data.vertices.size() * sizeof(Vector3));
}

void Serialize(std::ostream& stream, const asset::Mesh& data, size_t assetId)
{
    const auto assetSize = GetBlobSize(data);
    WriteHeader(stream, asset::MagicNumber::mesh, assetId, assetSize);
    Write(stream, data.extents);
    Write(stream, data.maxExtent);
    Write(stream, data.vertices.size());
    Write(stream, data.indices.size());
    Write(stream, data.vertices.data(), data.vertices.size() * sizeof(asset::MeshVertex));
    Write(stream, data.indices.data(), data.indices.size() * sizeof(uint32_t));
    Write(stream, data.bonesData.has_value());
    if (data.bonesData.has_value())
    {
        const auto& bonesData = data.bonesData.value();
        Write(stream, bonesData.vertexSpaceToBoneSpace.size());
        Write(stream, bonesData.boneSpaceToParentSpace.size());
        Write(stream, bonesData.vertexSpaceToBoneSpace);
        Write(stream, bonesData.boneSpaceToParentSpace);
    }
}

void Serialize(std::ostream& stream, const asset::SkeletalAnimationClip& data, size_t assetId)
{
    const auto assetSize = GetBlobSize(data);
    WriteHeader(stream, asset::MagicNumber::skeletalAnimationClip, assetId, assetSize);
    Write(stream, data.name.size());
    Write(stream, data.name.data(), data.name.size());
    Write(stream, data.durationInTicks);
    Write(stream, data.ticksPerSecond);
    Write(stream, data.framesPerBone.size());
    Write(stream, data.framesPerBone);
}

void Serialize(std::ostream& stream, const asset::Texture& data, size_t assetId)
{
    const auto assetSize = GetBlobSize(data);
    WriteHeader(stream, asset::MagicNumber::texture, assetId, assetSize);
    Write(stream, data.width);
    Write(stream, data.height);
    Write(stream, data.pixelData.data(), data.pixelData.size());
}
} // namespace nc::convert
