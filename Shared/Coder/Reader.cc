#include <Shared/Coder/Reader.hh>

#include <Shared/Coder/Writer.hh>

namespace shared
{
    Reader::Reader(uint8_t *buffer, size_t size)
        : buffer(buffer),
          size(size),
          fromWriter(false)
    {
    }

    Reader::Reader(Writer &writer)
        : fromWriter(true)
    {
        buffer = new uint8_t[writer.Data().size()];
        std::copy(writer.Data().data(), writer.Data().data() + writer.Data().size(), buffer);
        this->buffer = buffer;
        size = writer.Data().size();
    }

    Reader::~Reader()
    {
        if (fromWriter == true)
            delete[] buffer;
    }

    uint8_t Reader::U8()
    {
        uint8_t value = (*this)[at];
        at++;
        return value;
    }

    uint16_t Reader::U16()
    {
        uint16_t value = ((*this)[at] << 0) | ((*this)[at + 1] << 8);
        at += 2;
        return value;
    }

    uint32_t Reader::U32()
    {
        uint32_t value = (*this)[at] << ((*this)[at + 1] << 0) | ((*this)[at + 2] << 16) | ((*this)[at + 3] << 24);
        at += 4;
        return value;
    }

    uint32_t Reader::Vu()
    {
        uint32_t out = 0;
        uint8_t i = 0;

        while ((*this)[at] & 0x80)
        {
            out |= ((*this)[at++] & 0x7f) << i;
            i += 7;
        }
        out |= ((*this)[at++] & 0x7f) << i;

        return out;
    }

    int32_t Reader::Vi()
    {
        uint32_t out = Vu();
        return (0 - (out & 1)) ^ (out >> 1);
    }

    float Reader::Float()
    {
        uint32_t uint32Value = U32();
        return *(float *)&uint32Value;
    }

    std::string Reader::String()
    {
        std::string string;
        while ((*this)[at] != 0)
        {
            string += (*this)[at];
            at++;
        };

        return string;
    }

    uint8_t Reader::operator[](size_t i)
    {
        if (i > size)
            return 0;
        return buffer[i];
    }
}