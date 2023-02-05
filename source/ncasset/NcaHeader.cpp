#include "NcaHeader.h"

#include "ncutility/NcError.h"

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
} // namespace nc::asset
