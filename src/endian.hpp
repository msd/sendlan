#ifndef D5628A2C_0FB5_43F0_B7AA_59BCC584DFBD
#define D5628A2C_0FB5_43F0_B7AA_59BCC584DFBD

#include <algorithm>
#include <bit>
#include <exception>
#include <fstream>
#include <iterator>
#include <ranges>
#include <string>
#include <vector>
#include <sstream>

template<typename T>
concept TriviallyCopyable = std::is_trivially_copyable_v<T>;

template <class I, class O>
using copy_fn_fancy = decltype(std::copy<I, O>);

using copy_fn = std::byte* (*)(std::byte*, std::byte*, std::byte*);

namespace detail
{
    // copy bytes to array
    template <TriviallyCopyable T>
    static auto copy_bytes_to_array(T x, copy_fn cf)
    {
        std::array<std::byte, sizeof (T)> bytes;
        auto start = reinterpret_cast<std::byte*>(&x);
        cf(start, start + sizeof x, bytes.data());
        return bytes;
    }

    // Return a copy function that will copy the bytes from memory
    // to a buffer preserving the wanted endianess.
    // if wanted is native then std::copy else std::reverse copy
    static copy_fn choose_copy(std::endian wanted)
    {
        if (wanted == std::endian::native)
        {
            return &std::copy;
        }
        else{
            return &std::reverse_copy;
        }
    }

    namespace good
    {
        template <TriviallyCopyable Val>
        auto to_big_endian_array(Val val)
        {
            return detail::copy_bytes_to_array(val, detail::choose_copy(std::endian::big));
        }

        template <TriviallyCopyable Val>
        auto to_little_endian_array(Val val)
        {
            return detail::copy_bytes_to_array(val, detail::choose_copy(std::endian::little));
        }
    }
}

// template <std::input_iterator I, std::output_iterator O>
template <class I, class O>
O copy_appropriate(std::endian wanted, I b, I e, O o)
{
    if (std::endian::native == wanted)
    {
        return std::copy(b, e, o);
    }
    return std::reverse_copy(b, e, o);
}

using namespace detail::good;

// template <typename Val, typename ItOut>
template <TriviallyCopyable Val, std::output_iterator<std::byte> ItOut> // TODO use macro feature check
auto to_little_endian(Val val, ItOut beg)
{
    std::byte *bytes = reinterpret_cast<std::byte>(&val);
    return copy_appropriate(std::endian::little, bytes, bytes + sizeof val, beg);
}

// template <typename Val, typename ItOut>
template <TriviallyCopyable Val, std::output_iterator<std::byte> ItOut> // TODO use macro feature check
auto to_big_endian(Val val, ItOut beg)
{
    auto bytes = to_big_endian_array(val);
    return std::ranges::copy(bytes, beg).out;
}

template <TriviallyCopyable Val, std::input_iterator ByteIter>
Val from_little_endian(ByteIter beg)
{
    using std::endian, std::byte;
    Val converted;
    copy_appropriate(endian::little, beg, beg + sizeof(Val), reinterpret_cast<byte*>(&converted));
    return converted;
}

template <TriviallyCopyable Val>
Val from_little_endian_array(std::array<std::byte, sizeof(Val)> bytes)
{
    using std::endian, std::byte;
    Val converted;
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

// template <typename Val, typename ByteIter>
template <TriviallyCopyable Val, std::input_iterator ByteIter> // TODO use macro feature check
Val from_big_endian(ByteIter beg)
{
    using std::endian, std::byte;
    Val converted;
    if (endian::native == endian::big)
    {
        copy(beg, beg + sizeof(Val), reinterpret_cast<byte*>(&converted));
    }
    else
    {
        reverse_copy(beg, beg + sizeof(Val), reinterpret_cast<byte*>(&converted));
    }
    return converted;
}

// template <std::output_iterator Val>
template <TriviallyCopyable Val>
Val from_big_endian_array(std::array<std::byte, sizeof(Val)> bytes)
{
    using std::endian, std::byte;
    Val converted;
    copy_appropriate(endian::big, begin(bytes), end(bytes), reinterpret_cast<std::byte*>(&converted));
    return converted;
}

// template <typename Val, typename Iterator, typename Sentinel, typename IteratorOut>
// template <TriviallyCopyable Val, std::input_iterator Iterator, std::sentinel_for<Iterator> Sentinel, std::output_iterator<Val> IteratorOut> // TODO use macro feature check
template <TriviallyCopyable Val, std::input_iterator Iterator, std::output_iterator<Val> IteratorOut> // TODO use macro feature check
// template <class Val, class Iterator, class IteratorOut> // TODO use macro feature check
IteratorOut many_from_big_endian(Iterator beg, Iterator end, IteratorOut start)
{
    // decltype(std::iterator_traits<Iterator>::difference_type) byte_count = end - beg;
    auto byte_count = end - beg;
    size_t bytes_per_unit = sizeof(Val);
    if (byte_count % bytes_per_unit != 0)
    {
        std::ostringstream ss;
        ss << "number of bytes given is not an integer multiple of the bytes per unit (count "
        << byte_count << ", unit " << bytes_per_unit << ")";
        throw std::runtime_error(ss.str());
    }
    for (auto i = beg; i != end; i += sizeof(Val))
    {
        *(start++) = from_big_endian<Val>(i);
    }
    return start;
}
 
// template <typename Val, typename Iterator, typename Sentinel, typename IteratorOut>
// template <TriviallyCopyable Val, std::input_iterator Iterator, std::sentinel_for<Iterator> Sentinel, std::output_iterator<Val> IteratorOut> // TODO use macro feature check
template <TriviallyCopyable Val, std::input_iterator Iterator, std::output_iterator<Val> IteratorOut> // TODO use macro feature check
IteratorOut many_from_little_endian(Iterator beg, Iterator end, IteratorOut start)
{
    auto byte_count = end - beg;
    auto bytes_per_unit = sizeof(Val);
    if (byte_count % bytes_per_unit != 0)
    {
        std::stringstream err;
        err << "number of bytes given is not an integer multiple of the bytes per unit (count "
        << byte_count << ", unit " << bytes_per_unit << ")";
        throw std::runtime_error(err.str());
    }
    for (auto i = beg; i != end; i += sizeof(Val))
    {
        *(start++) = from_little_endian<Val>(i);
    }
    return start;
}

#endif /* D5628A2C_0FB5_43F0_B7AA_59BCC584DFBD */
