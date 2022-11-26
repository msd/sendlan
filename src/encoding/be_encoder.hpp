#ifndef AA59472A_1FCD_493B_AEE4_A8323A54C6A1
#define AA59472A_1FCD_493B_AEE4_A8323A54C6A1

#include "endian.hpp"

struct big_endian_encoder
{
    std::byte *dest; // TODO change to output_iterator
    template <std::integral Int>
    big_endian_encoder &operator|(Int val)
    {
        to_big_endian(val, dest);
        dest += sizeof(Int);
        return *this;
    }
    big_endian_encoder(std::byte *dest_) : dest(dest_)
    {
    }
};

#endif /* AA59472A_1FCD_493B_AEE4_A8323A54C6A1 */
