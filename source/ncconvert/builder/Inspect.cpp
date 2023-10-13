#include "Inspect.h"
#include "utility/Log.h"
#include "utility/EnumConversion.h"

#include "ncasset/Import.h"
#include "ncutility/NcError.h"

namespace
{
constexpr auto headerTemplate =
R"(File {}
Header
  magic number {}
  compression  {}
  id           {}
  size         {})";

constexpr auto audioClipTemplate =
R"(Data
  samples count {})";

constexpr auto concaveColliderTemplate =
R"(Data
  extents       {}, {}, {}
  max extent    {}
  vertex count  {})";

constexpr auto cubeMapTemplate =
R"(Data
  face side length {})";

constexpr auto hullColliderTemplate =
R"(Data
  extents        {}, {}, {}
  max extent     {}
  triangle count {})";

constexpr auto meshTemplate =
R"(Data
  extents                         {}, {}, {}
  max extent                      {}
  vertex count                    {}
  index count                     {}
  bones data vertex to bone count {}
  bones data bone to parent count {})";

constexpr auto skeletalAnimationClipTemplate =
R"(Data
  name              {}
  duration in ticks {}
  ticks per seconds {}
  frames per bone   {})";

constexpr auto textureTemplate =
R"(Data
  width  {}
  height {})";

} // anonymous namespace

namespace nc::convert
{
void Inspect(const std::filesystem::path& ncaPath)
{
    const auto header = asset::ImportNcaHeader(ncaPath);
    const auto type = GetAssetType(header);
    LOG(headerTemplate, ncaPath.string(), header.magicNumber, header.compressionAlgorithm, header.assetId, header.size);

    switch (type)
    {
        case asset::AssetType::AudioClip:
        {
            const auto asset = asset::ImportAudioClip(ncaPath);
            LOG(audioClipTemplate, asset.samplesPerChannel);
            break;
        }
        case asset::AssetType::ConcaveCollider:
        {
            const auto asset = asset::ImportConcaveCollider(ncaPath);
            LOG(concaveColliderTemplate, asset.extents.x, asset.extents.y, asset.extents.z, asset.maxExtent, asset.triangles.size());
            break;
        }
        case asset::AssetType::CubeMap:
        {
            const auto asset = asset::ImportCubeMap(ncaPath);
            LOG(cubeMapTemplate, asset.faceSideLength);
            break;
        }
        case asset::AssetType::HullCollider:
        {
            const auto asset = asset::ImportHullCollider(ncaPath);
            LOG(concaveColliderTemplate, asset.extents.x, asset.extents.y, asset.extents.z, asset.maxExtent, asset.vertices.size());
            break;
        }
        case asset::AssetType::Mesh:
        {
            const auto asset = asset::ImportMesh(ncaPath);
            LOG(meshTemplate, asset.extents.x, asset.extents.y, asset.extents.z, asset.maxExtent, asset.vertices.size(), asset.indices.size(), asset.bonesData.value().vertexSpaceToBoneSpace.size(), asset.bonesData.value().boneSpaceToParentSpace.size());
            break;
        }
        case asset::AssetType::Shader:
        {
            LOG("Shader not supported");
            break;
        }
        case asset::AssetType::SkeletalAnimationClip:
        {
            const auto asset = asset::ImportSkeletalAnimationClip(ncaPath);
            LOG(skeletalAnimationClipTemplate, asset.name, asset.durationInTicks, asset.ticksPerSecond, asset.framesPerBone.size());
            break;
        }
        case asset::AssetType::Texture:
        {
            const auto asset = asset::ImportTexture(ncaPath);
            LOG(textureTemplate, asset.width, asset.height);
            break;
        }
    }
}
} // namespace nc::convert
