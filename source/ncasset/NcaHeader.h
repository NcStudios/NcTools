#pragma once

#include <cstddef>

namespace nc::asset
{
struct MagicNumber
{
    static constexpr auto audioClip = "CLIP";
    static constexpr auto concaveCollider = "CONC";
    static constexpr auto cubemap = "CUBE";
    static constexpr auto hullCollider = "HULL";
    static constexpr auto mesh = "MESH";
    static constexpr auto shader = "SHAD";
    static constexpr auto texture = "TEXT";
};

struct NcaHeader
{
    // Binary size does not include null terminators
    static constexpr auto binarySize = size_t{24};

    char magicNumber[5] = "NONE";
    char compressionAlgorithm[5] = "NONE";
    size_t assetId = 0;
    size_t size = 0;
};

struct HullCollider;
struct ConcaveCollider;
struct Mesh;

auto GetBlobSize(const HullCollider& asset) -> size_t;
auto GetBlobSize(const ConcaveCollider& asset) -> size_t;
auto GetBlobSize(const Mesh& asset) -> size_t;
} // namespace nc::asset
