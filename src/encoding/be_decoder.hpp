#ifndef CA3AB780_6303_445E_A880_D882CC5A0B68
#define CA3AB780_6303_445E_A880_D882CC5A0B68

#include "endian.hpp"

// TODO make common interface with big_endian_encoder? maybe?
struct big_endian_decoder
{
    std::byte *src; // TODO change to input_iterator

    template <std::integral Int>
    big_endian_decoder operator|(Int &val)
    {
        val = from_big_endian<Int>(src);
        src += sizeof(Int);
        return *this;
    }

    big_endian_decoder(std::byte *src_) : src{src_}
    {

    }    
};

#endif /* CA3AB780_6303_445E_A880_D882CC5A0B68 */
