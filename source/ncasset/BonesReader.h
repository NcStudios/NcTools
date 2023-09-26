#pragma once

#include "ncasset/Assets.h"
#include "RawNcaBuffer.h"

namespace nc::asset
{
    /** @brief Read a VertexSpaceToBoneSpace vector from a RawNcaBuffer byte stream.
     * A VertexSpaceToBoneSpace object represents the transformation matrix required to transform the vertex/vertices affected by the named bone into the bone's space.
     * 
     * @param bytes The byte stream to read from.
     * @param matrixCount The count of items in the VertexSpaceToBoneSpace vector.
     * @returns A VertexSpaceToBoneSpace vector
     */
    auto ReadVertexToBoneMatrices(RawNcaBuffer& bytes, size_t matrixCount) -> std::vector<nc::asset::VertexSpaceToBoneSpace>;

    /** @brief Read a BoneSpaceToParentSpace vector from a RawNcaBuffer byte stream.
     * A BoneSpaceToParentSpace object represents the transformation matrix required to transform the named bone's space into it's parent bone's space.
     * 
     * @param bytes The byte stream to read from.
     * @param matrixCount The count of items in the BoneSpaceToParentSpace vector.
     * @returns A vector of BoneSpaceToParentSpace objects. 
     */
    auto ReadBoneToParentMatrices(RawNcaBuffer& bytes, size_t matrixCount) -> std::vector<nc::asset::BoneSpaceToParentSpace>;

    /** @brief Read a DirectX::XMMATRIX from a RawNcaBuffer byte stream.
     * 
     * @param bytes The byte stream to read from.
     */
    auto ReadMatrix(RawNcaBuffer& bytes) -> DirectX::XMMATRIX;
} // namespace nc::asset
