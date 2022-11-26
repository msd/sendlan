#ifndef D7D18BE8_C218_4621_9022_7088CA3CBCF7
#define D7D18BE8_C218_4621_9022_7088CA3CBCF7

#include "encoding/be_encoder.hpp"
#include "encoding/be_decoder.hpp"

using HDR_DATA_SIZE_T = uint32_t;
using HDR_NAME_SIZE_T = uint32_t;

struct packet_header
{
    HDR_NAME_SIZE_T name_size;
    HDR_DATA_SIZE_T data_size;

    auto encode(std::byte *dest)
    {
        big_endian_encoder enc{dest};
        enc
        | name_size
        | data_size;
        return enc.dest;
    }

    static auto decode(std::byte *src)
    {
        packet_header hdr{};
        big_endian_decoder(src)
        | hdr.name_size
        | hdr.data_size;
        return hdr;
    }
};

#endif /* D7D18BE8_C218_4621_9022_7088CA3CBCF7 */
