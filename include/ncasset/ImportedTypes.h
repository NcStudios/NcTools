#pragma once

#include "ncmath/Geometry.h"

#include <array>
#include <vector>

namespace nc::asset
{
struct AudioClip
{
    size_t samplesPerChannel;
    std::vector<double> leftChannel;
    std::vector<double> rightChannel;
};

struct HullCollider
{
    Vector3 extents;
    float maxExtent;
    std::vector<Vector3> vertices;
};

struct ConcaveCollider
{
    Vector3 extents;
    float maxExtent;
    std::vector<Triangle> triangles;
};

struct Mesh
{
    Vector3 extents;
    float maxExtent;
    std::vector<Vector3> vertices;
    std::vector<uint32_t> indices;
};

struct Shader
{
};

struct Texture
{
    uint32_t width;
    uint32_t height;
    std::vector<unsigned char> pixels;
};

struct Cubemap
{
    uint32_t size;
    std::array<Texture, 6> pixelArray;
};
} // namespace nc::asset
