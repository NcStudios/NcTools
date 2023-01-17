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
    /** @brief The directory to output .nca files to. */
    std::filesystem::path outputDirectory = "./";

    /**
     * @brief A path to a raw asset file to be converted to .nca format.
     * @note Specific to Mode::Target
     */
    std::optional<std::filesystem::path> targetPath;

    /**
     * @brief The name of the output .nca file.
     * @note Specific to Mode::Target
     */
    std::optional<std::string> assetName;

    /**
     * @brief The type of the asset to convert.
     * @note Specific to Mode::Target.
     */
    std::optional<asset::AssetType> targetType;

    /**
     * @brief A path to a manifest containing .nca conversion instructions.
     * @note Specific to Mode::Manifest.
     */
    std::optional<std::filesystem::path> manifestPath;
};
} // namespace nc::convert
