#pragma once

#include <filesystem>
#include <span>
#include <string>

namespace nc::convert
{
auto ValidateInputFile(const std::filesystem::path& path, std::span<const std::string> validExtensions) -> bool;
}
