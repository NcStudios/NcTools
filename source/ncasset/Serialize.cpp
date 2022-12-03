#include "Serialize.h"
#include "NcaHeader.h"

#include <ostream>

namespace
{
void WriteHeader(std::ostream& stream, const char* magicNumber, size_t assetId, size_t size)
{
    constexpr char defaultAlgo[5] = "NONE"; // not yet supported
    stream.write(magicNumber, 4);
    stream.write(defaultAlgo, 4);
    stream.write(reinterpret_cast<const char*>(&assetId), sizeof(size_t));
    stream.write(reinterpret_cast<const char*>(&size), sizeof(size));
}

template<class T>
void Write(std::ostream& stream, const T& data)
{
    stream.write(reinterpret_cast<const char*>(&data), sizeof(T));
}

template<class T>
void Write(std::ostream& stream, const T* data, size_t size)
{
    stream.write(reinterpret_cast<const char*>(data), size);
}
} // anonymous namespace

namespace nc::asset
{
void SerializeConcaveCollider(std::ostream& stream, const ConcaveCollider& data, size_t assetId)
{
    const auto assetSize = GetBlobSize(data);
    ::WriteHeader(stream, MagicNumber::concaveCollider, assetId, assetSize);
    ::Write(stream, data.extents);
    ::Write(stream, data.maxExtent);
    ::Write(stream, data.triangles.size());
    ::Write(stream, data.triangles.data(), data.triangles.size() * sizeof(Triangle));
}

void SerializeHullCollider(std::ostream& stream, const HullCollider& data, size_t assetId)
{
    const auto assetSize = GetBlobSize(data);
    ::WriteHeader(stream, MagicNumber::hullCollider, assetId, assetSize);
    ::Write(stream, data.extents);
    ::Write(stream, data.maxExtent);
    ::Write(stream, data.vertices.size());
    ::Write(stream, data.vertices.data(), data.vertices.size() * sizeof(Vector3));
}

void SerializeMesh(std::ostream& stream, const Mesh& data, size_t assetId)
{
    const auto assetSize = GetBlobSize(data);
    ::WriteHeader(stream, MagicNumber::mesh, assetId, assetSize);
    ::Write(stream, data.extents);
    ::Write(stream, data.maxExtent);
    ::Write(stream, data.vertices.size());
    ::Write(stream, data.indices.size());
    ::Write(stream, data.vertices.data(), data.vertices.size() * sizeof(Vector3));
    ::Write(stream, data.indices.data(), data.indices.size() * sizeof(uint32_t));
}
} // namespace nc::asset
