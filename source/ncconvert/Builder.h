#pragma once

#include "Config.h"

#include <memory>

namespace nc::convert
{
class FbxConverter;

class Builder
{
    public:
        Builder(Config config);
        ~Builder() noexcept;

        auto Build(const Target& target) -> bool;

    private:
        Config m_config;
        std::unique_ptr<FbxConverter> m_fbxConverter;
};
} // namespace nc::convert
