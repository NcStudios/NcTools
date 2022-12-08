#include "Builder.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>

const auto DefaultAssetTargetFilename = std::string{"targets.txt"};

void Usage();
bool ParseArgs(int argc, char** argv, nc::convert::Config* config);
auto GetAssetType(std::string type) -> nc::asset::AssetType;
auto ReadTargets(const nc::convert::Config& config) -> std::vector<nc::convert::Target>;

int main(int argc, char** argv)
{
    auto config = nc::convert::Config{};
    if(!ParseArgs(argc, argv, &config))
    {
        Usage();
        return 0;
    }

    try
    {
        auto builder = nc::convert::Builder{config};
        for(const auto& target : ReadTargets(config))
        {
            if (!builder.Build(target))
            {
                std::cerr << "Failed converting: " << target.path.string() << '\n';
            }
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}

void Usage()
{
    std::cout << "Usage: build.exe [options]\n"
              << "Options:\n"
              << "  -h or --help            Display this information\n"
              << "  -t <target>             Parse a single asset from <target>\n"
              << "  -a <asset type>         Specify asset type for a single target.\n"
              << "  -m <manifest>           Parse multiple assets from <manifest>\n"
              << "  -o <dir>                Output assets to <dir>\n\n"
              
              << "  Valid asset types are 'mesh', 'hull-collider', 'concave-collider', and 'cubemap' and are\n"
              << "  case-insensitive.\n\n"

              << "  When using -m, <manifest> should be the path to a newline-separated list of\n"
              << "  pairs in the form '<asset-type> <path-to-input-file>'.\n\n"
              
              << "  If neither -t nor -m are used, the executable's directory will be searched\n"
              << "  for a manifest named \"targets.txt\".\n\n"
              
              << "  All paths should be absolute or relative to the current directory when\n"
              << "  calling build.exe.\n";
}

bool ParseArgs(int argc, char** argv, nc::convert::Config* out)
{
    if(argc == 1)
    {
        return false;
    }

    out->outputDirectory = std::filesystem::path("./");
    auto option = std::string{};
    auto current = 1;
    while (current < argc)
    {
        option = argv[current];

        if(option.compare("-h") == 0 || option.compare("--help") == 0)
        {
            return false;
        }

        if(++current >= argc)
        {
            return false;
        }

        if(option.compare("-t") == 0)
        {
            out->singleTargetPath = std::filesystem::path(argv[current++]);
            out->singleTargetPath.value().make_preferred();
            continue;
        }

        if(option.compare("-a") == 0)
        {
            out->singleTargetType = GetAssetType(std::string{argv[current++]});
            continue;
        }

        if(option.compare("-m") == 0)
        {
            out->targetsFilePath = std::filesystem::path(argv[current++]);
            out->targetsFilePath.value().make_preferred();
            continue;
        }

        if(option.compare("-o") == 0)
        {
            out->outputDirectory = std::filesystem::path(argv[current++]);
            out->outputDirectory.make_preferred();
            continue;
        }

        return false;
    }

    if(!out->singleTargetPath && !out->targetsFilePath)
    {
        out->targetsFilePath = std::filesystem::path(argv[0]).replace_filename(DefaultAssetTargetFilename);
        out->targetsFilePath.value().make_preferred();
    }

    return true;
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

    throw std::runtime_error("Failed to parse asset type: " + type);
}

auto ReadTargets(const nc::convert::Config& config) -> std::vector<nc::convert::Target>
{
    auto targets = std::vector<nc::convert::Target>{};

    if(config.singleTargetPath)
    {
        if(!config.singleTargetType)
            throw std::runtime_error("Single target must specify asset type with -c");

        targets.emplace_back(config.singleTargetPath.value(), config.singleTargetType.value());
    }

    if(!config.targetsFilePath)
        return targets;

    const auto& filePath = config.targetsFilePath.value();
    std::ifstream file{filePath};
    if(!file.is_open())
        throw std::runtime_error("Failure opening file: " + filePath.string());

    constexpr unsigned bufferSize = 512u;
    char buffer[bufferSize];
    std::cout << "Reading targets from: " << filePath.string() << '\n';

    while(!file.eof())
    {
        if(file.fail())
            throw std::runtime_error("Failure reading file: " + filePath.string());

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
