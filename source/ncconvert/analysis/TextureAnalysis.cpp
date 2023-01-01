#include "TextureAnalysis.h"
#include "ncasset/AssetTypes.h"

#include "ncutility/NcError.h"

namespace nc::convert
{
auto GetCubeMapAtlasLayout(const nc::asset::Texture& texture) -> CubeMapAtlasLayout
{
    if (texture.height * 6u == texture.width)
    {
        return CubeMapAtlasLayout::HorizontalArray;
    }
    else if (texture.width * 6u == texture.height)
    {
        return CubeMapAtlasLayout::VerticalArray;
    }
    else if (texture.height * 4u == texture.width * 3u)
    {
        return CubeMapAtlasLayout::HorizontalCross;
    }
    else if (texture.width * 4u == texture.height * 3u)
    {
        return CubeMapAtlasLayout::VerticalCross;
    }
    else
    {
        throw nc::NcError("Invalid CubeMap dimensions");
    }
}

auto GetSubTextureInfo(const nc::asset::Texture& texture) -> CubeMapAtlasSubTextureInfo
{
    switch(GetCubeMapAtlasLayout(texture))
    {
        case CubeMapAtlasLayout::HorizontalArray:
        {
            const auto sideLength = texture.height;
            return CubeMapAtlasSubTextureInfo{
                .sideLength = sideLength,
                .frontPosition = SubTexturePos{0, 0},
                .backPosition = SubTexturePos{sideLength, 0},
                .topPosition = SubTexturePos{sideLength * 2, 0},
                .bottomPosition = SubTexturePos{sideLength * 3, 0},
                .rightPosition = SubTexturePos{sideLength * 4, 0},
                .leftPosition = SubTexturePos{sideLength * 5, 0}
            };
        }
        case CubeMapAtlasLayout::VerticalArray:
        {
            const auto sideLength = texture.width;
            return CubeMapAtlasSubTextureInfo{
                .sideLength = sideLength,
                .frontPosition = SubTexturePos{0, 0},
                .backPosition = SubTexturePos{0, sideLength},
                .topPosition = SubTexturePos{0, sideLength * 2},
                .bottomPosition = SubTexturePos{0, sideLength * 3},
                .rightPosition = SubTexturePos{0, sideLength * 4},
                .leftPosition = SubTexturePos{0, sideLength * 5}
            };
        }
        case CubeMapAtlasLayout::HorizontalCross:
        {
            const auto sideLength = texture.height / 3u;
            return CubeMapAtlasSubTextureInfo{
                .sideLength = sideLength,
                .frontPosition = SubTexturePos{sideLength, sideLength},
                .backPosition = SubTexturePos{sideLength * 3, sideLength},
                .topPosition = SubTexturePos{sideLength, 0},
                .bottomPosition = SubTexturePos{sideLength, sideLength * 2},
                .rightPosition = SubTexturePos{sideLength * 2, sideLength},
                .leftPosition = SubTexturePos{0, sideLength}
            };
        }
        case CubeMapAtlasLayout::VerticalCross:
        {
            const auto sideLength = texture.width / 3u;
            return CubeMapAtlasSubTextureInfo{
                .sideLength = sideLength,
                .frontPosition = SubTexturePos{0, sideLength},
                .backPosition = SubTexturePos{sideLength * 2, sideLength},
                .topPosition = SubTexturePos{sideLength, 0},
                .bottomPosition = SubTexturePos{sideLength, sideLength * 2},
                .rightPosition = SubTexturePos{sideLength, sideLength},
                .leftPosition = SubTexturePos{sideLength, sideLength * 3}
            };
        }
        default:
        {
            throw NcError{"Invalid CubeMapAtlasLayout"};
        }
    }
}
} // namespace nc::convert
