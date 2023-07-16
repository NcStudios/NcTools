#include "gtest/gtest.h"
#include "GeometryTestUtility.h"
#include "CollateralGeometry.h"

#include "analysis/GeometryAnalysis.h"
#include "converters/GeometryConverter.h"
#include "ncasset/Assets.h"

#include <algorithm>
#include <iostream>
#include <array>

namespace util
{
void PrintBones(const std::filesystem::path& filePath)
{
    auto uut = nc::convert::GeometryConverter{};
    const auto actual = uut.ImportMesh(filePath);

    for (const auto& vertex : actual.vertices)
    {
        std::cout << "IDs:" << std::endl;
        for (const auto& boneId : vertex.boneIds)
        {
            std::cout << boneId << ", ";
        }
        std::cout << std::endl;

        std::cout << "Weights:" << std::endl;
        std::cout << vertex.boneWeights.x << ", ";
        std::cout << vertex.boneWeights.y << ", ";
        std::cout << vertex.boneWeights.z << ", ";
        std::cout << vertex.boneWeights.w << std::endl;

        std::cout << std::endl;
        std::cout << std::endl;
    }
}

TEST(GeometryConverterTest, ImportConcaveCollider_convertsToNca)
{
    namespace test_data = collateral::plane_fbx;
    auto uut = nc::convert::GeometryConverter{};
    const auto actual = uut.ImportConcaveCollider(test_data::filePath);

    EXPECT_EQ(actual.extents, test_data::meshVertexExtents);
    EXPECT_FLOAT_EQ(actual.maxExtent, test_data::furthestDistanceFromOrigin);
    EXPECT_EQ(actual.triangles.size(), test_data::triangleCount);

    for (const auto& tri : actual.triangles)
    {
        const auto pos = std::ranges::find(test_data::possibleTriangles, tri);
        EXPECT_NE(pos, test_data::possibleTriangles.cend());
    }
}

TEST(GeometryConverterTest, ImportedHullCollider_convertsToNca)
{
    namespace test_data = collateral::cube_fbx;
    auto uut = nc::convert::GeometryConverter{};
    const auto actual = uut.ImportHullCollider(test_data::filePath);

    EXPECT_EQ(actual.extents, test_data::meshVertexExtents);
    EXPECT_FLOAT_EQ(actual.maxExtent, test_data::furthestDistanceFromOrigin);
    EXPECT_EQ(actual.vertices.size(), test_data::vertexCount);

    for (const auto& vertex : actual.vertices)
    {
        const auto pos = std::ranges::find(test_data::possibleVertices, vertex);
        EXPECT_NE(pos, test_data::possibleVertices.cend());
    }
}

TEST(GeometryConverterTest, ImportedMesh_convertsToNca)
{
    namespace test_data = collateral::cube_fbx;
    auto uut = nc::convert::GeometryConverter{};
    const auto actual = uut.ImportMesh(test_data::filePath);

    EXPECT_EQ(actual.extents, test_data::meshVertexExtents);
    EXPECT_FLOAT_EQ(actual.maxExtent, test_data::furthestDistanceFromOrigin);
    EXPECT_EQ(actual.vertices.size(), test_data::vertexCount);

    for (const auto& vertex : actual.vertices)
    {
        // TODO: uvs
        const auto vertexPos = std::ranges::find(test_data::possibleVertices, vertex.position);
        const auto normalPos = std::ranges::find(test_data::possibleNormals, vertex.normal);
        const auto tangentPos = std::ranges::find(test_data::possibleTangents, vertex.tangent);
        const auto bitangentPos = std::ranges::find(test_data::possibleBitangents, vertex.bitangent);
        EXPECT_NE(vertexPos, test_data::possibleVertices.cend());
        EXPECT_NE(normalPos, test_data::possibleNormals.cend());
        EXPECT_NE(tangentPos, test_data::possibleTangents.cend());
        EXPECT_NE(bitangentPos, test_data::possibleBitangents.cend());
    }

    const auto nVertices = actual.vertices.size();
    EXPECT_TRUE(std::ranges::all_of(actual.indices, [&nVertices](auto i){ return i < nVertices; }));
}

TEST(GeometryConverterTest, GetBoneWeights_SingleBone_1WeightAllVertices)
{
    namespace test_data = collateral::single_bone_four_vertex_fbx;
    auto uut = nc::convert::GeometryConverter{};
    const auto actual = uut.ImportMesh(test_data::filePath);

    for (const auto& vertex : actual.vertices)
    {
        EXPECT_EQ(vertex.boneIds[0], 0);
        EXPECT_EQ(vertex.boneIds[1], 0);
        EXPECT_EQ(vertex.boneIds[2], 0);
        EXPECT_EQ(vertex.boneIds[3], 0);
        EXPECT_EQ(vertex.boneWeights.x, 1);
        EXPECT_EQ(vertex.boneWeights.y, -1);
        EXPECT_EQ(vertex.boneWeights.z, -1);
        EXPECT_EQ(vertex.boneWeights.w, -1);
    }
}

TEST(GeometryConverterTest, GetBoneWeights_FourBones_QuarterWeightAllVertices)
{
    namespace test_data = collateral::four_bone_four_vertex_fbx;
    auto uut = nc::convert::GeometryConverter{};
    const auto actual = uut.ImportMesh(test_data::filePath);

    for (const auto& vertex : actual.vertices)
    {
        EXPECT_EQ(vertex.boneIds[0], 0);
        EXPECT_EQ(vertex.boneIds[1], 1);
        EXPECT_EQ(vertex.boneIds[2], 2);
        EXPECT_EQ(vertex.boneIds[3], 3);
        EXPECT_EQ(vertex.boneWeights.x, 0.25);
        EXPECT_EQ(vertex.boneWeights.y, 0.25);
        EXPECT_EQ(vertex.boneWeights.z, 0.25);
        EXPECT_EQ(vertex.boneWeights.w, 0.25);
    }
}

TEST(GeometryConverterTest, GetBoneWeights_FiveBonesPerVertex_ImportFails)
{
    namespace test_data = collateral::four_bone_four_vertex_fbx;
    auto uut = nc::convert::GeometryConverter{};
    const auto actual = uut.ImportMesh(test_data::filePath);

    for (const auto& vertex : actual.vertices)
    {
        EXPECT_EQ(vertex.boneIds[0], 0);
        EXPECT_EQ(vertex.boneIds[1], 1);
        EXPECT_EQ(vertex.boneIds[2], 2);
        EXPECT_EQ(vertex.boneIds[3], 3);
        EXPECT_EQ(vertex.boneWeights.x, 0.25);
        EXPECT_EQ(vertex.boneWeights.y, 0.25);
        EXPECT_EQ(vertex.boneWeights.z, 0.25);
        EXPECT_EQ(vertex.boneWeights.w, 0.25);
    }
}
}