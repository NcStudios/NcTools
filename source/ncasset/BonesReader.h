#pragma once

#include "ncasset/Assets.h"
#include "RawNcaBuffer.h"

#include <unordered_map>
#include <cstdint>
#include <string>

namespace nc::asset
{
void Read(RawNcaBuffer& bytes, std::unordered_map<std::string, uint32_t>* boneNamesToIds, size_t numBones);
void Read(RawNcaBuffer& bytes, nc::asset::BodySpaceNode* currentNode, nc::asset::BodySpaceNode* parentNode);
auto Read(RawNcaBuffer& bytes) -> DirectX::XMMATRIX;
}