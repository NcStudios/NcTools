#include "Serialize.h"
#include "utility/BlobSize.h"
#include "ncasset/Assets.h"
#include "ncasset/NcaHeader.h"

#include "ncutility/BinarySerialization.h"

#include <cstring>
#include <iostream>

namespace
{
template<class T>
void SerializeImpl(std::ostream& stream, const T& data, std::string_view magicNumber, size_t assetId)
{
    auto header = nc::asset::NcaHeader{"", "NONE", assetId, nc::convert::GetBlobSize(data)};
    std::memcpy(header.magicNumber, magicNumber.data(), 5);
    nc::serialize::Serialize(stream, header);
    nc::serialize::Serialize(stream, data);
}
} // anonymous namespace

namespace nc::convert
{
void Serialize(std::ostream& stream, const asset::AudioClip& data, size_t assetId)
{
    SerializeImpl(stream, data, asset::MagicNumber::audioClip, assetId);
}

void Serialize(std::ostream& stream, const asset::ConcaveCollider& data, size_t assetId)
{
    SerializeImpl(stream, data, asset::MagicNumber::concaveCollider, assetId);
}

void Serialize(std::ostream& stream, const asset::CubeMap& data, size_t assetId)
{
    SerializeImpl(stream, data, asset::MagicNumber::cubeMap, assetId);
}

void Serialize(std::ostream& stream, const asset::HullCollider& data, size_t assetId)
{
    SerializeImpl(stream, data, asset::MagicNumber::hullCollider, assetId);
}

void Serialize(std::ostream& stream, const asset::Mesh& data, size_t assetId)
{
    SerializeImpl(stream, data, asset::MagicNumber::mesh, assetId);
}

void Serialize(std::ostream& stream, const asset::SkeletalAnimation& data, size_t assetId)
{
    SerializeImpl(stream, data, asset::MagicNumber::skeletalAnimation, assetId);
}

void Serialize(std::ostream& stream, const asset::Texture& data, size_t assetId)
{
    SerializeImpl(stream, data, asset::MagicNumber::texture, assetId);
}
} // namespace nc::convert
