#pragma once

#include "Common.h"

struct VertexStream
{
    aiMesh* mesh = nullptr;
    unsigned index = 0u;
};

std::ostream& operator<<(std::ostream& os, const NcaHeader& header);
std::ostream& operator<<(std::ostream& os, const aiVector3D& vec);
std::ostream& operator<<(std::ostream& os, const MeshExtents& extents);
std::ostream& operator<<(std::ostream& os, const VertexStream& vertex);