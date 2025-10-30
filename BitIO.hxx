
#ifndef __BITIO_HXX__
#define __BITIO_HXX__
#include <cstdint>
#include <vector>
#include <istream>
#include <ostream>
struct BitWriter
{
    std::vector<std::uint8_t> buffer;
    std::uint8_t acc{0};
    int bits{0};
    void put_bit(bool b)
    {
        acc = (acc << 1) | (b ? 1 : 0);
        bits++;
        if (bits == 8)
        {
            buffer.push_back(acc);
            acc = 0;
            bits = 0;
        }
    }
    void put_bits(const std::vector<bool> &v)
    {
        for (bool b : v)
            put_bit(b);
    }
    void flush_byte_align()
    {
        while (bits != 0)
            put_bit(false);
    }
    void write_to(std::ostream &os) const { os.write(reinterpret_cast<const char *>(buffer.data()), (std::streamsize)buffer.size()); }
};
struct BitReader
{
    std::istream &is;
    std::uint8_t cur{0};
    int left{0};
    BitReader(std::istream &in) : is(in) {}
    bool get_bit()
    {
        if (left == 0)
        {
            if (!is.read(reinterpret_cast<char *>(&cur), 1))
                throw std::runtime_error("EOF bits");
            left = 8;
        }
        bool b = (cur & 0x80) != 0;
        cur <<= 1;
        left--;
        return b;
    }
    void discard_to_byte() { left = 0; }
};
template <typename T>
inline void write_le(std::ostream &os, T v)
{
    static_assert(std::is_integral<T>::value, "T integral");
    for (size_t i = 0; i < sizeof(T); ++i)
    {
        std::uint8_t b = (std::uint8_t)(((std::uint64_t)v >> (8 * i)) & 0xFF);
        os.write(reinterpret_cast<const char *>(&b), 1);
    }
}
template <typename T>
inline T read_le(std::istream &is)
{
    static_assert(std::is_integral<T>::value, "T integral");
    std::uint64_t v = 0;
    for (size_t i = 0; i < sizeof(T); ++i)
    {
        std::uint8_t b = 0;
        if (!is.read(reinterpret_cast<char *>(&b), 1))
            throw std::runtime_error("EOF int");
        v |= ((std::uint64_t)b) << (8 * i);
    }
    return (T)v;
}
#endif
