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
void SerializeBodySpaceOffsetTree(std::ostream& stream, BodySpaceNode* parentNode)
{
    if (!parentNode)
    {
        return;
    }

    ::Write(stream, parentNode->boneName);
    ::Write(stream, parentNode->localSpace);
    stream << parentNode->boneName << ", Children: " << parentNode->children.size() << std:: endl;

    if (parentNode->children.size() > 0)
    {
        numGenerations++;
    }
    for (auto i = 0u; i < numGenerations; i++)
    {
        std::cout << "    ";
    }

    for (auto& child : parentNode->children)
    {
        SerializeBodySpaceOffsetTree(&child, numGenerations);
    }
}
}