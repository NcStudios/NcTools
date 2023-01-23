#pragma once

#include "ncmath/Geometry.h"

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

struct MeshVertex
{
    Vector3 position = Vector3::Zero();
    Vector3 normal = Vector3::Zero();
    Vector2 uv = Vector2::Zero();
    Vector3 tangent = Vector3::Zero();
    Vector3 bitangent = Vector3::Zero();
};

struct Mesh
{
    Vector3 extents;
    float maxExtent;
    std::vector<MeshVertex> vertices;
    std::vector<uint32_t> indices;
};

struct Shader
{
};

struct Texture
{
    static constexpr uint32_t numChannels = 4u;

    uint32_t width;
    uint32_t height;
    std::vector<unsigned char> pixelData;
};

struct CubeMap
{
    static constexpr uint32_t numChannels = 4u;

    uint32_t faceSideLength;
    std::vector<unsigned char> pixelData;
};
} // namespace nc::asset
