#pragma once

#include <filesystem>

namespace nc::convert
{
/** @note This exists to retain cubemap support using the old format until textures are implemented. */
void ConvertCubeMap(const std::filesystem::path& inPath, const std::filesystem::path& outDirectory);
}
