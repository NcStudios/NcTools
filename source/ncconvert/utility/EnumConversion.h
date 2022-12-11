#pragma once

#include "ncasset/AssetTypes.h"

#include <string>

namespace nc::convert
{
auto ToAssetType(std::string type) -> asset::AssetType;
auto ToString(asset::AssetType) -> std::string;
}
