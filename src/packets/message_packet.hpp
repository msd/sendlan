#ifndef C51237B8_38DA_47A9_A5D7_242E0390B2F0
#define C51237B8_38DA_47A9_A5D7_242E0390B2F0

#include <ranges>

#include "../packet.hpp"

constexpr std::byte HEADER_SPLIT{0};
constexpr std::byte DATA_SPLIT{0};

class message_packet : PACKET
{
    packet_str str;
public:
    message_packet(std::string_view message)
    {
        if (message.size() > std::numeric_limits<HDR_DATA_SIZE_T>::max())
        { 
            throw std::runtime_error("message too long");
        }

        auto message_bytes = reinterpret_cast<const std::byte*>(message.data());
        auto message_len = static_cast<HDR_DATA_SIZE_T>(message.size());
        to_big_endian(message_len, back_inserter(str));
        str.append(1, std::byte{0});
        std::copy(message_bytes, message_bytes + message_len, std::back_inserter(str));
    }
    auto string()
    {
        return str;
    }
};

#endif /* C51237B8_38DA_47A9_A5D7_242E0390B2F0 */
