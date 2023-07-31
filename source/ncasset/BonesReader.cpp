#include "BonesReader.h"

namespace nc::asset
{
void Read(RawNcaBuffer& bytes, std::unordered_map<std::string, uint32_t>* boneNamesToIds, size_t numBones)
{
    for (auto i = 0u; i < numBones; i++)
    {
        auto boneName = std::string{};
        auto boneId = size_t{};
        bytes.Read(&boneName);
        bytes.Read(&boneId);
        boneNamesToIds->emplace(boneName, boneId);
    }
}

void Read(RawNcaBuffer& bytes, nc::asset::BodySpaceNode* parentNode)
{
    auto generation = size_t{};
    auto isRoot = false;
    auto parentStatus = std::string{};
    auto numChildren = size_t{};
    bytes.Read(&generation);
    bytes.Read(&parentNode->boneName);
    //ReadMatrix(&parentNode->localSpace);
    bytes.Read(&parentStatus);
    isRoot = parentStatus == "hasParent" ? false : true;
    parentNode->parent = parentNode; //@todo Wrong
    bytes.Read(&numChildren);
    parentNode->children.reserve(numChildren);
    for (auto i = 0u; i < numChildren; i++)
    {
        parentNode->children.emplace_back();
        Read(bytes, &parentNode->children.back());
    }
}
}