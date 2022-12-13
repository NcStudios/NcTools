#include "Import.h"
#include "common/Deserialize.h"

#include "ncutility/NcError.h"

#include <cstring>
#include <fstream>

namespace
{
auto OpenNca(const std::filesystem::path& ncaPath) -> std::ifstream
{
    if (!std::filesystem::is_regular_file(ncaPath))
    {
        throw nc::NcError("File does not exist: ", ncaPath.string());
    }

    auto file = std::ifstream{ncaPath, std::ios::binary};
    if (!file.is_open())
    {
        throw nc::NcError("Could not open file: ", ncaPath.string());
    }

    return file;
}
} // anonymous namespace

namespace nc::asset
{
auto ImportConcaveCollider(const std::filesystem::path& ncaPath) -> ConcaveCollider
{
    auto file = ::OpenNca(ncaPath);
    auto [header, asset] = DeserializeConcaveCollider(file);
    return asset;
}

auto ImportHullCollider(const std::filesystem::path& ncaPath) -> HullCollider
{
    auto file = ::OpenNca(ncaPath);
    auto [header, asset] = DeserializeHullCollider(file);
    return asset;
}

auto ImportMesh(const std::filesystem::path& ncaPath) -> Mesh
{
    auto file = ::OpenNca(ncaPath);
    auto [header, asset] = DeserializeMesh(file);
    return asset;
}

auto ImportTexture(const std::filesystem::path& ncaPath) -> Texture
{
    auto file = ::OpenNca(ncaPath);
    auto [header, asset] = DeserializeTexture(file);
    return asset;
}
} // namespace nc::asset
