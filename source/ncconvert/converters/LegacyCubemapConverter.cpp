#include "LegacyCubemapConverter.h"

#include "ncutility/NcError.h"

#include <algorithm>
#include <array>
#include <fstream>

namespace
{
auto ToAssetPath(const std::filesystem::path& meshPath, const std::filesystem::path& outputDirectory) -> std::filesystem::path
{
    std::filesystem::path outPath{outputDirectory};
    outPath /= meshPath.filename();
    outPath.replace_extension(".nca");
    return outPath;
}
}

namespace nc::convert
{
void ConvertCubeMap(const std::filesystem::path& inPath, const std::filesystem::path& outDirectory)
{
    if (!std::filesystem::is_directory(inPath))
    {
        throw NcError("Must specify a directory.");
    }

    const auto assetPath = ToAssetPath(inPath, outDirectory);
    std::ofstream outFile{assetPath};
    if(!outFile)
    {
        throw NcError("Failure opening asset file");
    }

    struct Entry
    {
        std::string face;
        bool found = false;
    };

    auto cubeMapFacePaths = std::array<Entry, 6>{
        Entry{"front"}, Entry{"back"}, Entry{"up"},
        Entry{"down"}, Entry{"right"}, Entry{"left"}
    };

    for (const auto& facePathIt : std::filesystem::directory_iterator{inPath})
    {
        const auto& facePath = facePathIt.path();
        auto facePathName = facePath.stem().string();
        auto facePathExt = facePath.extension().string();
        std::ranges::for_each(facePathName, [](auto& character){character = std::tolower(character);});

        auto pos = std::ranges::find_if(cubeMapFacePaths, [facePathName](const auto& entry)
        {
            return entry.face == facePathName;
        });

        if (pos == cubeMapFacePaths.end())
        {
            continue;
        }

        pos->found = true;
        auto outputDir = outDirectory / facePathName;
        outputDir.replace_extension(facePathExt);
        auto outputDirStr = outputDir.string();
        try
        {
            std::filesystem::copy_file(facePath, outputDir, std::filesystem::copy_options::overwrite_existing);
        }
        catch(const std::exception&)
        {
        }
        outputDirStr.erase(std::remove(outputDirStr.begin(), outputDirStr.end(), '\"'), outputDirStr.end());
        outFile << facePathName << facePathExt << '\n';
    }

    const bool isMissingImage = std::ranges::any_of(cubeMapFacePaths, [](const auto& entry)
    {
        return entry.found == false;
    });

    if (isMissingImage)
    {
        throw NcError("One or more cube map faces did not get created correctly.");
    }
}
}
