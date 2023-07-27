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
}