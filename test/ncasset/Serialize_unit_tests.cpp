#include "gtest/gtest.h"
#include "Deserialize.h"
#include "Serialize.h"

#include <algorithm>
#include <sstream>

TEST(SerializationTest, HullCollider_roundTrip_succeeds)
{
    constexpr auto assetId = 1234ull;
    const auto expectedAsset = nc::asset::HullCollider{
        .extents = nc::Vector3{1.2f, 3.4f, 5.6f},
        .maxExtent = 42.42f,
        .vertices = std::vector<nc::Vector3>{
            nc::Vector3::Left(), nc::Vector3::Right(), nc::Vector3::Up(),
            nc::Vector3::Down(), nc::Vector3::Front(), nc::Vector3::Back()
        }
    };

    auto stream = std::stringstream{std::ios::in | std::ios::out | std::ios::binary};
    nc::asset::SerializeHullCollider(stream, expectedAsset, assetId);
    const auto [actualHeader, actualAsset] = nc::asset::DeserializeHullCollider(stream);

    EXPECT_STREQ("HULL", actualHeader.magicNumber);
    EXPECT_EQ(assetId, actualHeader.assetId);
    EXPECT_EQ(nc::asset::GetBlobSize(expectedAsset), actualHeader.size);
    EXPECT_STREQ("NONE", actualHeader.compressionAlgorithm);

    EXPECT_EQ(expectedAsset.extents, actualAsset.extents);
    EXPECT_EQ(expectedAsset.maxExtent, actualAsset.maxExtent);
    ASSERT_EQ(expectedAsset.vertices.size(), actualAsset.vertices.size());
    EXPECT_TRUE(std::equal(expectedAsset.vertices.cbegin(),
                           expectedAsset.vertices.cend(),
                           actualAsset.vertices.cbegin()));
}

TEST(SerializationTest, ConcaveCollider_roundTrip_succeeds)
{
    constexpr auto assetId = 1234ull;
    const auto expectedAsset = nc::asset::ConcaveCollider{
        .extents = nc::Vector3{-1.1f, 2.2f, -3.3f},
        .maxExtent = 123.321f,
        .triangles = std::vector<nc::Triangle>{
            nc::Triangle{nc::Vector3::Splat(1), nc::Vector3::Splat(2), nc::Vector3::Splat(3)},
            nc::Triangle{nc::Vector3::Splat(4), nc::Vector3::Splat(5), nc::Vector3::Splat(6)}
        }
    };

    auto stream = std::stringstream{std::ios::in | std::ios::out | std::ios::binary};
    nc::asset::SerializeConcaveCollider(stream, expectedAsset, assetId);
    const auto [actualHeader, actualAsset] = nc::asset::DeserializeConcaveCollider(stream);

    EXPECT_STREQ("CONC", actualHeader.magicNumber);
    EXPECT_EQ(assetId, actualHeader.assetId);
    EXPECT_EQ(nc::asset::GetBlobSize(expectedAsset), actualHeader.size);
    EXPECT_STREQ("NONE", actualHeader.compressionAlgorithm);

    EXPECT_EQ(expectedAsset.extents, actualAsset.extents);
    EXPECT_EQ(expectedAsset.maxExtent, actualAsset.maxExtent);
    ASSERT_EQ(expectedAsset.triangles.size(), actualAsset.triangles.size());

    for(auto i = 0u; i < expectedAsset.triangles.size(); ++i)
    {
        const auto& expected = expectedAsset.triangles[i];
        const auto& actual = actualAsset.triangles[i];
        EXPECT_EQ(expected.a, actual.a);
        EXPECT_EQ(expected.b, actual.b);
        EXPECT_EQ(expected.c, actual.c);
    }
}

TEST(SerializationTest, Mesh_roundTrip_succeeds)
{
    constexpr auto assetId = 1234ull;
    const auto expectedAsset = nc::asset::Mesh{
        .extents = nc::Vector3{-5.0f, 4.22f, 10.010101f},
        .maxExtent = 10.010101f,
        .vertices = std::vector<nc::Vector3>{
            nc::Vector3::Splat(-4.0f), nc::Vector3::Splat(5.0f), nc::Vector3::Splat(15.3f)
        },
        .indices = std::vector<uint32_t>{
            0, 1, 2,  1, 2, 0,  2, 0, 1
        }
    };

    auto stream = std::stringstream{std::ios::in | std::ios::out | std::ios::binary};
    nc::asset::SerializeMesh(stream, expectedAsset, assetId);
    const auto [actualHeader, actualAsset] = nc::asset::DeserializeMesh(stream);

    EXPECT_STREQ("MESH", actualHeader.magicNumber);
    EXPECT_EQ(assetId, actualHeader.assetId);
    EXPECT_EQ(nc::asset::GetBlobSize(expectedAsset), actualHeader.size);
    EXPECT_STREQ("NONE", actualHeader.compressionAlgorithm);

    EXPECT_EQ(expectedAsset.extents, actualAsset.extents);
    EXPECT_EQ(expectedAsset.maxExtent, actualAsset.maxExtent);
    ASSERT_EQ(expectedAsset.vertices.size(), actualAsset.vertices.size());
    ASSERT_EQ(expectedAsset.indices.size(), actualAsset.indices.size());
    EXPECT_TRUE(std::equal(expectedAsset.vertices.cbegin(),
                           expectedAsset.vertices.cend(),
                           actualAsset.vertices.cbegin()));
    EXPECT_TRUE(std::equal(expectedAsset.indices.cbegin(),
                           expectedAsset.indices.cend(),
                           actualAsset.indices.cbegin()));
}

int main(int argc, char ** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
