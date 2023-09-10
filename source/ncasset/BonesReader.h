#pragma once

#include "ncasset/Assets.h"
#include "RawNcaBuffer.h"

#include <unordered_map>
#include <cstdint>
#include <string>

namespace nc::asset
{
void Read(RawNcaBuffer& bytes, std::vector<nc::asset::BoneSpaceToParentSpace>* boneSpaceToParentSpaceMatrices, size_t matrixCount);
void Read(RawNcaBuffer& bytes, std::vector<nc::asset::VertexSpaceToBoneSpace>* vertexSpaceToBoneSpaceMatrices, size_t matrixCount);
auto ReadMatrix(RawNcaBuffer& bytes) -> DirectX::XMMATRIX;
}