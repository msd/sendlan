#ifndef A87A1539_20F8_4BC5_8F73_130378BDF903
#define A87A1539_20F8_4BC5_8F73_130378BDF903
#include <sstream>
#include <iomanip>
#include <limits>
#include "endian.hpp"
#include "message.hpp"
#include "format.hpp"

using HDR_SIZE_T = uint32_t;
using packet_str = std::basic_string<std::byte>;

std::string to_string(packet_str p)
{
    // TODO fix join_with not using to_string(byte) and
    // then change to join_with_bytes to regular join_with
    return "[" + join_with(p, ", ") + "]";
}

packet_str to_packet(const char* str, size_t len)
{
    return {reinterpret_cast<const std::byte*>(str), len};
}

packet_str to_packet(std::string_view str)
{
    return to_packet(str.data(), str.size());
}

packet_str packet_with_message(std::string_view contents)
{
    packet_str pkt{4, std::byte{0}}; // reserve space for size
    auto hdr_size = static_cast<HDR_SIZE_T>(contents.size());
    to_big_endian(hdr_size, pkt.data());
    pkt.append(1, std::byte{0});
    pkt += {reinterpret_cast<const std::byte*>(contents.data()), contents.size()};
    return pkt;
}

struct ByteExtractor {
    packet_str &data;
    packet_str::const_iterator it;
    auto position()
    {
        return std::distance(cbegin(data), it);
    }
    auto remaining()
    {
        return std::distance(it, cend(data));
    }
    ByteExtractor(packet_str &_data): data{_data}, it(cbegin(data))
    {

    }
    template <size_t N>
    auto extract_bytes()
    {
        if (remaining() < N)
        {
            throw std::runtime_error("tried to read " + std::to_string(N) + " bytes with only " + std::to_string(remaining()) + " bytes remaining (position:" + std::to_string(position()) + ", size: " + std::to_string(data.size()) + ")");
        }
        std::array<std::byte, N> bytes;
        std::copy(it, it + N, bytes.data());
        std::advance(it, N);
        return bytes;
    }
    // template <typename T>
    // T extract_stuff(packet_str stream);

    // template <>
    auto extract_int()
    {
        auto bytes = extract_bytes<sizeof(HDR_SIZE_T)>();
        return from_big_endian_array<HDR_SIZE_T>(bytes);
    }

    auto extract_rest()
    {
        return std::vector<std::byte>(it, cend(data));
    }
};

message decode_message(packet_str &&data)
{
    constexpr int MIN_SIZE = 5;
    if (data.size() < MIN_SIZE)
    {
        throw std::runtime_error("data does not contain a full header");
    }
    ByteExtractor ex{data};
    
    auto msg_len = ex.extract_int(); // hdr size

    if (ex.extract_bytes<1>().front() != std::byte{0})
    {
        throw std::runtime_error("corrupted header");
    }
    
    auto msg = ex.extract_rest(); // msg
    if (msg.size() != msg_len)
    {
        std::cout << to_string(data);
        throw std::runtime_error("message received is of length " + std::to_string(msg.size()) + " but expected size " + std::to_string(msg_len));
    }

    return { msg };
}

class PACKET
{
    template <std::integral N>
    check_length(N n)
    {
        if (n > std::numeric_limits<HDR_SIZE_T>::max())
        {
            throw std::runtime_error("data too big");
        }
    }
};


// TODO
// PACKET encode_message(message)
// {

// }

// TODO
// message decode_message(message)



#endif /* A87A1539_20F8_4BC5_8F73_130378BDF903 */
