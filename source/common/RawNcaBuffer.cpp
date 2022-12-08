#include "RawNcaBuffer.h"

#include "fmt/format.h"
#include "ncutility/NcError.h"

#include <cstring>
#include <istream>

namespace nc::asset
{
RawNcaBuffer::RawNcaBuffer(std::istream& stream, size_t size)
    : m_data(size),
      m_pos{0}
{
    stream.read(m_data.data(), size);
    if (auto bytesRead = stream.gcount(); bytesRead != static_cast<int64_t>(size))
    {
        throw NcError(
            fmt::format("Expected to read {} bytes but only read {}.", size, bytesRead)
        );
    }
}

auto RawNcaBuffer::RemainingByteCount() const noexcept -> size_t
{
    return m_data.size() - m_pos;
}

auto RawNcaBuffer::GetPosition() const noexcept -> size_t
{
    return m_pos;
}

void RawNcaBuffer::SetPosition(size_t position)
{
    if (position > m_data.size())
    {
        throw NcError("Attempt to set RawNcaBuffer position outside buffer.");
    }

    m_pos = position;
}

void RawNcaBuffer::CopyAndAdvance(void* dest, size_t nBytes)
{
    NC_ASSERT(m_pos + nBytes <= m_data.size(), "RawNcaBuffer: attempt to read outside buffer");
    std::memcpy(dest, m_data.data() + m_pos, nBytes);
    m_pos += nBytes;
}
} // namespace nc::asset
