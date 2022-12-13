#pragma once

#include "AssetTypes.h"

#include <filesystem>

namespace nc::asset
{
/** @brief Read a HullCollider asset from an .nca file. */
auto ImportHullCollider(const std::filesystem::path& ncaPath) -> HullCollider;

/** @brief Read a ConcaveCollider asset from an .nca file. */
auto ImportConcaveCollider(const std::filesystem::path& ncaPath) -> ConcaveCollider;

/** @brief Read a Mesh asset from an .nca file. */
auto ImportMesh(const std::filesystem::path& ncaPath) -> Mesh;

/** @brief Read a Texture asset from an .nca file. */
auto ImportTexture(const std::filesystem::path& ncaPath) -> Texture;
} // namespace nc::asset
