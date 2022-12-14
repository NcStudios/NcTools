#include "gtest/gtest.h"
#include "converters/TextureConverter.h"
#include "ncasset/AssetTypes.h"

#include <algorithm>
#include <array>
#include <cstring>

const auto collateralDirectory = std::filesystem::path{TEST_COLLATERAL_DIR};

constexpr auto red = uint32_t{0xFF0000FF};
constexpr auto green = uint32_t{0x00FF00FF};
constexpr auto blue = uint32_t{0x0000FFFF};
constexpr auto black = uint32_t{0x000000FF};
constexpr auto white = uint32_t{0xFFFFFFFF};

auto ReadPixel(const unsigned char* data, size_t position) -> uint32_t
{
    const auto r = uint32_t{data[position++]} << 24;
    const auto g = uint32_t{data[position++]} << 16;
    const auto b = uint32_t{data[position++]} << 8;
    const auto a = uint32_t{data[position]};
    return r | g | b | a;
}

// Describes the collateral files named rgb_corners_4x8.XXX
namespace rgb_corners
{
constexpr auto width = 4u;
constexpr auto height = 8u;
constexpr auto numPixels = width * height;
constexpr auto numBytes = numPixels * 4u;
constexpr auto pixels = std::array<uint32_t, 32>{
    red,   white, white, green,
    white, white, white, white,
    white, white, white, white,
    white, white, white, white,
    white, white, white, white,
    white, white, white, white,
    white, white, white, white,
    blue,  white, white, black
};
} // namespace rgb_corners

TEST(TextureConverterTest, ImportTexture_png_convertsToNca)
{
    auto uut = nc::convert::TextureConverter{};
    const auto actual = uut.ImportTexture(collateralDirectory / "rgb_corners_4x8.png");

    EXPECT_EQ(rgb_corners::width, actual.width);
    EXPECT_EQ(rgb_corners::height, actual.height);
    ASSERT_EQ(rgb_corners::numBytes, actual.pixelData.size());

    for (auto pixelIndex = 0u; pixelIndex < rgb_corners::numPixels; ++pixelIndex)
    {
        const auto expectedPixel = rgb_corners::pixels[pixelIndex];
        const auto actualPixel = ReadPixel(actual.pixelData.data(), pixelIndex * 4);
        EXPECT_EQ(expectedPixel, actualPixel);
    }
}

TEST(TextureConverterTest, ImportTexture_jpg_convertsToNca)
{
    auto uut = nc::convert::TextureConverter{};
    const auto actual = uut.ImportTexture(collateralDirectory / "rgb_corners_4x8.jpg");

    EXPECT_EQ(rgb_corners::width, actual.width);
    EXPECT_EQ(rgb_corners::height, actual.height);
    ASSERT_EQ(rgb_corners::numBytes, actual.pixelData.size());

    // Not testing pixels because jpeg
}

TEST(TextureConverterTest, ImportTexture_bmp_convertsToNca)
{
    auto uut = nc::convert::TextureConverter{};
    const auto actual = uut.ImportTexture(collateralDirectory / "rgb_corners_4x8.bmp");

    EXPECT_EQ(rgb_corners::width, actual.width);
    EXPECT_EQ(rgb_corners::height, actual.height);
    ASSERT_EQ(rgb_corners::numBytes, actual.pixelData.size());

    for (auto pixelIndex = 0u; pixelIndex < rgb_corners::numPixels; ++pixelIndex)
    {
        const auto expectedPixel = rgb_corners::pixels[pixelIndex];
        const auto actualPixel = ReadPixel(actual.pixelData.data(), pixelIndex * 4);
        EXPECT_EQ(expectedPixel, actualPixel);
    }
}