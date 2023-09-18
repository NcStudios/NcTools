#include "DataWriter.h"

namespace nc::convert
{
void WriteHeader(std::ostream& stream, std::string_view magicNumber, size_t assetId, size_t size)
{
    constexpr char defaultAlgo[5] = "NONE"; // not yet supported
    stream.write(magicNumber.data(), 4);
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
} // namespace nc::convert
