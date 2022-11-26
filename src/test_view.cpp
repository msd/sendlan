#include <iostream>
#include "packets/file_packet.hpp"

static constexpr int ALPHA_LOWER_COUNT = 26;
static constexpr int ALPHA_UPPER_COUNT = 26;
static constexpr int DIGIT_COUNT = 10;
static constexpr int ALPHA_COUNT = ALPHA_LOWER_COUNT + ALPHA_UPPER_COUNT;
static constexpr int ALPHANUM_COUNT = ALPHA_COUNT + DIGIT_COUNT;

char index_lower_alpha(int x)
{
    return 'a' + x % ALPHA_LOWER_COUNT;
}

char index_upper_alpha(int x)
{    
    return 'A' + x % ALPHA_UPPER_COUNT;
}

char index_alpha(int x)
{
    int x_ = x % ALPHA_COUNT;
    if (x_ < ALPHA_LOWER_COUNT)
    {
        return index_lower_alpha(x_);
    }
    return index_upper_alpha(x_ - ALPHA_LOWER_COUNT);
}

char index_digits(int x)
{
    return '0' + (x % DIGIT_COUNT);
}

char index_alphanum(int x)
{
    int x_ = x % ALPHANUM_COUNT;
    if (x_ < ALPHA_COUNT)
    {
        return index_alpha(x_);
    }
    return index_digits(x_ - ALPHA_COUNT);
}

int main(int argc, char const *argv[])
{
    std::string path = "hello";
    {
        auto f = std::ofstream(path);
        constexpr int loop_write_count = 9;
        char x[loop_write_count];
        int wrote = 0;
        int desired_len = 20;
        for (size_t i = 0; i < desired_len / loop_write_count; i++)
        {
            char fill_char = index_alphanum(i);
            std::fill(x, x + loop_write_count, fill_char);
            f.write(x, loop_write_count);
            wrote += loop_write_count;
        }
    }
    return 0;
}
