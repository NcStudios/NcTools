#pragma once

#include "ncasset/AssetTypes.h"

#include <memory>

namespace nc::convert
{
struct Target;
class FbxConverter;

/** @brief Manager that handles nca conversion and serialization. */
class Builder
{
    public:
        Builder();
        ~Builder() noexcept;

        /** @brief Create a new .nca file. */
        auto Build(asset::AssetType type, const Target& target) -> bool;

    private:
        std::unique_ptr<FbxConverter> m_fbxConverter;
};
} // namespace nc::convert
