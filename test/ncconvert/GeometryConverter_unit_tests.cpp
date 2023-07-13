#include "gtest/gtest.h"
#include "GeometryTestUtility.h"
#include "CollateralGeometry.h"

#include "analysis/GeometryAnalysis.h"
#include "converters/GeometryConverter.h"
#include "ncasset/Assets.h"

#include <algorithm>
#include <array>

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

TEST(GeometryConverterTest, GetBoneWeights_SingleBone_100PercentAllVertices)
{
    namespace test_data = collateral::single_bone_100_weights_fbx;
    auto uut = nc::convert::GeometryConverter{};
    const auto actual = uut.ImportMesh(test_data::filePath);

    //for (const auto& vertex : actual.vertices)
    //{
        // TODO: uvs
        //const auto boneIds = std::ranges::find(test_data::possibleVertices, vertex.boneIds);
        //const auto normalPos = std::ranges::find(test_data::possibleNormals, vertex.normal);
        //const auto tangentPos = std::ranges::find(test_data::possibleTangents, vertex.tangent);
        //const auto bitangentPos = std::ranges::find(test_data::possibleBitangents, vertex.bitangent);
        //EXPECT_NE(vertexPos, test_data::possibleVertices.cend());
        //EXPECT_NE(normalPos, test_data::possibleNormals.cend());
        //EXPECT_NE(tangentPos, test_data::possibleTangents.cend());
        //EXPECT_NE(bitangentPos, test_data::possibleBitangents.cend());
  /*  }

    const auto nVertices = actual.vertices.size();
    EXPECT_TRUE(std::ranges::all_of(actual.indices, [&nVertices](auto i) { return i < nVertices; }));*/
}
