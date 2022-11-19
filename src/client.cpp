#include <iostream>
#include <bitset>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include "info.hpp"
#include "endian.hpp"
#include "packets/file_packet.hpp"
using boost::asio::ip::tcp;

constexpr size_t BUFFER_SIZE = 4096;

int main(int argc, char* argv[])
{
    try
    {
        if (argc != 2)
        {
            std::cerr << "Usage: client <host>" << std::endl;
            return 1;
        }

        boost::asio::io_context io_context;

        // tcp::resolver resolver(io_context);
        // tcp::resolver::results_type endpoints = resolver.resolve(argv[1], "daytime");
        // tcp::socket socket(io_context);
        // boost::asio::connect(socket, endpoints);

        tcp::socket socket(io_context);
        socket.connect(tcp::endpoint(boost::asio::ip::make_address_v4(argv[1]), PORT));

        packet_str packet_data;

        while (true)
        {
            boost::array<std::byte, BUFFER_SIZE> buf;
            boost::system::error_code error;

            size_t len = socket.read_some(boost::asio::buffer(buf), error);

            if (error == boost::asio::error::eof)
                break; // Connection closed cleanly by peer.
            else if (error)
                throw boost::system::system_error(error); // Some other error.

            packet_data.append(buf.data(), len);
        }
        
        message m = decode_message(std::move(packet_data));

        std::cout << "Received message " << m.data.size() << '\n';
    }
    catch (std::exception& e)
    {
        std::cerr << "ERROR: " << e.what() << std::endl;
    }

    return 0;
}
