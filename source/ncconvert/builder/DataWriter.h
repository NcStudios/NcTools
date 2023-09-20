#include <ostream>

namespace nc::convert
{
/** @brief Write the asset header into the given stream.
 *
 * @param stream The byte stream to write to.
 * @param magicNumber The string_view used to reperesent the asset type. See NcaHeader.h
 * @param assetId The unique ID for the asset.
 * @param size The size of the asset.
 */
inline void WriteHeader(std::ostream& stream, std::string_view magicNumber, size_t assetId, size_t size)
{
    constexpr char defaultAlgo[5] = "NONE"; // not yet supported
    stream.write(magicNumber.data(), 4);
    stream.write(defaultAlgo, 4);
    stream.write(reinterpret_cast<const char*>(&assetId), sizeof(size_t));
    stream.write(reinterpret_cast<const char*>(&size), sizeof(size));
}

/** @brief Write the data into the stream.
 *
 * @param stream The byte stream to read to.
 * @param data The data to write.
 */
template<class T>
void Write(std::ostream& stream, const T& data)
{
    stream.write(reinterpret_cast<const char*>(&data), sizeof(T));
}

/** @brief Write the data into the stream. Used for vectors/arrays.
 *
 * @param stream The byte stream to read to.
 * @param data The data to write.
 * @param size The size of the data to write.
 */
template<class T>
void Write(std::ostream& stream, const T* data, size_t size)
{
    stream.write(reinterpret_cast<const char*>(data), size);
}
} // namespace nc::convert
