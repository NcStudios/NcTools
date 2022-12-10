#pragma once

#include "ncasset/AssetTypes.h"

#include <filesystem>
#include <optional>
#include <string>
#include <vector>

namespace nc::convert
{
struct Config
{
    std::optional<std::filesystem::path> singleTargetPath;
    std::optional<nc::asset::AssetType> singleTargetType;
    std::optional<std::filesystem::path> targetsFilePath;
    std::filesystem::path outputDirectory;
};

struct Target
{
    std::filesystem::path path;
    nc::asset::AssetType type;
};

auto GetAssetType(std::string type) -> nc::asset::AssetType;
auto ReadTargets(const Config& config) -> std::vector<Target>;
} // namespace nc::convert
