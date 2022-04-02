#pragma once

#include "Common.h"

auto GetMeshVertexExtents(const aiVector3D* data, unsigned count) -> MeshExtents;
auto GetMaximumVertexInDirection(const aiVector3D* data, unsigned count, aiVector3D direction) -> aiVector3D;