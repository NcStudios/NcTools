#include "Serialize.h"
#include "builder/BonesWriter.h"
#include "utility/BlobSize.h"
#include "ncasset/Assets.h"
#include "ncasset/NcaHeader.h"

#include <ostream>

namespace
{
void WriteHeader(std::ostream& stream, std::string_view magicNumber, size_t assetId, size_t size)
{
    constexpr char defaultAlgo[5] = "NONE"; // not yet supported
    stream.write(magicNumber.data(), 4);
    stream.write(defaultAlgo, 4);
    stream.write(reinterpret_cast<const char*>(&assetId), sizeof(size_t));
    stream.write(reinterpret_cast<const char*>(&size), sizeof(size));
}

template<class T>
void Write(std::ostream& stream, const T& data)
{
    stream.write(reinterpret_cast<const char*>(&data), sizeof(T));
}

template<class T>
void Write(std::ostream& stream, const T* data, size_t size)
{
    stream.write(reinterpret_cast<const char*>(data), size);
}
} // anonymous namespace

namespace nc::convert
{
void Serialize(std::ostream& stream, const asset::AudioClip& data, size_t assetId)
{
    const auto assetSize = GetBlobSize(data);
    ::WriteHeader(stream, asset::MagicNumber::audioClip, assetId, assetSize);
    ::Write(stream, data.samplesPerChannel);
    ::Write(stream, data.leftChannel.data(), data.leftChannel.size() * sizeof(double));
    ::Write(stream, data.rightChannel.data(), data.rightChannel.size() * sizeof(double));
}

void Serialize(std::ostream& stream, const asset::ConcaveCollider& data, size_t assetId)
{
    const auto assetSize = GetBlobSize(data);
    ::WriteHeader(stream, asset::MagicNumber::concaveCollider, assetId, assetSize);
    ::Write(stream, data.extents);
    ::Write(stream, data.maxExtent);
    ::Write(stream, data.triangles.size());
    ::Write(stream, data.triangles.data(), data.triangles.size() * sizeof(Triangle));
}

void Serialize(std::ostream& stream, const asset::CubeMap& data, size_t assetId)
{
    const auto assetSize = GetBlobSize(data);
    ::WriteHeader(stream, asset::MagicNumber::cubeMap, assetId, assetSize);
    ::Write(stream, data.faceSideLength);
    ::Write(stream, data.pixelData.data(), data.pixelData.size());
}

void Serialize(std::ostream& stream, const asset::HullCollider& data, size_t assetId)
{
    const auto assetSize = GetBlobSize(data);
    ::WriteHeader(stream, asset::MagicNumber::hullCollider, assetId, assetSize);
    ::Write(stream, data.extents);
    ::Write(stream, data.maxExtent);
    ::Write(stream, data.vertices.size());
    ::Write(stream, data.vertices.data(), data.vertices.size() * sizeof(Vector3));
}

void Serialize(std::ostream& stream, const asset::Mesh& data, size_t assetId)
{
    const auto assetSize = GetBlobSize(data);
    ::WriteHeader(stream, asset::MagicNumber::mesh, assetId, assetSize);
    ::Write(stream, data.extents);
    ::Write(stream, data.maxExtent);
    ::Write(stream, data.vertices.size());
    ::Write(stream, data.indices.size());
    ::Write(stream, data.vertices.data(), data.vertices.size() * sizeof(asset::MeshVertex));
    ::Write(stream, data.indices.data(), data.indices.size() * sizeof(uint32_t));
    ::Write(stream, data.bonesData.has_value());
    if (data.bonesData.has_value())
    {
        const auto& bonesData = data.bonesData.value();
        ::Write(stream, bonesData.boneNamesToIds.size());
        ::Write(stream, bonesData.bodySpaceOffsetTreeSize);
        Write(stream, bonesData.boneNamesToIds);
        ::Write(stream, bonesData.boneTransforms.data(), bonesData.boneTransforms.size() * sizeof(DirectX::XMMATRIX));
        Write(stream, &bonesData.bodySpaceOffsetTree, 0u);
    }
}

void Serialize(std::ostream& stream, const asset::Texture& data, size_t assetId)
{
    const auto assetSize = GetBlobSize(data);
    ::WriteHeader(stream, asset::MagicNumber::texture, assetId, assetSize);
    ::Write(stream, data.width);
    ::Write(stream, data.height);
    ::Write(stream, data.pixelData.data(), data.pixelData.size());
}
} // namespace nc::convert
