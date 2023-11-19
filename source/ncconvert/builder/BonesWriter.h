#pragma once

#include "ncasset/Assets.h"

#include <ostream>

namespace nc::convert
{
/** @brief Write a VertexSpaceToBoneSpace vector to a binary stream.
 * 
 * @param stream The byte stream to write to.
 * @param vertexSpaceToBoneSpaceMatrices A vector of VertexSpaceToBoneSpace objects. 
 * A VertexSpaceToBoneSpace object represents the transformation matrix required to transform the vertex/vertices affected by the named bone into the bone's space.
 */
void Write(std::ostream& stream, const std::vector<nc::asset::VertexSpaceToBoneSpace>& vertexSpaceToBoneSpaceMatrices);

/** @brief Write a BoneSpaceToParentSpace vector to a binary stream.
 * 
 * @param stream The byte stream to write to.
 * @param boneSpaceToParentSpace A vector of BoneSpaceToParentSpace objects. 
* A BoneSpaceToParentSpace object represents the transformation matrix required to transform the named bone's space into it's parent bone's space.
 */
void Write(std::ostream& stream, const std::vector<nc::asset::BoneSpaceToParentSpace>& boneSpaceToParentSpace);

/** @brief Write the map of bone name to index the shader expects to a binary stream.
 * 
 * @param bytes The byte stream to write to.
 */
void Write(std::ostream& stream, const std::unordered_map<std::string, uint32_t>& boneMapping);

/** @brief Write a DirectX::XMMATRIX to a binary stream.
 * 
 * @param bytes The byte stream to write to.
 */
void Write(std::ostream& stream, const DirectX::XMMATRIX& matrix);
} // namespace nc::convert
