#include "Path.h"

#include <algorithm>

namespace nc::convert
{
auto ValidateInputFile(const std::filesystem::path& path, std::span<const std::string> validExtensions) -> bool
{
    if (!std::filesystem::is_regular_file(path))
    {
        return false;
    }

    const auto extension = path.extension().string();
    return std::ranges::find(validExtensions, extension) != validExtensions.end();
}
}
