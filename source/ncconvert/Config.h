#pragma once

#include "ncasset/AssetTypes.h"

#include <filesystem>
#include <optional>
#include <string>
#include <vector>

namespace nc::convert
{
/** @brief Build controls generated from command line options. */
struct Config
{
    std::optional<std::filesystem::path> singleTargetPath;
    std::optional<asset::AssetType> singleTargetType;
    std::optional<std::filesystem::path> targetsFilePath;
    std::filesystem::path outputDirectory;
};
} // namespace nc::convert
