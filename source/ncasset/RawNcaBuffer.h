#pragma once

#include <iosfwd>
#include <type_traits>
#include <vector>

namespace nc::asset
{
/**
 *  @brief POD type requirement for safe deserialization.
 *  @note This must also be recursively applicable to each member of T,
 *        but that is not enforced here.
 */
template<class T>
concept PodType = std::is_trivial_v<T> && std::is_standard_layout_v<T>;

/** @brief Manages an input byte buffer for asset deserialization. */
class RawNcaBuffer
{
    public:
        /**
         * @brief Construct a RawNcaBuffer from a byte stream.
         * 
         * @param stream The stream to read from.
         * @param size The number of bytes to read from the stream.
         * @throw NcError if cannot read 'size' bytes from 'stream'.
         */
        RawNcaBuffer(std::istream& stream, size_t size);

        /** @brief Get the number of bytes left in the buffer available to read. */
        auto RemainingByteCount() const noexcept -> size_t;

        /** @brief Get the current number of bytes read from the buffer so far. */
        auto GetPosition() const noexcept -> size_t;

        /** @brief Change the position in the buffer the next read will start from. */
        void SetPosition(size_t position);

        /**
         * @brief Write bytes to the specified destination, and advance the buffer position.
         * 
         * @tparam PodType Type requirement for the destination object.
         * @param dest The destination bytes will be writtent to.
         * @param nBytes The number of bytes to write. Must be <= RemainingByteCount().
         * @note An array can be written to by setting nBytes = sizeof(PodType) * nElements.
         */
        template<class PodType>
        void Read(PodType* dest, size_t nBytes = sizeof(PodType))
        {
            CopyAndAdvance(static_cast<void*>(dest), nBytes);
        }

    private:
        std::vector<char> m_data;
        size_t m_pos;

        void CopyAndAdvance(void* dest, size_t nBytes);
};
} // namespace nc::asset
