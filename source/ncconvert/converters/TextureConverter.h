#pragma once

#include <filesystem>

namespace nc::asset
{
struct CubeMap;
struct Texture;
} // namesapce nc::asset

namespace nc::convert
{
class TextureConverter
{
    public:
        auto ImportCubeMap(const std::filesystem::path& path) -> asset::CubeMap;
        auto ImportTexture(const std::filesystem::path& path) -> asset::Texture;
};
} // namespace nc::convert
