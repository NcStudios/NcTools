#include "gtest/gtest.h"
#include "analysis/GeometryAnalysis.h"
#include "converters/GeometryConverter.h"
#include "ncasset/AssetTypes.h"

#include <algorithm>
#include <array>

const auto collateralDirectory = std::filesystem::path{TEST_COLLATERAL_DIR};

namespace nc
{
auto operator==(const Triangle& lhs, const Triangle& rhs) -> bool
{
    return lhs.a == rhs.a && lhs.b == rhs.b && lhs.c == rhs.c;
}
}

TEST(GeometryConverterTest, ImportConcaveCollider_convertsToNca)
{
    constexpr auto possibleTriangles = std::array<nc::Triangle, 2>{
        nc::Triangle{
            nc::Vector3{-1.0f, -1.0f, 0.0f},
            nc::Vector3{-1.0f,  1.0f, 0.0f},
            nc::Vector3{ 1.0f, -1.0f, 0.0f}
        },
        nc::Triangle{
            nc::Vector3{-1.0f,  1.0f, 0.0f},
            nc::Vector3{ 1.0f,  1.0f, 0.0f},
            nc::Vector3{ 1.0f, -1.0f, 0.0f}
        }
    };

    auto uut = nc::convert::GeometryConverter{};
    const auto actual = uut.ImportConcaveCollider(collateralDirectory / "plane.fbx");

    EXPECT_EQ(actual.extents, nc::convert::GetMeshVertexExtents(possibleTriangles));
    EXPECT_FLOAT_EQ(actual.maxExtent, nc::convert::FindFurthestDistanceFromOrigin(possibleTriangles));
    EXPECT_EQ(actual.triangles.size(), 2);

    for (const auto& tri : actual.triangles)
    {
        EXPECT_NE(std::ranges::find(possibleTriangles, tri), possibleTriangles.cend());
    }
}

TEST(GeometryConverterTest, ImportedHullCollider_convertsToNca)
{
    constexpr auto possibleVertices = std::array<nc::Vector3, 8>{
        nc::Vector3{ 0.5f,  0.5f,  0.5f}, nc::Vector3{-0.5f,  0.5f,  0.5f},
        nc::Vector3{ 0.5f,  0.5f, -0.5f}, nc::Vector3{-0.5f,  0.5f, -0.5f},
        nc::Vector3{ 0.5f, -0.5f,  0.5f}, nc::Vector3{-0.5f, -0.5f,  0.5f},
        nc::Vector3{ 0.5f, -0.5f, -0.5f}, nc::Vector3{-0.5f, -0.5f, -0.5f}
    };

    auto uut = nc::convert::GeometryConverter{};
    const auto actual = uut.ImportHullCollider(collateralDirectory / "cube.fbx");

    EXPECT_EQ(actual.extents, nc::convert::GetMeshVertexExtents(possibleVertices));
    EXPECT_FLOAT_EQ(actual.maxExtent, nc::convert::FindFurthestDistanceFromOrigin(possibleVertices));
    EXPECT_EQ(actual.vertices.size(), 24);

    for (const auto& vertex : actual.vertices)
    {
        EXPECT_NE(std::ranges::find(possibleVertices, vertex), possibleVertices.cend());
    }
}

TEST(GeometryConverterTest, ImportedMesh_convertsToNca)
{
    constexpr auto possibleVertices = std::array<nc::Vector3, 8>{
        nc::Vector3{ 0.5f,  0.5f,  0.5f}, nc::Vector3{-0.5f,  0.5f,  0.5f},
        nc::Vector3{ 0.5f,  0.5f, -0.5f}, nc::Vector3{-0.5f,  0.5f, -0.5f},
        nc::Vector3{ 0.5f, -0.5f,  0.5f}, nc::Vector3{-0.5f, -0.5f,  0.5f},
        nc::Vector3{ 0.5f, -0.5f, -0.5f}, nc::Vector3{-0.5f, -0.5f, -0.5f}
    };

    constexpr auto possibleNormals = std::array<nc::Vector3, 6>{
        nc::Vector3::Left(), nc::Vector3::Right(), nc::Vector3::Up(),
        nc::Vector3::Down(), nc::Vector3::Front(), nc::Vector3::Back()
    };

    constexpr auto possibleTangents = std::array<nc::Vector3, 3>{
        nc::Vector3::Left(), nc::Vector3::Right(), nc::Vector3::Back()
    };

    constexpr auto possibleBitangents = std::array<nc::Vector3, 4>{
        nc::Vector3::Left(), nc::Vector3::Right(), nc::Vector3::Up(),
        nc::Vector3::Down()
    };

    auto uut = nc::convert::GeometryConverter{};
    const auto actual = uut.ImportMesh(collateralDirectory / "cube.fbx");

    EXPECT_EQ(actual.extents, nc::convert::GetMeshVertexExtents(possibleVertices));
    EXPECT_FLOAT_EQ(actual.maxExtent, nc::convert::FindFurthestDistanceFromOrigin(possibleVertices));
    EXPECT_EQ(actual.vertices.size(), 24);

    for (const auto& vertex : actual.vertices)
    {
        EXPECT_NE(std::ranges::find(possibleVertices, vertex.position), possibleVertices.cend());
        EXPECT_NE(std::ranges::find(possibleNormals, vertex.normal), possibleNormals.cend());
        // TODO: uvs
        EXPECT_NE(std::ranges::find(possibleTangents, vertex.tangent), possibleTangents.cend());
        EXPECT_NE(std::ranges::find(possibleBitangents, vertex.bitangent), possibleBitangents.cend());
    }

    const auto nVertices = actual.vertices.size();
    EXPECT_TRUE(std::ranges::all_of(actual.indices, [&nVertices](auto i){ return i < nVertices; }));
}
