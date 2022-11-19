#ifndef D5628A2C_0FB5_43F0_B7AA_59BCC584DFBD
#define D5628A2C_0FB5_43F0_B7AA_59BCC584DFBD

#include <algorithm>
#include <bit>
#include <exception>
#include <fstream>
#include <string>
#include <vector>
#include <iterator>
#include <ranges>

template <class Int, class It>
Int be_int(It beg, It end)
{
    Int ret = 0;
    if (end - beg != sizeof(ret))
    {
        throw std::runtime_error("range received not of length " + std::to_string(sizeof(ret)));
    }
    for (auto i = beg; i != end; ++i)
    {
        ret <<= 8;
        ret += *i;
    }
    return ret;
}

template <typename T>
auto direct_copy_bytes(T x)
{
    using std::copy;
    std::array<std::byte, sizeof (T)> bytes;
    auto start = reinterpret_cast<std::byte*>(&x);
    copy(start, start + sizeof x, bytes.data());
    return bytes;
}

template <typename T>
auto reverse_copy_bytes(T x)
{
    using std::reverse_copy;
    std::array<std::byte, sizeof (T)> bytes;
    auto start = reinterpret_cast<std::byte*>(&x);
    reverse_copy(start, start + sizeof x, bytes.data());
    return bytes;
}

template <typename Int>
auto to_big_endian_array(Int x)
{
    using std::endian;
    return endian::native == endian::big ? direct_copy_bytes(x) : reverse_copy_bytes(x);
}

template <typename Int>
auto to_little_endian_array(Int x)
{
    using std::endian;
    return endian::native == endian::little ? direct_copy_bytes(x) : reverse_copy_bytes(x);
}

template <typename Int, std::output_iterator<std::byte> ItOut>
auto to_little_endian(Int x, ItOut beg)
{
    auto bytes = to_little_endian_array(x);
    return std::ranges::copy(bytes, beg);
}

template <typename Int, std::output_iterator<std::byte> ItOut>
auto to_big_endian(Int x, ItOut beg)
{
    auto bytes =  to_big_endian_array(x);
    return std::ranges::copy(bytes, beg);
}

template <typename To, typename ByteIter>
To from_little_endian(ByteIter beg)
{
    using std::endian, std::byte;
    To converted;
    if (endian::native == endian::little)
    {
        copy(beg, beg + sizeof(To), reinterpret_cast<byte*>(&converted));
    }
    else
    {
        reverse_copy(beg, beg + sizeof(To), reinterpret_cast<byte*>(&converted));
    }
    return converted;
}

template <typename To>
To from_little_endian_array(std::array<std::byte, sizeof(To)> bytes)
{
    using std::endian, std::byte;
    To converted;
    if (endian::native == endian::little)
    {
        copy(cbegin(bytes), cend(bytes), reinterpret_cast<byte*>(&converted));
    }
    else
    {
        reverse_copy(cbegin(bytes), cend(bytes), reinterpret_cast<byte*>(&converted));
    }
    return converted;
}

template <typename To, std::input_iterator ByteIter>
To from_big_endian(ByteIter beg)
{
    using std::endian, std::byte;
    To converted;
    if (endian::native == endian::big)
    {
        copy(beg, beg + sizeof(To), reinterpret_cast<byte*>(&converted));
    }
    else
    {
        reverse_copy(beg, beg + sizeof(To), reinterpret_cast<byte*>(&converted));
    }
    return converted;
}

template <typename To>
To from_big_endian_array(std::array<std::byte, sizeof(To)> bytes)
{
    using std::endian, std::byte;
    To converted;
    if (endian::native == endian::big)
    {
        copy(cbegin(bytes), cend(bytes), reinterpret_cast<std::byte*>(&converted));
    }
    else
    {
        reverse_copy(cbegin(bytes), cend(bytes), reinterpret_cast<std::byte*>(&converted));
    }
    return converted;
}

template <typename Tout, std::input_iterator Iterator, std::sentinel_for<Iterator> Sentinel, std::output_iterator<Tout> IteratorOut>
IteratorOut many_from_big_endian(Iterator beg, Sentinel end, IteratorOut start)
{
    decltype(std::iterator_traits<Iterator>::difference_type) byte_count = end - beg;
    size_t bytes_per_unit = sizeof(Tout);
    if (byte_count % bytes_per_unit != 0)
    {
        throw runtime_error("number of bytes given is not an integer multiple of the bytes per unit (count " + to_string(byte_count) + ", unit " + std::to_string(bytes_per_unit) + ")");
    }
    for (auto i = beg; i != end; i += sizeof(Tout))
    {
        *(start++) = from_big_endian<Tout>(i);
    }
    return start;
}
 
template <typename Tout, std::input_iterator Iterator, std::sentinel_for<Iterator> Sentinel, std::output_iterator<Tout> IteratorOut>
IteratorOut many_from_little_endian(Iterator beg, Sentinel end, IteratorOut start)
{
    auto byte_count = end - beg;
    auto bytes_per_unit = sizeof(Tout);
    if (byte_count % bytes_per_unit != 0)
    {
        throw runtime_error("number of bytes given is not an integer multiple of the bytes per unit (count " + to_string(byte_count) + ", unit " + std::to_string(bytes_per_unit) + ")");
    }
    for (auto i = beg; i != end; i += sizeof(Tout))
    {
        *(start++) = from_little_endian<Tout>(i);
    }
    return start;
}

#endif /* D5628A2C_0FB5_43F0_B7AA_59BCC584DFBD */
