#include "Manifest.h"
#include "Target.h"
#include "utility/EnumConversion.h"
#include "utility/Log.h"
#include "utility/Path.h"

#include "ncutility/NcError.h"
#include "nlohmann/json.hpp"

#include <fstream>

namespace
{
const auto jsonAssetArrayTags = std::array<std::string, 6> {
    "audio-clip", "concave-collider", "cube-map", "hull-collider", "mesh", "texture"
};

struct GlobalManifestOptions
{
    std::filesystem::path outputDirectory;
    std::filesystem::path workingDirectory;
};

void from_json(const nlohmann::json& json, GlobalManifestOptions& options)
{
    options.outputDirectory = json.value("outputDirectory", "./");
    options.workingDirectory = json.value("workingDirectory", std::filesystem::path{});
}

void ProcessOptions(const GlobalManifestOptions& options, const std::filesystem::path& manifestPath)
{
    auto&& workingDirectory = options.workingDirectory.empty() ?
        std::filesystem::absolute(manifestPath.parent_path())
        : options.workingDirectory;

    LOG("Setting working directory: {}", workingDirectory.string());
    std::filesystem::current_path(workingDirectory);

    if(!std::filesystem::exists(options.outputDirectory))
    {
        LOG("Creating directory: {}", options.outputDirectory.string());
        if(!std::filesystem::create_directories(options.outputDirectory))
        {
            throw nc::NcError("Failed to create output directory: ", options.outputDirectory.string());
        }
    }
}

auto BuildTarget(const nlohmann::json& json, const std::filesystem::path& outputDirectory) -> nc::convert::Target
{
    return nc::convert::Target{
        json.at("sourcePath"),
        nc::convert::AssetNameToNcaPath(json.at("assetName"), outputDirectory)
    };
}

auto IsUpToDate(const nc::convert::Target& target) -> bool
{
    if (!std::filesystem::exists(target.destinationPath))
    {
        return false;
    }

    return std::filesystem::last_write_time(target.destinationPath) > std::filesystem::last_write_time(target.sourcePath);
}
} // anonymous namespace

namespace nc::convert
{
void ReadManifest(const std::filesystem::path& manifestPath, std::unordered_map<asset::AssetType, std::vector<Target>>& instructions)
{
    auto file = std::ifstream{manifestPath};
    if (!file.is_open())
    {
        throw nc::NcError{"Failed to open manifest: ", manifestPath.string()};
    }

    auto json = nlohmann::json::parse(file);
    auto options = json.value("globalOptions", ::GlobalManifestOptions{});
    ::ProcessOptions(options, manifestPath);

    for (const auto& typeTag : ::jsonAssetArrayTags)
    {
        if (!json.contains(typeTag))
        {
            continue;
        }

        const auto type = ToAssetType(typeTag);
        for (const auto& asset : json.at(typeTag))
        {
            auto target = ::BuildTarget(asset, options.outputDirectory);
            if (!std::filesystem::is_regular_file(target.sourcePath))
            {
                throw nc::NcError("Invalid source file: ", target.sourcePath.string());
            }

            if (::IsUpToDate(target))
            {
                LOG("Up-to-date: {}", target.destinationPath.string());
                continue;
            }

            LOG("Adding build target: {} -> {}", target.sourcePath.string(), target.destinationPath.string());
            instructions.at(type).push_back(std::move(target));
        }
    }
}
} // namespace nc::convert
