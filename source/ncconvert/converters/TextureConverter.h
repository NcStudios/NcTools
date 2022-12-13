#pragma once

#include <filesystem>

namespace nc::asset
{
struct Texture;
} // namesapce nc::asset

namespace nc::convert
{
class TextureConverter
{
    public:
        auto ImportTexture(const std::filesystem::path& path) -> asset::Texture;
};
} // namespace nc::convert
