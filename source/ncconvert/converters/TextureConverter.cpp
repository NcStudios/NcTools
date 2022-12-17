#include "TextureConverter.h"
#include "ncasset/AssetTypes.h"
#include "utility/Path.h"

#include "ncutility/NcError.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include <algorithm>
#include <array>
#include <cstdlib>

namespace
{
const auto supportedFileExtensions = std::array<std::string, 4>{".png", ".jpg", ".jpeg", ".bmp"};
}

namespace nc::convert
{
auto TextureConverter::ImportTexture(const std::filesystem::path& path) -> asset::Texture
{
    if (!ValidateInputFile(path, supportedFileExtensions))
    {
        throw NcError("Invalid input file: ", path.string());
    }

    const auto pathString = path.string(); // because path.c_str() is wchar* on Windows
    auto width = int32_t{};
    auto height = int32_t {};
    auto nChannels = int32_t{};
    auto* rawPixels = stbi_load(pathString.c_str(), &width, &height, &nChannels, STBI_rgb_alpha);
    if(!rawPixels)
    {
        throw NcError("Failed to load texture file: ", pathString);
    }

    auto pixels = std::vector<unsigned char>{};
    const auto nBytes = width * height * asset::Texture::numChannels; // ignore nChannels because we force to 4 8-bit channels
    pixels.reserve(nBytes);
    std::copy(rawPixels, rawPixels + nBytes, std::back_inserter(pixels));
    ::free(rawPixels);

    return nc::asset::Texture{
        static_cast<uint32_t>(width),
        static_cast<uint32_t>(height),
        std::move(pixels)
    };
}
} // namespace nc::covnert
