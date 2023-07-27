#pragma once

#include "ncasset/Assets.h"
#include "ncasset/RawNcaBuffer.h"

#include <ostream>

namespace nc::convert
{
void Write(std::ostream& stream, const std::unordered_map<std::string, uint32_t>& boneNamesToIds);
void Write(std::ostream& stream, const nc::asset::BodySpaceNode* parentNode, uint32_t generation);
void Read(nc::asset::RawNcaBuffer& bytes, std::unordered_map<std::string, uint32_t>* boneNamesToIds, size_t numBones);
}
