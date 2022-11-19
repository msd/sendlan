#include <iostream>

#include "packet.hpp"
#include "packets/message_packet.hpp"

using namespace std;

template<class T, equality_comparable_with<T> S>
void assert_eq(T expected, S got, std::string_view msg)
{
    if (expected == got)
    {
        cout << "SUCCESS: " << msg << '\n';
    }
    else
    {
        cout << "FAILED: " << msg << '\n'
            << "\texpected: " << to_string(expected) << '\n'
            << "\tgot: " << to_string(got) << '\n';
    }
}

void test_new_message_packet()
{
    assert_eq(packet_with_message("").size(), 5, "size of packet with empty message");
    assert_eq(packet_with_message("hello").size(), 10, "size of packet with message 'hello");
    assert_eq(packet_with_message(""), to_packet("\0\0\0\0\0", 5), "empty packet");
    assert_eq(packet_with_message("hello"), to_packet("\0\0\0\5\0hello", 10), "packet 'hello'");

    assert_eq(packet_with_message("hello"), message_packet("hello").string(), "stevE");
}

int main()
{
    test_new_message_packet();
}
