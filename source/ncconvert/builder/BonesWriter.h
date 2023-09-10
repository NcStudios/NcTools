#pragma once

#include "ncasset/Assets.h"

#include <ostream>

namespace nc::convert
{
void Write(std::ostream& stream, const std::vector<nc::asset::VertexSpaceToBoneSpace>& vertexSpaceToBoneSpaceMatrices);
void Write(std::ostream& stream, const std::vector<nc::asset::BoneSpaceToParentSpace>& boneSpaceToParentSpace);
void Write(std::ostream& stream, const DirectX::XMMATRIX& matrix);
}
