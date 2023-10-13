#include "SkeletalAnimationReader.h"

namespace nc::asset
{
auto ReadSkeletalAnimationFrames(RawNcaBuffer& bytes, size_t framesPerBoneSize) -> std::unordered_map<std::string, nc::asset::SkeletalAnimationFrames>
{
    auto framesPerBone = std::unordered_map<std::string, nc::asset::SkeletalAnimationFrames>{};
    framesPerBone.reserve(framesPerBoneSize);
    for (auto i = 0u; i < framesPerBoneSize; i++)
    {
        auto boneNameSize = size_t{};
        bytes.Read(&boneNameSize);
        auto boneName = std::string{};
        boneName.resize(boneNameSize);
        bytes.Read(boneName.data(), boneNameSize);
        auto positionFramesSize = size_t{};
        bytes.Read(&positionFramesSize);
        auto positionFrames = std::vector<nc::asset::PositionFrame>{};
        positionFrames.resize(positionFramesSize);
        bytes.Read(positionFrames.data(), positionFramesSize * sizeof(nc::asset::PositionFrame));
        auto rotationFramesSize = size_t{};
        bytes.Read(&rotationFramesSize);
        auto rotationFrames = std::vector<nc::asset::RotationFrame>{};
        rotationFrames.resize(rotationFramesSize);
        bytes.Read(rotationFrames.data(), rotationFramesSize * sizeof(nc::asset::RotationFrame));
        auto scaleFramesSize = size_t{};
        bytes.Read(&scaleFramesSize);
        auto scaleFrames = std::vector<nc::asset::ScaleFrame>{};
        scaleFrames.resize(scaleFramesSize);
        bytes.Read(scaleFrames.data(), scaleFramesSize * sizeof(nc::asset::ScaleFrame));
        framesPerBone.emplace(std::make_pair<std::string, nc::asset::SkeletalAnimationFrames>(std::move(boneName), 
        nc::asset::SkeletalAnimationFrames
        {
            std::move(positionFrames),
            std::move(rotationFrames),
            std::move(scaleFrames)
        }));
    }
    return framesPerBone;
}
} // namespace nc::asset
