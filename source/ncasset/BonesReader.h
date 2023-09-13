#pragma once

#include "ncasset/Assets.h"
#include "RawNcaBuffer.h"

namespace nc::asset
{
    /** @brief Read a VertexSpaceToBoneSpace vector from a RawNcaBuffer byte stream.
     * 
     * @param bytes The byte stream to read from.
     * @param vertexSpaceToBoneSpaceMatrices A vector of VertexSpaceToBoneSpace objects. 
     * A VertexSpaceToBoneSpace object represents the transformation matrix required to transform the vertex/vertices affected by the named bone into the bone's space.
     * @param matrixCount The count of items in the VertexSpaceToBoneSpace vector.
     */
    void Read(RawNcaBuffer& bytes, std::vector<nc::asset::VertexSpaceToBoneSpace>* vertexSpaceToBoneSpaceMatrices, size_t matrixCount);

    /** @brief Read a BoneSpaceToParentSpace vector from a RawNcaBuffer byte stream.
     * 
     * @param bytes The byte stream to read from.
     * @param boneSpaceToParentSpaceMatrices A vector of BoneSpaceToParentSpace objects. 
     * A BoneSpaceToParentSpace object represents the transformation matrix required to transform the named bone's space into it's parent bone's space.
     * @param matrixCount The count of items in the BoneSpaceToParentSpace vector.
     */
    void Read(RawNcaBuffer& bytes, std::vector<nc::asset::BoneSpaceToParentSpace>* boneSpaceToParentSpaceMatrices, size_t matrixCount);

    /** @brief Read a DirectX::XMMATRIX from a RawNcaBuffer byte stream.
     * 
     * @param bytes The byte stream to read from.
     */
    auto ReadMatrix(RawNcaBuffer& bytes) -> DirectX::XMMATRIX;
}