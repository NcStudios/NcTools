#include <ostream>

namespace nc::convert
{
void WriteHeader(std::ostream& stream, std::string_view magicNumber, size_t assetId, size_t size);

template<class T>
void Write(std::ostream& stream, const T& data);

template<class T>
void Write(std::ostream& stream, const T* data, size_t size);
} // namespace nc::convert
