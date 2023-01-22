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

/** @brief Construct an AudioClip from data in a binary stream. */
auto DeserializeAudioClip(std::istream& stream) -> DeserializedResult<AudioClip>;

/** @brief Construct a ConcaveCollider from data in a binary stream. */
auto DeserializeConcaveCollider(std::istream& stream) -> DeserializedResult<ConcaveCollider>;

/** @brief Construct a CubeMap from data in a binary stream. */
auto DeserializeCubeMap(std::istream& stream) -> DeserializedResult<CubeMap>;

/** @brief Construct a HullCollider from data in a binary stream. */
auto DeserializeHullCollider(std::istream& stream) -> DeserializedResult<HullCollider>;

/** @brief Construct a Mesh from data in a binary stream. */
auto DeserializeMesh(std::istream& stream) -> DeserializedResult<Mesh>;

/** @brief Construct a Texture from data in a binary stream. */
auto DeserializeTexture(std::istream& stream) -> DeserializedResult<Texture>;
} // nc::asset
