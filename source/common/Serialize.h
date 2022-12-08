#pragma once

#include "ncasset/AssetTypes.h"

#include <iosfwd>

namespace nc::asset
{
void Serialize(std::ostream& stream, const ConcaveCollider& data, size_t assetId);
void Serialize(std::ostream& stream, const HullCollider& data, size_t assetId);
void Serialize(std::ostream& stream, const Mesh& data, size_t assetId);
} // nc::asset
