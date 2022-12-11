#include "EnumConversion.h"

#include "fmt/format.h"
#include "ncutility/NcError.h"

#include <algorithm>

namespace nc::convert
{
auto ToAssetType(std::string type) -> asset::AssetType
{
    std::ranges::transform(type, type.begin(), [](char c) { return std::tolower(c); });

    if(type == "mesh")
        return asset::AssetType::Mesh;
    else if(type == "hull-collider")
        return asset::AssetType::HullCollider;
    else if(type == "concave-collider")
        return asset::AssetType::ConcaveCollider;
    else if(type == "cube-map")
        return asset::AssetType::CubeMap;

    throw NcError("Failed to parse asset type from: " + type);
}

auto ToString(asset::AssetType type) -> std::string
{
    switch(type)
    {
        case asset::AssetType::CubeMap:
            return "cube-map";
        case asset::AssetType::ConcaveCollider:
            return "concave-collider";
        case asset::AssetType::HullCollider:
            return "hull-collider";
        case asset::AssetType::Mesh:
            return "mesh";
        default:
            break;
    }

    throw NcError(
        fmt::format("Unknown AssetType: {}", static_cast<int>(type))
    );
}
} // namespace nc::convert
