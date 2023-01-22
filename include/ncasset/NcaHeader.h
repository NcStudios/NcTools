#pragma once

#include <cstddef>

namespace nc::asset
{
/** @brief Identifiers for asset blobs in .nca files. */
struct MagicNumber
{
    static constexpr auto audioClip = "CLIP";
    static constexpr auto concaveCollider = "CONC";
    static constexpr auto cubeMap = "CUBE";
    static constexpr auto hullCollider = "HULL";
    static constexpr auto mesh = "MESH";
    static constexpr auto shader = "SHAD";
    static constexpr auto texture = "TEXT";
};

/** @brief Common file header for all asset types. */
struct NcaHeader
{
    /**
     * @brief Size of a serialized NcaHeader.
     * @note Binary size does not include null terminators.
     */
    static constexpr auto binarySize = size_t{24};

    /** @brief Asset type identifier. */
    char magicNumber[5] = "NONE";

    /** @brief Compression type for the asset blob. */
    char compressionAlgorithm[5] = "NONE";

    /** @brief The Fnv1a hash of the asset's friendly name. */
    size_t assetId = 0;

    /** @brief Size in bytes of the asset blob following this header. */
    size_t size = 0;
};

struct AudioClip;
struct CubeMap;
struct ConcaveCollider;
struct HullCollider;
struct Mesh;
struct Texture;

/** @brief Get the serialized size in bytes for an AudioClip. */
auto GetBlobSize(const AudioClip& asset) -> size_t;

/** @brief Get the serialized size in bytes for a ConcaveCollider. */
auto GetBlobSize(const ConcaveCollider& asset) -> size_t;

/** @brief Get the serialized size in bytes for a CubeMap. */
auto GetBlobSize(const CubeMap& asset) -> size_t;

/** @brief Get the serialized size in bytes for a HullCollider. */
auto GetBlobSize(const HullCollider& asset) -> size_t;

/** @brief Get the serialized size in bytes for a Mesh. */
auto GetBlobSize(const Mesh& asset) -> size_t;

/** @brief Get the serialized size in bytes for a Texture. */
auto GetBlobSize(const Texture& asset) -> size_t;
} // namespace nc::asset
