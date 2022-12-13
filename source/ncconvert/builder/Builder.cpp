#include "Builder.h"
#include "BuildInstructions.h"
#include "Target.h"
#include "converters/GeometryConverter.h"
#include "converters/LegacytCubemapConverter.h"
#include "converters/TextureConverter.h"
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
                          nc::convert::GeometryConverter* converter)
{
    const auto asset = converter->ImportConcaveCollider(inPath);
    auto outFile = ::OpenOutFile(outPath);
    auto assetId = size_t{};
    nc::asset::Serialize(outFile, asset, assetId);
}

void BuildHullCollider(const std::filesystem::path& inPath,
                       const std::filesystem::path& outPath,
                       nc::convert::GeometryConverter* converter)
{
    const auto asset = converter->ImportHullCollider(inPath);
    auto outFile = ::OpenOutFile(outPath);
    auto assetId = size_t{};
    nc::asset::Serialize(outFile, asset, assetId);
}

void BuildMesh(const std::filesystem::path& inPath,
               const std::filesystem::path& outPath,
               nc::convert::GeometryConverter* converter)
{
    const auto asset = converter->ImportMesh(inPath);
    auto outFile = ::OpenOutFile(outPath);
    auto assetId = size_t{};
    nc::asset::Serialize(outFile, asset, assetId);
}

void BuildCubeMap(const std::filesystem::path& inPath,
                  const std::filesystem::path& outDirectory)
{
    nc::convert::ConvertCubeMap(inPath, outDirectory);
}

void BuildTexture(const std::filesystem::path& inPath,
                  const std::filesystem::path& outPath,
                  nc::convert::TextureConverter* converter)
{
    const auto asset = converter->ImportTexture(inPath);
    auto outFile = ::OpenOutFile(outPath);
    auto assetId = size_t{};
    nc::asset::Serialize(outFile, asset, assetId);
}
} // anonymous namespace

namespace nc::convert
{
Builder::Builder()
    : m_geometryConverter{std::make_unique<GeometryConverter>()},
      m_textureConverter{std::make_unique<TextureConverter>()}
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
        case asset::AssetType::CubeMap:
        {
            ::BuildCubeMap(target.sourcePath, target.destinationPath);
            break;
        }
        case asset::AssetType::ConcaveCollider:
        {
            ::BuildConcaveCollider(target.sourcePath, target.destinationPath, m_geometryConverter.get());
            break;
        }
        case asset::AssetType::HullCollider:
        {
            ::BuildHullCollider(target.sourcePath, target.destinationPath, m_geometryConverter.get());
            break;
        }
        case asset::AssetType::Mesh:
        {
            ::BuildMesh(target.sourcePath, target.destinationPath, m_geometryConverter.get());
            break;
        }
        case asset::AssetType::Shader:
        {
            throw NcError("not implemented");
        }
        case asset::AssetType::Texture:
        {
            ::BuildTexture(target.sourcePath, target.destinationPath, m_textureConverter.get());
            break;
        }

        throw NcError(fmt::format("Unknown AssetType: {} for {}",
            static_cast<int>(type), target.sourcePath.string()
        ));
    }

    return true;
}
} // namespace nc::convert
