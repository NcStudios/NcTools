#include "BonesReader.h"

namespace nc::asset
{
void Read(RawNcaBuffer& bytes, std::unordered_map<std::string, uint32_t>* boneNamesToIds, size_t numBones)
{
    for (auto i = 0u; i < numBones; i++)
    {
        auto boneNameSize = size_t{};
        auto boneName = std::string{};
        auto boneId = uint32_t{};
        bytes.Read(&boneNameSize);
        boneName.reserve(boneNameSize);

        // As long as not done through a const-qualified overload, you can write to
        // a string like an array through the data() member (but not c_str() because
        // its const!):
        //
        // auto str = std::string(size, '\0');
        // bytes.Read(str.data(), size);

        for (auto j = 0u; j < boneNameSize; j++)
        {
            auto boneNameChar = char{};
            bytes.Read(&boneNameChar);
            boneName.push_back(boneNameChar);
        }

        bytes.Read(&boneId);
        boneNamesToIds->emplace(boneName, boneId);
    }
}

void Read(RawNcaBuffer& bytes, nc::asset::BoneParentOffset* currentNode, nc::asset::BoneParentOffset* parentNode)
{
    auto numChildren = size_t{};
    bytes.Read(&currentNode->boneName); //32 
    currentNode->localSpace = Read(bytes); //64
    currentNode->parent = parentNode;
    bytes.Read(&numChildren);
    currentNode->children.reserve(numChildren);
    for (auto i = 0u; i < numChildren; i++)
    {
        currentNode->children.emplace_back();
        Read(bytes, &currentNode->children.back(), currentNode);
    }
}

auto Read(RawNcaBuffer& bytes) -> DirectX::XMMATRIX
{
    // Can we use XMMATRIX(const float*) ?
    //   float buf[16];
    //   bytes.Read(buf, 16 * sizeof(float));
    //   return DirectX::XMMATRIX{buf};

    float a1 = float{};
    float a2 = float{};
    float a3 = float{};
    float a4 = float{};

    float b1 = float{};
    float b2 = float{};
    float b3 = float{};
    float b4 = float{};

    float c1 = float{};
    float c2 = float{};
    float c3 = float{};
    float c4 = float{};

    float d1 = float{};
    float d2 = float{};
    float d3 = float{};
    float d4 = float{};

    bytes.Read(&a1); bytes.Read(&a2); bytes.Read(&a3); bytes.Read(&a4);
    bytes.Read(&b1); bytes.Read(&b2); bytes.Read(&b3); bytes.Read(&b4);
    bytes.Read(&c1); bytes.Read(&c2); bytes.Read(&c3); bytes.Read(&c4);
    bytes.Read(&d1); bytes.Read(&d2); bytes.Read(&d3); bytes.Read(&d4);

    return DirectX::XMMATRIX
    {
        a1, a2, a3, a4,
        b1, b2, b3, b4,
        c1, c2, c3, c4,
        d1, d2, d3, d4,
    };
}
}