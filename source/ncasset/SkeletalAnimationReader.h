#pragma once

#include "ncasset/Assets.h"
#include "RawNcaBuffer.h"

namespace nc::asset
{
    /** @brief Read an unordered_map of bone names to a struct containing vectors of animation transform frames from a RawNcaBuffer byte stream.
     * 
     * @param bytes The byte stream to read from.
     * @param framesPerBoneSize The count of pairs in the perFrameBones unordered_map.
     * @returns An unordered_map of bone names to a struct containing vectors of animation transform frames
     */
    auto ReadSkeletalAnimationFrames(RawNcaBuffer& bytes, size_t framesPerBoneSize) -> std::unordered_map<std::string, nc::asset::SkeletalAnimationFrames>;
} // namespace nc::asset
