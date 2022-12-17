#pragma once

#include "CollateralCommon.h"

#include <array>
#include <cstdint>

namespace collateral
{

constexpr auto red = uint32_t{0xFF0000FF};
constexpr auto green = uint32_t{0x00FF00FF};
constexpr auto blue = uint32_t{0x0000FFFF};
constexpr auto black = uint32_t{0x000000FF};
constexpr auto white = uint32_t{0xFFFFFFFF};

// Describes the collateral files named rgb_corners_4x8.XXX
namespace rgb_corners
{
const auto bmpFilePath = collateralDirectory / "rgb_corners_4x8.bmp";
const auto jpgFilePath = collateralDirectory / "rgb_corners_4x8.jpg";
const auto pngFilePath = collateralDirectory / "rgb_corners_4x8.png";
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
} // namespace collateral
