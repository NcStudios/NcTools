#include "BonesWriter.h"
#include "builder/DataWriter.h"

#include <cstring>

namespace nc::convert
{
void Write(std::ostream& stream, const std::vector<nc::asset::VertexSpaceToBoneSpace>& vertexSpaceToBoneSpaceMatrices)
{
    for (const auto& toBoneSpace : vertexSpaceToBoneSpaceMatrices)
    {
        Write(stream, toBoneSpace.boneName.size());
        Write(stream, toBoneSpace.boneName.data(), toBoneSpace.boneName.size());
        Write(stream, toBoneSpace.transformationMatrix);
    }
}

void Write(std::ostream& stream, const std::vector<nc::asset::BoneSpaceToParentSpace>& boneSpaceToParentSpaceMatrices)
{
    for (const auto& toParentSpace : boneSpaceToParentSpaceMatrices)
    {
        Write(stream, toParentSpace.boneName.size());
        Write(stream, toParentSpace.boneName.data(), toParentSpace.boneName.size());
        Write(stream, toParentSpace.transformationMatrix);
        Write(stream, toParentSpace.numChildren);
        Write(stream, toParentSpace.indexOfFirstChild);
    }
}

void Write(std::ostream& stream, const std::unordered_map<std::string, uint32_t>& boneMapping)
{
    for (const auto& [boneName, index] : boneMapping)
    {
        Write(stream, boneName.size());
        Write(stream, boneName.data(), boneName.size());
        Write(stream, index);
    }
}

void Write(std::ostream& stream, const DirectX::XMMATRIX& matrix)
{
    DirectX::XMFLOAT4X4 view;
    XMStoreFloat4x4(&view, matrix);
    Write(stream, view.m, sizeof(float)*16);
}
} // namespace nc::convert
