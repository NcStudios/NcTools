#include "BonesReader.h"

namespace nc::asset
{
auto ReadMatrix(RawNcaBuffer& bytes) -> DirectX::XMMATRIX
{
    float buf[16];
    bytes.Read(buf, 16 * sizeof(float));
    return DirectX::XMMATRIX{buf};
}

auto ReadVertexToBoneMatrices(RawNcaBuffer& bytes, size_t matrixCount) -> std::vector<nc::asset::VertexSpaceToBoneSpace>
{
    auto vertexSpaceToBoneSpaceMatrices = std::vector<nc::asset::VertexSpaceToBoneSpace>{};
    vertexSpaceToBoneSpaceMatrices.reserve(matrixCount);
    for (auto i = 0u; i < matrixCount; i++)
    {
        auto boneNameSize = size_t{};
        bytes.Read(&boneNameSize);
        auto vertexSpaceToBoneSpace = nc::asset::VertexSpaceToBoneSpace{};
        vertexSpaceToBoneSpace.boneName.resize(boneNameSize);
        bytes.Read(vertexSpaceToBoneSpace.boneName.data(), boneNameSize);
        vertexSpaceToBoneSpace.transformationMatrix = ReadMatrix(bytes);
        vertexSpaceToBoneSpaceMatrices.push_back(std::move(vertexSpaceToBoneSpace));
    }
    return vertexSpaceToBoneSpaceMatrices;
}

auto ReadBoneToParentMatrices(RawNcaBuffer& bytes, size_t matrixCount) -> std::vector<nc::asset::BoneSpaceToParentSpace>
{
    auto boneSpaceToParentSpaceMatrices = std::vector<nc::asset::BoneSpaceToParentSpace>{};
    boneSpaceToParentSpaceMatrices.reserve(matrixCount);
    for (auto i = 0u; i < matrixCount; i++)
    {
        auto numChildren = uint32_t{};
        auto indexOfFirstChild = uint32_t{};
        auto boneNameSize = size_t{};
        bytes.Read(&boneNameSize);
        auto boneSpaceToParentSpace = nc::asset::BoneSpaceToParentSpace{};
        boneSpaceToParentSpace.boneName.resize(boneNameSize);
        bytes.Read(boneSpaceToParentSpace.boneName.data(), boneNameSize);
        boneSpaceToParentSpace.transformationMatrix = ReadMatrix(bytes);
        bytes.Read(&numChildren);
        boneSpaceToParentSpace.numChildren = numChildren;
        bytes.Read(&indexOfFirstChild);
        boneSpaceToParentSpace.indexOfFirstChild = indexOfFirstChild;
        boneSpaceToParentSpaceMatrices.push_back(std::move(boneSpaceToParentSpace));
    }
    return boneSpaceToParentSpaceMatrices;
}
} // namespace nc::asset
