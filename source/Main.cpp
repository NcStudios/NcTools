#include "Builders.h"

#include <optional>

struct Config
{
    std::optional<std::filesystem::path> SingleTargetPath;
    std::optional<AssetType> SingleTargetType;
    std::optional<std::filesystem::path> TargetsFilePath;
    std::filesystem::path OutputDirectory;
};

void Usage();
bool ParseArgs(int argc, char** argv, Config* config);
auto GetAssetType(std::string type) -> AssetType;
void CreateOutputDirectory(const std::filesystem::path& directory);
auto ReadTargets(const Config& config) -> std::vector<Target>;
bool IsValidMeshExtension(const std::filesystem::path& extension);
auto ToAssetPath(const std::filesystem::path& meshPath, const Config& config) -> std::filesystem::path;

int main(int argc, char** argv)
{
    Config config;
    if(!ParseArgs(argc, argv, &config))
    {
        Usage();
        return 1;
    }

    try
    {
        CreateOutputDirectory(config.OutputDirectory);
        Assimp::Importer importer;
        for(const auto& target : ReadTargets(config))
        {
            auto outPath = ToAssetPath(target.path, config);
            BuildAsset(&importer, target, outPath);
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
    std::cout << "Usage:\n"
              << "  NcAssetBuilder -a TYPE -i IN\n"
              << "  NcAssetBuilder -a TYPE -i IN -o OUT\n"
              << "  NcAssetBuilder -m MANIFEST -o OUT\n\n"

              << "  Valid asset types are 'mesh', 'hull-collider', 'concave-collider', and 'skybox' and are\n"
              << "  case-insensitive.\n\n"

              << "  When using -m, manifest should be the path to a newline-separated list of\n"
              << "  pairs in the form '<asset-type> <path-to-input-file>'.\n\n"

              << "  All paths should be absolute or relative to the current working directory.\n";
}

bool ParseArgs(int argc, char** argv, Config* out)
{
    out->OutputDirectory = std::filesystem::path("./");

    if(argc == 1)
        return false;

    std::string option;
    int current = 1;
    while (current < argc)
    {
        option = argv[current];

        if(option.compare("-h") == 0 || option.compare("--help") == 0)
            return false;

        if(++current >= argc)
            return false;

        if(option.compare("-i") == 0)
        {
            out->SingleTargetPath = std::filesystem::path(argv[current++]);
            out->SingleTargetPath.value().make_preferred();
            continue;
        }

        if(option.compare("-a") == 0)
        {
            out->SingleTargetType = GetAssetType(std::string{argv[current++]});
            continue;
        }

        // if(option.compare("-m") == 0)
        // {
        //     out->TargetsFilePath = std::filesystem::path(argv[current++]);
        //     out->TargetsFilePath.value().make_preferred();
        //     continue;
        // }

        if(option.compare("-o") == 0)
        {
            out->OutputDirectory = std::filesystem::path(argv[current++]);
            out->OutputDirectory.make_preferred();
            continue;
        }

        return false;
    }

    if(!out->SingleTargetPath && !out->TargetsFilePath)
    {
        out->TargetsFilePath = std::filesystem::path(argv[0]).replace_filename(DefaultAssetTargetFilename);
        out->TargetsFilePath.value().make_preferred();
    }

    return true;
}

auto GetAssetType(std::string type) -> AssetType
{
    std::ranges::transform(type, type.begin(), [](char c) { return std::tolower(c); });

    if(type.compare("mesh") == 0)
        return AssetType::Mesh;
    else if(type.compare("hull-collider") == 0)
        return AssetType::ConvexHull;
    else if(type.compare("concave-collider") == 0)
        return AssetType::ConcaveCollider;
    else if(type.compare("skybox") == 0)
        return AssetType::Skybox;

    throw std::runtime_error("Failed to parse asset type: " + type);
}

void CreateOutputDirectory(const std::filesystem::path& directory)
{
    if(std::filesystem::exists(directory))
        return;

    std::cout << "Creating directory: " << directory << '\n';
    if(!std::filesystem::create_directories(directory))
        throw std::runtime_error("Failed to create output directory: " + directory.string());
}

auto ReadTargets(const Config& config) -> std::vector<Target>
{
    std::vector<Target> out;

    if(config.SingleTargetPath)
    {
        if(!config.SingleTargetType)
            throw std::runtime_error("Single target must specify asset type with -c");

        out.emplace_back(config.SingleTargetPath.value(), config.SingleTargetType.value());
    }

    if(!config.TargetsFilePath)
        return out;

    const auto& filePath = config.TargetsFilePath.value();
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
        
        Target newTarget;
        file.getline(buffer, bufferSize, ' ');
        newTarget.type = GetAssetType(std::string{buffer});

        file.getline(buffer, bufferSize, '\n');
        newTarget.path = buffer;
        newTarget.path.make_preferred();
        std::cout << "    " << newTarget.path << '\n';
        out.push_back(newTarget);
    }

    file.close();
    return out;
}

auto ToAssetPath(const std::filesystem::path& meshPath, const Config& config) -> std::filesystem::path
{
    std::filesystem::path outPath{config.OutputDirectory};
    outPath /= meshPath.filename();
    outPath.replace_extension(AssetExtension);
    return outPath;
}