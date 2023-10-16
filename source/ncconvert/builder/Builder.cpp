#include "Builder.h"
#include "BuildInstructions.h"
#include "Serialize.h"
#include "Target.h"
#include "utility/EnumExtensions.h"
#include "converters/AudioConverter.h"
#include "converters/GeometryConverter.h"
#include "converters/TextureConverter.h"
#include "utility/Log.h"
#include "utility/Path.h"

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

auto Builder::Build(asset::AssetType type, const Target& target, bool) -> bool
{
    if (CanOutputMany(type))
    {
        const auto internalAssetNames = ParseInternalAssetNames(type, target);

        for (const auto& internalAssetName : internalAssetNames)
        {           
            auto internalDestinationPath = AssetNameToNcaPath(internalAssetName, target.destinationPath).string();
            auto internalTarget = Target
            {
                .sourcePath = target.sourcePath,
                .destinationPath = internalDestinationPath
            };

            // if (::IsUpToDate(internalTarget) && !forceUpdate)
            // {
            //     LOG("Up-to-date: {}", internalDestinationPath);
            //     LOG("Removing build target: {} -> {}", target.sourcePath.string(), internalDestinationPath);
            //     continue;
            // }

            LOG("Building {}: {}", ToString(type), internalDestinationPath);
            if (!BuildAssets(type, internalTarget))
            {
                LOG("Failed building: {}", internalDestinationPath);
            }
        }
        return true;
    }

    // if (::IsUpToDate(target) && !forceUpdate)
    // {
    //     LOG("Up-to-date: {}", target.destinationPath.string());
    //     LOG("Removing build target: {} -> {}", target.sourcePath.string(), target.destinationPath.string());
    //     return true;
    // }

    LOG("Building {}: {}", ToString(type), target.destinationPath.string());
    if (!BuildAssets(type, target))
    {
        LOG("Failed building: {}", target.destinationPath.string());
    }

    return true;
}

auto Builder::BuildAssets(asset::AssetType type, const Target& target) -> bool
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
            const auto asset = m_geometryConverter->ImportMesh(target.sourcePath);
            convert::Serialize(outFile, asset, assetId);
            return true;
        }
        case asset::AssetType::Shader:
        {
            throw NcError("Not implemented");
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

const std::vector<std::string> Builder::ParseInternalAssetNames(asset::AssetType type, const Target& target)
{
    if (type == nc::asset::AssetType::Mesh)
    {
        return m_geometryConverter->ParseInternalMeshNames(target.sourcePath);
    }

    throw NcError(fmt::format("Unknown AssetType: {} for {}",
        static_cast<int>(type), target.sourcePath.string()
    ));
}

} // namespace nc::convert
