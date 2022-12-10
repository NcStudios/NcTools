#pragma once

#include "ncasset/AssetTypes.h"

#include <iosfwd>

namespace nc::asset
{
/** @brief Write a ConcaveCollider to a binary stream. */
void Serialize(std::ostream& stream, const ConcaveCollider& data, size_t assetId);

/** @brief Write a HullCollider to a binary stream. */
void Serialize(std::ostream& stream, const HullCollider& data, size_t assetId);

/** @brief Write a Mesh to a binary stream. */
void Serialize(std::ostream& stream, const Mesh& data, size_t assetId);
} // nc::asset
