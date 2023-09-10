#include "BonesReader.h"

namespace nc::asset
{
auto ReadMatrix(RawNcaBuffer& bytes) -> DirectX::XMMATRIX
{
    float buf[16];
    bytes.Read(buf, 16 * sizeof(float));
    return DirectX::XMMATRIX{buf};
}

void Read(RawNcaBuffer& bytes, std::vector<nc::asset::BoneSpaceToParentSpace>* boneSpaceToParentSpaceMatrices, size_t matrixCount)
{
    for (auto i = 0u; i < matrixCount; i++)
    {
        auto numChildren = uint32_t{};
        auto indexOfFirstChild = uint32_t{};
        auto boneSpaceToParentSpace = nc::asset::BoneSpaceToParentSpace{};
        bytes.Read(&boneSpaceToParentSpace.boneName);
        boneSpaceToParentSpace.transformationMatrix = ReadMatrix(bytes);
        bytes.Read(&numChildren);
        boneSpaceToParentSpace.numChildren = numChildren;
        bytes.Read(&indexOfFirstChild);
        boneSpaceToParentSpace.indexOfFirstChild = indexOfFirstChild;
        boneSpaceToParentSpaceMatrices->push_back(std::move(boneSpaceToParentSpace));
    }
}

void Read(RawNcaBuffer& bytes, std::vector<nc::asset::VertexSpaceToBoneSpace>* vertexSpaceToBoneSpaceMatrices, size_t matrixCount)
{
    for (auto i = 0u; i < matrixCount; i++)
    {
        auto vertexSpaceToBoneSpace = nc::asset::VertexSpaceToBoneSpace{};
        bytes.Read(&vertexSpaceToBoneSpace.boneName);
        vertexSpaceToBoneSpace.transformationMatrix = ReadMatrix(bytes);
        vertexSpaceToBoneSpaceMatrices->push_back(std::move(vertexSpaceToBoneSpace));
    }
}
}