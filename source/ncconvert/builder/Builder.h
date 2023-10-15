#pragma once

#include "ncasset/AssetType.h"

#include <memory>
#include <vector>

namespace nc::convert
{
struct Target;
class AudioConverter;
class GeometryConverter;
class TextureConverter;

/** @brief Manager that handles nca conversion and serialization. */
class Builder
{
    public:
        Builder();
        ~Builder() noexcept;

        /** @brief Create a new .nca file. */
        auto Build(asset::AssetType type, const Target& target, bool forceUpdate) -> bool;

        /** @brief Some asset types can contain multiple assets in their source files. 
         * This method returns the names of those assets in the order in which they're presented in the software. */
        const std::vector<std::string> ParseInternalAssetNames(asset::AssetType type, const Target& target);

    private:
        auto BuildAssets(asset::AssetType type, const Target& target) -> bool;
        std::unique_ptr<AudioConverter> m_audioConverter;
        std::unique_ptr<GeometryConverter> m_geometryConverter;
        std::unique_ptr<TextureConverter> m_textureConverter;
};
} // namespace nc::convert
