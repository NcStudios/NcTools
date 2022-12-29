#include "AudioConverter.h"
#include "ncasset/AssetTypes.h"
#include "utility/Path.h"

#include "audio_file/AudioFile.h"
#include "ncutility/NcError.h"

#include <array>

namespace
{
const auto supportedFileExtensions = std::array<std::string, 1> { ".wav" };
}

namespace nc::convert
{
auto AudioConverter::ImportAudioClip(const std::filesystem::path& path) -> asset::AudioClip
{
    if (!ValidateInputFile(path, supportedFileExtensions))
    {
        throw NcError("Invalid input file: ", path.string());
    }

    auto rawAsset = AudioFile<double>{};
    if(!rawAsset.load(path.string()))
    {
        throw NcError("Failure opening audio file: ", path.string());
    }

    return asset::AudioClip{
        rawAsset.samples.at(0).size(),
        std::move(rawAsset.samples.at(0)),
        std::move(rawAsset.samples.at(1))
    };
}
} // namespace nc::convert
