#include "Deserialize.h"
#include "ImportedTypes.h"
#include "RawNcaBuffer.h"

#include "ncutility/NcError.h"
#include "fmt/format.h"

#include <cstring>
#include <istream>
#include <vector>

namespace
{
auto ReadNcaHeader(std::istream& stream, const char* expectedMagicNumber) -> nc::asset::NcaHeader
{
    auto bytes = nc::asset::RawNcaBuffer{stream, nc::asset::NcaHeader::binarySize};
    auto header = nc::asset::NcaHeader{};
    bytes.WriteTo(&header.magicNumber, 4); // not null-terminated in file

    // Verify we're reading the expected type of data
    if (strcmp(header.magicNumber, expectedMagicNumber))
    {
        throw nc::NcError(fmt::format(
            "Magic number mismatch actual: '{}' expected: '{}' ",
             header.magicNumber, expectedMagicNumber)
        );
    }

    bytes.WriteTo(&header.compressionAlgorithm, 4); // not null-terminated in file
    bytes.WriteTo(&header.assetId);
    bytes.WriteTo(&header.size);
    return header;
}
} // anonymous namespace

namespace nc::asset
{
auto DeserializeConcaveCollider(std::istream& stream) -> DeserializedResult<ConcaveCollider>
{
    const auto header = ::ReadNcaHeader(stream, MagicNumber::concaveCollider);
    auto bytes = RawNcaBuffer{stream, header.size};
    auto asset = ConcaveCollider{};
    auto triCount = size_t{};
    bytes.WriteTo(&asset.extents);
    bytes.WriteTo(&asset.maxExtent);
    bytes.WriteTo(&triCount);
    asset.triangles.resize(triCount);
    bytes.WriteTo(asset.triangles.data(), triCount * sizeof(Triangle));

    if (bytes.RemainingByteCount() != 0)
    {
        throw NcError("Not all data was read from RawNcaBuffer");
    }

    return {header, asset};
}

auto DeserializeHullCollider(std::istream& stream) -> DeserializedResult<HullCollider>
{
    const auto header = ::ReadNcaHeader(stream, MagicNumber::hullCollider);
    auto bytes = RawNcaBuffer{stream, header.size};
    auto asset = HullCollider{};
    auto vertexCount = size_t{};
    bytes.WriteTo(&asset.extents);
    bytes.WriteTo(&asset.maxExtent);
    bytes.WriteTo(&vertexCount);
    asset.vertices.resize(vertexCount);
    bytes.WriteTo(asset.vertices.data(), vertexCount * sizeof(Vector3));

    if (bytes.RemainingByteCount() != 0)
    {
        throw NcError("Not all data was read from RawNcaBuffer");
    }

    return {header, asset};
}

auto DeserializeMesh(std::istream& stream) -> DeserializedResult<Mesh>
{
    const auto header = ::ReadNcaHeader(stream, MagicNumber::mesh);
    auto bytes = RawNcaBuffer{stream, header.size};
    auto asset = Mesh{};
    auto vertexCount = size_t{};
    auto indexCount = size_t{};
    bytes.WriteTo(&asset.extents);
    bytes.WriteTo(&asset.maxExtent);
    bytes.WriteTo(&vertexCount);
    bytes.WriteTo(&indexCount);
    asset.vertices.resize(vertexCount);
    bytes.WriteTo(asset.vertices.data(), vertexCount * sizeof(Vector3));
    asset.indices.resize(indexCount);
    bytes.WriteTo(asset.indices.data(), indexCount * sizeof(uint32_t));

    if (bytes.RemainingByteCount() != 0)
    {
        throw NcError("Not all data was read from RawNcaBuffer");
    }

    return {header, asset};
}
} // namespace nc::asset
