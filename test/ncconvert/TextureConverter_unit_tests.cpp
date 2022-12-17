#include "gtest/gtest.h"
#include "CollateralTexture.h"
#include "TextureTestUtility.h"

#include "converters/TextureConverter.h"
#include "ncasset/AssetTypes.h"

#include <algorithm>
#include <array>
#include <cstring>

TEST(TextureConverterTest, ImportTexture_png_convertsToNca)
{
    namespace test_data = collateral::rgb_corners;
    auto uut = nc::convert::TextureConverter{};
    const auto actual = uut.ImportTexture(test_data::pngFilePath);

    EXPECT_EQ(test_data::width, actual.width);
    EXPECT_EQ(test_data::height, actual.height);
    ASSERT_EQ(test_data::numBytes, actual.pixelData.size());

    for (auto pixelIndex = 0u; pixelIndex < test_data::numPixels; ++pixelIndex)
    {
        const auto expectedPixel = test_data::pixels[pixelIndex];
        const auto actualPixel = ReadPixel(actual.pixelData.data(), pixelIndex * 4);
        EXPECT_EQ(expectedPixel, actualPixel);
    }
}

TEST(TextureConverterTest, ImportTexture_jpg_convertsToNca)
{
    namespace test_data = collateral::rgb_corners;
    auto uut = nc::convert::TextureConverter{};
    const auto actual = uut.ImportTexture(test_data::jpgFilePath);

    EXPECT_EQ(test_data::width, actual.width);
    EXPECT_EQ(test_data::height, actual.height);
    ASSERT_EQ(test_data::numBytes, actual.pixelData.size());

    // Not testing pixels because jpeg
}

TEST(TextureConverterTest, ImportTexture_bmp_convertsToNca)
{
    namespace test_data = collateral::rgb_corners;
    auto uut = nc::convert::TextureConverter{};
    const auto actual = uut.ImportTexture(test_data::bmpFilePath);

    EXPECT_EQ(test_data::width, actual.width);
    EXPECT_EQ(test_data::height, actual.height);
    ASSERT_EQ(test_data::numBytes, actual.pixelData.size());

    for (auto pixelIndex = 0u; pixelIndex < test_data::numPixels; ++pixelIndex)
    {
        const auto expectedPixel = test_data::pixels[pixelIndex];
        const auto actualPixel = ReadPixel(actual.pixelData.data(), pixelIndex * 4);
        EXPECT_EQ(expectedPixel, actualPixel);
    }
}