#pragma once

#include "ncasset/Assets.h"

#include <ostream>

namespace nc::convert
{
void Write(std::ostream& stream, const std::unordered_map<std::string, uint32_t>& boneNamesToIds);
void Write(std::ostream& stream, const std::vector<nc::asset::BoneParentOffset>& boneParentOffsets);
void Write(std::ostream& stream, const DirectX::XMMATRIX& matrix);
}
