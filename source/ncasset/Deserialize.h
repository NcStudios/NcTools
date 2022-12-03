#pragma once

#include "ImportedTypes.h"
#include "NcaHeader.h"

#include <iosfwd>

namespace nc::asset
{
template<class AssetType>
struct DeserializedResult
{
    NcaHeader header;
    AssetType asset;
};

auto DeserializeConcaveCollider(std::istream& stream) -> DeserializedResult<ConcaveCollider>;
auto DeserializeHullCollider(std::istream& stream) -> DeserializedResult<HullCollider>;
auto DeserializeMesh(std::istream& stream) -> DeserializedResult<Mesh>;
} // nc::asset
