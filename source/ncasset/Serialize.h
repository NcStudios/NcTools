#pragma once

#include "ImportedTypes.h"

#include <iosfwd>

namespace nc::asset
{
void SerializeConcaveCollider(std::ostream& stream, const ConcaveCollider& data, size_t assetId);
void SerializeHullCollider(std::ostream& stream, const HullCollider& data, size_t assetId);
void SerializeMesh(std::ostream& stream, const Mesh& data, size_t assetId);
} // nc::asset
