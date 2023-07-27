#include "BonesSerializer.h"

namespace 
{
template<class T>
void Write(std::ostream& stream, const T& data)
{
    stream.write(reinterpret_cast<const char*>(&data), sizeof(T));
}

template<class T>
void Write(std::ostream& stream, const T* data, size_t size)
{
    stream.write(reinterpret_cast<const char*>(data), size);
}
} // anonymous namespace

namespace nc::convert
{
void Write(std::ostream& stream, const nc::asset::BodySpaceNode* parentNode, uint32_t generation)
{
    if (!parentNode)
    {
        return;
    }
    ::Write(stream, generation);
    ::Write(stream, parentNode->boneName);
    ::Write(stream, parentNode->localSpace);
    ::Write(stream, parentNode->parent ? "hasParent" : "isRoot");
    ::Write(stream, parentNode->children.size());

    if (parentNode->children.size() > 0)
    {
        generation++;
    }
    for (auto& child : parentNode->children)
    {
        Write(stream, &child, generation);
    }
}

void Write(std::ostream& stream, const std::unordered_map<std::string, uint32_t>& boneNamesToIds)
{
    for (const auto& pair: boneNamesToIds)
    {
        ::Write(stream, pair.first);
        ::Write(stream, pair.second);
    }
}

void Read(nc::asset::RawNcaBuffer& bytes, std::unordered_map<std::string, uint32_t>* boneNamesToIds, size_t numBones)
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