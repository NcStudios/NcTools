#include "Config.h"

#include "ncutility/NcError.h"

#include <algorithm>
#include <fstream>
#include <iostream>

namespace nc::convert
{
auto ReadTargets(const nc::convert::Config& config) -> std::vector<nc::convert::Target>
{
    auto targets = std::vector<nc::convert::Target>{};

    if(config.singleTargetPath)
    {
        if(!config.singleTargetType)
        {
            throw nc::NcError("Single target mode must specify asset type with -a");
        }

        targets.emplace_back(config.singleTargetPath.value(), config.singleTargetType.value());
        return targets;
    }

    if(!config.targetsFilePath)
    {
        throw nc::NcError("Manifest mode must specify a manifest path");
    }

    const auto& filePath = config.targetsFilePath.value();
    std::ifstream file{filePath};
    if(!file.is_open())
    {
        throw nc::NcError("Failure opening manifest: " + filePath.string());
    }

    constexpr unsigned bufferSize = 512u;
    char buffer[bufferSize];
    std::cout << "Reading targets from: " << filePath.string() << '\n';

    while(!file.eof())
    {
        if(file.fail())
        {
            throw nc::NcError("Failure reading manifest: " + filePath.string());
        }

        auto newTarget = nc::convert::Target{};
        file.getline(buffer, bufferSize, ' ');
        newTarget.type = GetAssetType(std::string{buffer});

        file.getline(buffer, bufferSize, '\n');
        newTarget.path = buffer;
        newTarget.path.make_preferred();
        std::cout << "    " << newTarget.path << '\n';
        targets.push_back(newTarget);
    }

    file.close();
    return targets;
}

auto GetAssetType(std::string type) -> nc::asset::AssetType
{
    std::ranges::transform(type, type.begin(), [](char c) { return std::tolower(c); });

    if(type == "mesh")
        return nc::asset::AssetType::Mesh;
    else if(type == "hull-collider")
        return nc::asset::AssetType::HullCollider;
    else if(type == "concave-collider")
        return nc::asset::AssetType::ConcaveCollider;
    else if(type == "cubemap")
        return nc::asset::AssetType::Cubemap;

    throw nc::NcError("Failed to parse asset type: " + type);
}
} // namespace nc::convert
