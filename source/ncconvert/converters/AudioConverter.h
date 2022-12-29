#pragma once

#include <filesystem>

namespace nc::asset
{
struct AudioClip;
} // namespace nc::asset

namespace nc::convert
{
class AudioConverter
{
    public:
        auto ImportAudioClip(const std::filesystem::path& path) -> asset::AudioClip;
};
} // namespace nc::convert
