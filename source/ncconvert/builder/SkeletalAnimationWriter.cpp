#include "SkeletalAnimationWriter.h"
#include "builder/DataWriter.h"

#include <cstring>

namespace nc::convert
{
void Write(std::ostream& stream, const std::unordered_map<std::string, nc::asset::SkeletalAnimationFrames>& framesPerBone)
{
    for (auto framesPerBoneIt=framesPerBone.begin(); framesPerBoneIt!=framesPerBone.end(); framesPerBoneIt++)
    {
        Write(stream, framesPerBoneIt->first.size()); // name size (unordered_map key)
        Write(stream, framesPerBoneIt->first.data(), framesPerBoneIt->first.size()); // name (unordered_map key)
        const auto& frames = framesPerBoneIt->second;
        Write(stream, frames.positionFrames.size()); // std::vector<nc::asset::PositionFrame> count
        Write(stream, frames.positionFrames.data(), frames.positionFrames.size() * sizeof(nc::asset::PositionFrame)); // std::vector<nc::asset::PositionFrame>
        Write(stream, frames.rotationFrames.size()); // std::vector<nc::asset::RotationFrame> count
        Write(stream, frames.rotationFrames.data(), frames.rotationFrames.size() * sizeof(nc::asset::RotationFrame)); // std::vector<nc::asset::RotationFrame>
        Write(stream, frames.scaleFrames.size()); // std::vector<nc::asset::ScaleFrame> count
        Write(stream, frames.scaleFrames.data(), frames.scaleFrames.size() * sizeof(nc::asset::ScaleFrame)); // std::vector<nc::asset::ScaleFrame>
    }
}
} // namespace nc::convert
