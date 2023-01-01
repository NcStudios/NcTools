#include "gtest/gtest.h"
#include "Deserialize.h"
#include "Serialize.h"

#include <algorithm>
#include <sstream>

namespace nc::asset
{
bool operator==(const nc::asset::MeshVertex& lhs, const nc::asset::MeshVertex& rhs)
{
    return lhs.position == rhs.position &&
           lhs.normal == rhs.normal &&
           lhs.uv == rhs.uv &&
           lhs.tangent == rhs.tangent &&
           lhs.bitangent == rhs.bitangent;
}
}

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
    nc::asset::Serialize(stream, expectedAsset, assetId);
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
    nc::asset::Serialize(stream, expectedAsset, assetId);
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
        .vertices = std::vector<nc::asset::MeshVertex>{
            nc::asset::MeshVertex{nc::Vector3::Splat(0.0f),
                              nc::Vector3::Splat(1.0f),
                              nc::Vector2::Splat(2.0f),
                              nc::Vector3::Splat(3.0f),
                              nc::Vector3::Splat(4.0f)},
            nc::asset::MeshVertex{nc::Vector3::Splat(5.0f),
                              nc::Vector3::Splat(6.0f),
                              nc::Vector2::Splat(7.0f),
                              nc::Vector3::Splat(9.0f),
                              nc::Vector3::Splat(9.0f)},
            nc::asset::MeshVertex{nc::Vector3::Splat(10.0f),
                              nc::Vector3::Splat(11.0f),
                              nc::Vector2::Splat(12.0f),
                              nc::Vector3::Splat(13.0f),
                              nc::Vector3::Splat(14.0f)}
        },
        .indices = std::vector<uint32_t>{
            0, 1, 2,  1, 2, 0,  2, 0, 1
        }
    };

    auto stream = std::stringstream{std::ios::in | std::ios::out | std::ios::binary};
    nc::asset::Serialize(stream, expectedAsset, assetId);
    const auto [actualHeader, actualAsset] = nc::asset::DeserializeMesh(stream);

    EXPECT_STREQ("MESH", actualHeader.magicNumber);
    EXPECT_EQ(assetId, actualHeader.assetId);
    EXPECT_EQ(nc::asset::GetBlobSize(expectedAsset), actualHeader.size);
    EXPECT_STREQ("NONE", actualHeader.compressionAlgorithm);

    EXPECT_EQ(expectedAsset.extents, actualAsset.extents);
    EXPECT_EQ(expectedAsset.maxExtent, actualAsset.maxExtent);
    ASSERT_EQ(expectedAsset.vertices.size(), actualAsset.vertices.size());
    ASSERT_EQ(expectedAsset.indices.size(), actualAsset.indices.size());

    for(auto i = 0u; i < expectedAsset.vertices.size(); ++i)
    {
        const auto& e = expectedAsset.vertices[i];
        const auto& a = actualAsset.vertices[i];
        EXPECT_EQ(e, a);
    }

    EXPECT_TRUE(std::equal(expectedAsset.indices.cbegin(),
                           expectedAsset.indices.cend(),
                           actualAsset.indices.cbegin()));
}

TEST(SerializationTest, Texture_roundTrip_succeeds)
{
    constexpr auto assetId = 1234ull;
    const auto expectedAsset = nc::asset::Texture{
        .width = 2, .height = 2,
        .pixelData = std::vector<unsigned char>{
            0xA1, 0xA2, 0xA3, 0xA4,  0xB5, 0xB6, 0xB7, 0xB8,
            0xCA, 0xCB, 0xCC, 0xCD,  0xD0, 0xD1, 0xD2, 0xD3
        }
    };

    auto stream = std::stringstream{std::ios::in | std::ios::out | std::ios::binary};
    nc::asset::Serialize(stream, expectedAsset, assetId);
    const auto [actualHeader, actualAsset] = nc::asset::DeserializeTexture(stream);

    EXPECT_STREQ("TEXT", actualHeader.magicNumber);
    EXPECT_EQ(assetId, actualHeader.assetId);
    EXPECT_EQ(nc::asset::GetBlobSize(expectedAsset), actualHeader.size);
    EXPECT_STREQ("NONE", actualHeader.compressionAlgorithm);

    EXPECT_EQ(expectedAsset.width, actualAsset.width);
    EXPECT_EQ(expectedAsset.height, actualAsset.height);
    ASSERT_EQ(expectedAsset.pixelData.size(), actualAsset.pixelData.size());

    EXPECT_TRUE(std::equal(expectedAsset.pixelData.cbegin(),
                           expectedAsset.pixelData.cend(),
                           actualAsset.pixelData.cbegin()));
}

TEST(SerializationTest, AudioClip_roundTrip_succeeds)
{
    constexpr auto assetId = 1234ull;
    const auto expectedAsset = nc::asset::AudioClip{
        .samplesPerChannel = 4ull,
        .leftChannel = std::vector<double>{0.0f, 0.5f, 1.0f, 0.5f},
        .rightChannel = std::vector<double>{0.0f, 0.25f, 0.75f, 1.0f}
    };

    auto stream = std::stringstream{std::ios::in | std::ios::out | std::ios::binary};
    nc::asset::Serialize(stream, expectedAsset, assetId);
    const auto [actualHeader, actualAsset] = nc::asset::DeserializeAudioClip(stream);

    EXPECT_STREQ("CLIP", actualHeader.magicNumber);
    EXPECT_EQ(assetId, actualHeader.assetId);
    EXPECT_EQ(nc::asset::GetBlobSize(expectedAsset), actualHeader.size);
    EXPECT_STREQ("NONE", actualHeader.compressionAlgorithm);

    EXPECT_EQ(expectedAsset.samplesPerChannel, actualAsset.samplesPerChannel);
    ASSERT_EQ(expectedAsset.leftChannel.size(), actualAsset.leftChannel.size());
    ASSERT_EQ(expectedAsset.rightChannel.size(), actualAsset.rightChannel.size());
    EXPECT_TRUE(std::equal(expectedAsset.leftChannel.cbegin(),
                           expectedAsset.leftChannel.cend(),
                           actualAsset.leftChannel.cbegin()));
    EXPECT_TRUE(std::equal(expectedAsset.rightChannel.cbegin(),
                           expectedAsset.rightChannel.cend(),
                           actualAsset.rightChannel.cbegin()));
}

TEST(SerializationTest, CubeMap_roundTrip_succeeds)
{
    constexpr auto assetId = 1234ull;
    const auto expectedAsset = nc::asset::CubeMap{
        .faceSideLength = 1,
        .pixelData = std::vector<unsigned char>{
            0xA1, 0xA2, 0xA3, 0xA4, // front
            0xB1, 0xB2, 0xB3, 0xB4, // back
            0xC1, 0xC2, 0xC3, 0xC4, // up
            0xD1, 0xD2, 0xD3, 0xD4, // down
            0xE1, 0xE2, 0xE3, 0xE4, // right
            0xF1, 0xF2, 0xF3, 0xF4,  // left
        }
    };

    auto stream = std::stringstream{std::ios::in | std::ios::out | std::ios::binary};
    nc::asset::Serialize(stream, expectedAsset, assetId);
    const auto [actualHeader, actualAsset] = nc::asset::DeserializeCubeMap(stream);

    EXPECT_STREQ("CUBE", actualHeader.magicNumber);
    EXPECT_EQ(assetId, actualHeader.assetId);
    EXPECT_EQ(nc::asset::GetBlobSize(expectedAsset), actualHeader.size);
    EXPECT_STREQ("NONE", actualHeader.compressionAlgorithm);

    EXPECT_EQ(expectedAsset.faceSideLength, actualAsset.faceSideLength);
    ASSERT_EQ(expectedAsset.pixelData.size(), actualAsset.pixelData.size());

    EXPECT_TRUE(std::equal(expectedAsset.pixelData.cbegin(),
                           expectedAsset.pixelData.cend(),
                           actualAsset.pixelData.cbegin()));
}
