#include "MeshAnalysis.h"

auto GetMaximumVertexInDirection(const aiVector3D* data, unsigned count, aiVector3D direction) -> aiVector3D
{
    if(count == 0u)
        throw std::runtime_error("Vertex count in hull collider is zero");
    
    unsigned maxIndex = 0u;
    float maxDot = data[0] * direction;

    for(unsigned i = 1u; i < count; ++i)
    {
        float dot = data[i] * direction;
        if(dot > maxDot)
        {
            maxDot = dot;
            maxIndex = i;
        }
    }

    return data[maxIndex];
}

auto GetMeshVertexExtents(const aiVector3D* data, unsigned count) -> MeshExtents
{
    auto minX = GetMaximumVertexInDirection(data, count, aiVector3D{ -1,  0,  0});
    auto maxX = GetMaximumVertexInDirection(data, count, aiVector3D{  1,  0,  0});
    auto minY = GetMaximumVertexInDirection(data, count, aiVector3D{  0, -1,  0});
    auto maxY = GetMaximumVertexInDirection(data, count, aiVector3D{  0,  1,  0});
    auto minZ = GetMaximumVertexInDirection(data, count, aiVector3D{  0,  0, -1});
    auto maxZ = GetMaximumVertexInDirection(data, count, aiVector3D{  0,  0,  1});

    if(minX.x > 0.0f || maxX.x < 0.0f || minY.y > 0.0f || maxY.y < 0.0f || minZ.z > 0.0f || maxZ.z < 0.0f)
        std::cerr << "Warning: Hull collider mesh does not contain the origin. The origin is assumed to be the center of mass.\n";

    std::array<float, 6u> squareMagnitudes
    {
        minX.SquareLength(), maxX.SquareLength(), minY.SquareLength(), maxY.SquareLength(), minZ.SquareLength(), maxZ.SquareLength()
    };

    float maxSquareMagnitude = squareMagnitudes[0];
    for(unsigned i = 1u; i < 6u; ++i)
    {
        if(squareMagnitudes[i] > maxSquareMagnitude)
            maxSquareMagnitude = squareMagnitudes[i];
    }

    return MeshExtents
    {
        .x = maxX.x - minX.x,
        .y = maxY.y - minY.y,
        .z = maxZ.z - minZ.z,
        .max = sqrtf(maxSquareMagnitude)
    };
}