#include "Builder.h"
#include "BuildInstructions.h"
#include "Serialize.h"
#include "Target.h"
#include "converters/AudioConverter.h"
#include "converters/GeometryConverter.h"
#include "converters/TextureConverter.h"
#include "utility/Log.h"

#include "ncasset/Assets.h"

#include "fmt/format.h"
#include "ncutility/Hash.h"
#include "ncutility/NcError.h"

#include <fstream>

namespace
{
auto OpenOutFile(const std::filesystem::path& outPath) -> std::ofstream
{
    if (!std::filesystem::exists(outPath) && outPath.has_parent_path())
    {
        const auto parentPath = outPath.parent_path();
        if (!std::filesystem::exists(parentPath) && !std::filesystem::create_directories(parentPath))
        {
            throw nc::NcError("Could not create parent directories for: ", outPath.string());
        }
    }

    auto outFile = std::ofstream{outPath, std::ios::binary};
    if (!outFile.is_open())
    {
        throw nc::NcError("Could not open output file: ", outPath.string());
    }

    return outFile;
}

auto GetAssetId(const std::filesystem::path& outPath) -> size_t
{
    const auto ncaName = outPath.filename();
    return nc::utility::Fnv1a(ncaName.string());
}
} // anonymous namespace

namespace nc::convert
{
Builder::Builder()
    : m_audioConverter{std::make_unique<AudioConverter>()},
      m_geometryConverter{std::make_unique<GeometryConverter>()},
      m_textureConverter{std::make_unique<TextureConverter>()}
{
}

Builder::~Builder() noexcept = default;

auto Builder::Build(asset::AssetType type, const Target& target) -> bool
{
    auto outFile = ::OpenOutFile(target.destinationPath);
    const auto assetId = ::GetAssetId(target.destinationPath);
    switch (type)
    {
        case asset::AssetType::AudioClip:
        {
            const auto asset = m_audioConverter->ImportAudioClip(target.sourcePath);
            convert::Serialize(outFile, asset, assetId);
            return true;
        }
        case asset::AssetType::CubeMap:
        {
            const auto asset = m_textureConverter->ImportCubeMap(target.sourcePath);
            convert::Serialize(outFile, asset, assetId);
            return true;
        }
        case asset::AssetType::ConcaveCollider:
        {
            const auto asset = m_geometryConverter->ImportConcaveCollider(target.sourcePath);
            convert::Serialize(outFile, asset, assetId);
            return true;
        }
        case asset::AssetType::HullCollider:
        {
            const auto asset = m_geometryConverter->ImportHullCollider(target.sourcePath);
            convert::Serialize(outFile, asset, assetId);
            return true;
        }
        case asset::AssetType::Mesh:
        {
            try
            {
                const auto asset = m_geometryConverter->ImportMesh(target.sourcePath, target.internalName);
                convert::Serialize(outFile, asset, assetId);
            }
            catch(const NcError& e)
            {
                if (std::string(e.what()).find(std::string("An internal mesh name was provided but no mesh")) != std::string::npos)
                {
                    LOG("Warning: ", e.what());
                }
                else
                {
                    throw e;
                }
            }
            return true;
        }
        case asset::AssetType::Shader:
        {
            throw NcError("Not implemented");
        }
        case asset::AssetType::SkeletalAnimation:
        {
            try
            {
                const auto asset = m_geometryConverter->ImportSkeletalAnimation(target.sourcePath, target.internalName);
                convert::Serialize(outFile, asset, assetId);
            }
            catch(const NcError& e)
            {
                if (std::string(e.what()).find(std::string("An internal skeletal animation name was provided but no animation")) != std::string::npos)
                {
                    LOG("Warning: ", e.what());
                }
                else
                {
                    throw e;
                }
            }
            return true;
        }
        case asset::AssetType::Texture:
        {
            const auto asset = m_textureConverter->ImportTexture(target.sourcePath);
            convert::Serialize(outFile, asset, assetId);
            return true;
        }
    }
    throw NcError(fmt::format("Unknown AssetType: {} for {}",
        static_cast<int>(type), target.sourcePath.string()
    ));
}
} // namespace nc::convert
