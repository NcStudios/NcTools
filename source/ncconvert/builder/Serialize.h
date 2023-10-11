#pragma once

#include "ncasset/AssetsFwd.h"

#include <iosfwd>

namespace nc::convert
{
/** @brief Write an AudioClip to a binary stream. */
void Serialize(std::ostream& stream, const asset::AudioClip& data, size_t assetId);

/** @brief Write a ConcaveCollider to a binary stream. */
void Serialize(std::ostream& stream, const asset::ConcaveCollider& data, size_t assetId);

/** @brief Write a CubeMap to a binary stream. */
void Serialize(std::ostream& stream, const asset::CubeMap& data, size_t assetId);

/** @brief Write a HullCollider to a binary stream. */
void Serialize(std::ostream& stream, const asset::HullCollider& data, size_t assetId);

/** @brief Write a Mesh to a binary stream. */
void Serialize(std::ostream& stream, const asset::Mesh& data, size_t assetId);

/** @brief Write a SkeletalAnimationClip to a binary stream. */
void Serialize(std::ostream& stream, const asset::SkeletalAnimationClip& data, size_t assetId);

/** @brief Write a Texture to a binary stream. */
void Serialize(std::ostream& stream, const asset::Texture& data, size_t assetId);
} // nc::convert
