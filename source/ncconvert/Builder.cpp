#include "Builder.h"
#include "converters/FbxConverter.h"
#include "converters/LegacytCubemapConverter.h"
#include "common/Serialize.h"

#include "fmt/format.h"
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

auto OpenOutFile(const std::filesystem::path& outPath) -> std::ofstream
{
    auto outFile = std::ofstream{outPath, std::ios::binary};
    if (!outFile.is_open())
    {
        throw nc::NcError("Could not open output file: ", outPath.string());
    }

    return outFile;
}

void BuildConcaveCollider(const std::filesystem::path& inPath,
                          const std::filesystem::path& outPath,
                          nc::convert::FbxConverter* converter)
{
    const auto asset = converter->ImportConcaveCollider(inPath);
    auto outFile = ::OpenOutFile(outPath);
    auto assetId = size_t{};
    nc::asset::Serialize(outFile, asset, assetId);
}

void BuildHullCollider(const std::filesystem::path& inPath,
                       const std::filesystem::path& outPath,
                       nc::convert::FbxConverter* converter)
{
    const auto asset = converter->ImportHullCollider(inPath);
    auto outFile = ::OpenOutFile(outPath);
    auto assetId = size_t{};
    nc::asset::Serialize(outFile, asset, assetId);
}

void BuildMesh(const std::filesystem::path& inPath,
               const std::filesystem::path& outPath,
               nc::convert::FbxConverter* converter)
{
    const auto asset = converter->ImportMesh(inPath);
    auto outFile = ::OpenOutFile(outPath);
    auto assetId = size_t{};
    nc::asset::Serialize(outFile, asset, assetId);
}

void BuildCubemap(const std::filesystem::path& inPath,
                  const std::filesystem::path& outDirectory)
{
    nc::convert::ConvertCubemap(inPath, outDirectory);
}
} // anonymous namespace

namespace nc::convert
{
Builder::Builder(Config config)
    : m_config{std::move(config)},
      m_fbxConverter{std::make_unique<FbxConverter>()}
{
    if(!std::filesystem::exists(m_config.outputDirectory))
    {
        std::cout << "Creating directory: " << m_config.outputDirectory << '\n';
        if(!std::filesystem::create_directories(m_config.outputDirectory))
        {
            throw NcError("Failed to create output directory: ", m_config.outputDirectory.string());
        }
    }
}

Builder::~Builder() = default;

auto Builder::Build(const Target& target) -> bool
{
    const auto outPath = ::GetOutputName(target.path, m_config.outputDirectory);
    switch (target.type)
    {
        case asset::AssetType::AudioClip:
        {
            throw NcError("not implemented");
        }
        case asset::AssetType::Cubemap:
        {
            ::BuildCubemap(target.path, m_config.outputDirectory);
            break;
        }
        case asset::AssetType::ConcaveCollider:
        {
            ::BuildConcaveCollider(target.path, outPath, m_fbxConverter.get());
            break;
        }
        case asset::AssetType::HullCollider:
        {
            ::BuildHullCollider(target.path, outPath, m_fbxConverter.get());
            break;
        }
        case asset::AssetType::Mesh:
        {
            ::BuildMesh(target.path, outPath, m_fbxConverter.get());
            break;
        }
        case asset::AssetType::Shader:
        {
            throw NcError("not implemented");
        }
        case asset::AssetType::Texture:
        {
            throw NcError("not implemented");
        }

        throw NcError(fmt::format("Unknown AssetType: {} for {}",
            static_cast<int>(target.type), target.path.string()
        ));
    }

    return true;
}
} // namespace nc::convert
