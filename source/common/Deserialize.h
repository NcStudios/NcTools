#pragma once

#include "NcaHeader.h"
#include "ncasset/AssetTypes.h"

#include <iosfwd>

namespace nc::asset
{
/** @brief A header and asset pair returned from a deserialize operation. */
template<class AssetType>
struct DeserializedResult
{
    NcaHeader header;
    AssetType asset;
};

/** @brief Construct a ConcaveCollider from data in a binary stream. */
auto DeserializeConcaveCollider(std::istream& stream) -> DeserializedResult<ConcaveCollider>;

/** @brief Construct a HullCollider from data in a binary stream. */
auto DeserializeHullCollider(std::istream& stream) -> DeserializedResult<HullCollider>;

/** @brief Construct a Mesh from data in a binary stream. */
auto DeserializeMesh(std::istream& stream) -> DeserializedResult<Mesh>;
} // nc::asset
