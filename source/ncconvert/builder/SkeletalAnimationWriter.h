#pragma once

#include "ncasset/Assets.h"

#include <ostream>

namespace nc::convert
{
/** @brief Write a SkeletalAnimationFrames object to a byte stream.
 * 
 * @param stream The byte stream to write to.
 * @param skeletalAnimationFrames A mapping between bones and the vectors representing the current transforms at a given time.
 */
void Write(std::ostream& stream, const std::unordered_map<std::string, nc::asset::SkeletalAnimationFrames>& framesPerBone);
} // namespace nc::convert
