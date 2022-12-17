#include "gtest/gtest.h"
#include "utility/EnumConversion.h"

#include "ncutility/NcError.h"

TEST(EnumConversionTest, ToAssetType_fromString_succeeds)
{
    EXPECT_EQ(nc::convert::ToAssetType("concave-collider"), nc::asset::AssetType::ConcaveCollider);
    EXPECT_EQ(nc::convert::ToAssetType("cube-map"), nc::asset::AssetType::CubeMap);
    EXPECT_EQ(nc::convert::ToAssetType("hull-collider"), nc::asset::AssetType::HullCollider);
    EXPECT_EQ(nc::convert::ToAssetType("mesh"), nc::asset::AssetType::Mesh);
    EXPECT_EQ(nc::convert::ToAssetType("texture"), nc::asset::AssetType::Texture);
}

TEST(EnumConversionTest, ToAssetType_badString_throws)
{
    EXPECT_THROW(nc::convert::ToAssetType("not-an-asset"), nc::NcError);
}

TEST(EnumConversionTest, ToString_fromAssetType_succeeds)
{
    EXPECT_EQ(nc::convert::ToString(nc::asset::AssetType::ConcaveCollider), "concave-collider");
    EXPECT_EQ(nc::convert::ToString(nc::asset::AssetType::CubeMap), "cube-map");
    EXPECT_EQ(nc::convert::ToString(nc::asset::AssetType::HullCollider), "hull-collider");
    EXPECT_EQ(nc::convert::ToString(nc::asset::AssetType::Mesh), "mesh");
    EXPECT_EQ(nc::convert::ToString(nc::asset::AssetType::Texture), "texture");
}

TEST(EnumConversionTest, ToString_badAssetType_throws)
{
    EXPECT_THROW(nc::convert::ToString(static_cast<nc::asset::AssetType>(999)), nc::NcError);
}
