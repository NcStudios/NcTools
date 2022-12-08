#pragma once

#include "AssetTypes.h"

#include <filesystem>

namespace nc::asset
{
auto ImportHullCollider(const std::filesystem::path& ncaPath) -> HullCollider;
auto ImportConcaveCollider(const std::filesystem::path& ncaPath) -> ConcaveCollider;
auto ImportMesh(const std::filesystem::path& ncaPath) -> Mesh;
} // namespace nc::asset
