#include "BuildInstructions.h"
#include "Config.h"
#include "Target.h"
#include "utility/EnumConversion.h"

#include "ncutility/NcError.h"

#include <fstream>
#include <iostream>

namespace
{
auto GetOutputName(const std::filesystem::path& inPath, const std::filesystem::path& outDir) -> std::filesystem::path
{
    const auto ncaFileName = inPath.filename().replace_extension(".nca");
    return outDir / ncaFileName;
}
} // anonymous namespace

namespace nc::convert
{
BuildInstructions::BuildInstructions(const Config& config)
    : m_instructions{}
{
    m_instructions.emplace(asset::AssetType::CubeMap, std::vector<Target>{});
    m_instructions.emplace(asset::AssetType::ConcaveCollider, std::vector<Target>{});
    m_instructions.emplace(asset::AssetType::HullCollider, std::vector<Target>{});
    m_instructions.emplace(asset::AssetType::Mesh, std::vector<Target>{});
    ReadTargets(config);
}

auto BuildInstructions::GetTargetsForType(asset::AssetType type) const -> const std::vector<Target>&
{
    return m_instructions.at(type);
}

void BuildInstructions::ReadTargets(const Config& config)
{
    if(config.singleTargetPath)
    {
        if(!config.singleTargetType)
        {
            throw nc::NcError("Single target mode must specify asset type with -a");
        }

        auto& collection = m_instructions.at(config.singleTargetType.value());
        collection.emplace_back(
            config.singleTargetPath.value(),
            ::GetOutputName(config.singleTargetPath.value(),
                            config.outputDirectory)
        );
        return;
    }

    if(!config.targetsFilePath)
    {
        throw nc::NcError("Manifest mode must specify a manifest path");
    }

    const auto& manifestPath = config.targetsFilePath.value();
    auto manifestFile = std::ifstream{manifestPath};
    if(!manifestFile.is_open())
    {
        throw nc::NcError("Failure opening manifest: " + manifestPath.string());
    }

    constexpr unsigned bufferSize = 512u;
    char buffer[bufferSize];
    std::cout << "Reading targets from: " << manifestPath << '\n';

    while(!manifestFile.eof())
    {
        if(manifestFile.fail())
        {
            throw nc::NcError("Failure reading manifest: " + manifestPath.string());
        }

        auto newTarget = Target{};
        manifestFile.getline(buffer, bufferSize, ' ');
        const auto type = ToAssetType(buffer);

        manifestFile.getline(buffer, bufferSize, '\n');
        newTarget.sourcePath = buffer;
        newTarget.sourcePath.make_preferred();
        newTarget.destinationPath = ::GetOutputName(newTarget.sourcePath, config.outputDirectory);

        std::cout << "Found target:\n"
                  << "  type: " << nc::convert::ToString(type) << '\n'
                  << "  source: " << newTarget.sourcePath << '\n'
                  << "  destination: " << newTarget.destinationPath << '\n';

        auto& collection = m_instructions.at(type);
        collection.push_back(std::move(newTarget));
    }

    manifestFile.close();
}
} // namespace nc::convert
