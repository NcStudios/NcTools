#include "ncasset/NcaHeader.h"

#include "ncutility/NcError.h"
#include "ncutility/BinarySerialization.h"

namespace nc::asset
{
auto GetAssetType(const NcaHeader& header) -> AssetType
{
    auto magicNumber = std::string_view{header.magicNumber};
    if (magicNumber == MagicNumber::audioClip)
    {
        return AssetType::AudioClip;
    }
    else if (magicNumber == MagicNumber::concaveCollider)
    {
        return AssetType::ConcaveCollider;
    }
    else if (magicNumber == MagicNumber::cubeMap)
    {
        return AssetType::CubeMap;
    }
    else if (magicNumber == MagicNumber::hullCollider)
    {
        return AssetType::HullCollider;
    }
    else if (magicNumber == MagicNumber::mesh)
    {
        return AssetType::Mesh;
    }
    else if (magicNumber == MagicNumber::shader)
    {
        return AssetType::Shader;
    }
    else if (magicNumber == MagicNumber::texture)
    {
        return AssetType::Texture;
    }

    throw NcError("Unknown magic number in NcaHeader: ", header.magicNumber);
}

void Serialize(std::ostream& stream, const NcaHeader& header)
{
    constexpr char defaultAlgo[5] = "NONE"; // not yet supported
    stream.write(header.magicNumber, 4);
    stream.write(defaultAlgo, 4);
    nc::serialize::Serialize(stream, header.assetId);
    nc::serialize::Serialize(stream, header.size);
}

void Deserialize(std::istream& stream, NcaHeader& header)
{
    stream.read(reinterpret_cast<char*>(header.magicNumber), 4);
    stream.read(header.compressionAlgorithm, 4);
    header.magicNumber[4] = '\0';
    header.compressionAlgorithm[4] = '\0';
    nc::serialize::Deserialize(stream, header.assetId);
    nc::serialize::Deserialize(stream, header.size);
}
} // namespace nc::asset
