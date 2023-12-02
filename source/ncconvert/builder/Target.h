#pragma once

#include <filesystem>
#include <optional>

namespace nc::convert
{
/** @brief Data describing a required asset conversion. */
struct Target
{
    std::filesystem::path sourcePath;
    std::filesystem::path destinationPath;
    std::optional<std::string> subResourceName;
};
}
