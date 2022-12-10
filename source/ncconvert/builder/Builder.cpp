#include "Builder.h"
#include "BuildInstructions.h"
#include "Target.h"
#include "converters/FbxConverter.h"
#include "converters/LegacytCubemapConverter.h"
#include "common/Serialize.h"

#include "fmt/format.h"
#include "ncutility/NcError.h"

#include <fstream>
#include <iostream>

namespace
{
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
Builder::Builder()
    : m_fbxConverter{std::make_unique<FbxConverter>()}
{
}

Builder::~Builder() = default;

auto Builder::Build(asset::AssetType type, const Target& target) -> bool
{
    switch (type)
    {
        case asset::AssetType::AudioClip:
        {
            throw NcError("not implemented");
        }
        case asset::AssetType::Cubemap:
        {
            ::BuildCubemap(target.sourcePath, target.destinationPath);
            break;
        }
        case asset::AssetType::ConcaveCollider:
        {
            ::BuildConcaveCollider(target.sourcePath, target.destinationPath, m_fbxConverter.get());
            break;
        }
        case asset::AssetType::HullCollider:
        {
            ::BuildHullCollider(target.sourcePath, target.destinationPath, m_fbxConverter.get());
            break;
        }
        case asset::AssetType::Mesh:
        {
            ::BuildMesh(target.sourcePath, target.destinationPath, m_fbxConverter.get());
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
            static_cast<int>(type), target.sourcePath.string()
        ));
    }

    return true;
}
} // namespace nc::convert
