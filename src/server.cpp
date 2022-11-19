#include <ctime>
#include <iostream>
#include <string>
#include <boost/bind/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <iomanip>
#include <filesystem>
#include <fstream>
#include "info.hpp"
#include "endian.hpp"
#include "read_file.hpp"
#include "packets/file_packet.hpp"
#include "packets/message_packet.hpp"

using boost::asio::ip::tcp;


class tcp_connection : public boost::enable_shared_from_this<tcp_connection>
{
	const std::string path_;
public:
	typedef boost::shared_ptr<tcp_connection> pointer;

	static pointer create(std::string path, boost::asio::io_context& io_context)
	{
		return pointer(new tcp_connection(path, io_context));
	}

	tcp::socket& socket()
	{
		return socket_;
	}

	void start()
	{
		auto bytes = read_file(path_);
		packet = message_packet(bytes).string();

		boost::asio::async_write(socket_, boost::asio::buffer(packet),
				boost::bind(&tcp_connection::handle_write, shared_from_this(),
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred));
	}

private:
	tcp_connection(const std::string &path, boost::asio::io_context& io_context)
		: path_(path), socket_(io_context)
	{
	}

	void handle_write(const boost::system::error_code& /*error*/, size_t /*bytes_transferred*/)
	{
	}

	tcp::socket socket_;
	packet_str packet;
};

class tcp_server
{
	const std::string &path_;
public:
	tcp_server(const std::string& path, boost::asio::io_context& io_context) :
		path_(path),
		io_context_(io_context),
		acceptor_(io_context, tcp::endpoint(tcp::v4(), PORT))
	{
		start_accept();
	}

private:
	void start_accept()
	{
		tcp_connection::pointer new_connection = tcp_connection::create(path_, io_context_);

		acceptor_.async_accept(new_connection->socket(),
				boost::bind(&tcp_server::handle_accept, this, new_connection,
					boost::asio::placeholders::error));
	}

	void handle_accept(tcp_connection::pointer new_connection,
			const boost::system::error_code& error)
	{
		if (!error)
		{
			new_connection->start();
		}

		start_accept();
	}

	boost::asio::io_context& io_context_;
	tcp::acceptor acceptor_;
};

using std::to_string;

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << "syntax " << argv[0] << " <file>" << std::endl;
		return 1;
	}
	std::string filename = argv[1];
	try
	{
		boost::asio::io_context io_context;
		tcp_server server(filename, io_context);
		io_context.run();
	}
	catch (std::exception& e)
	{
		std::cerr << "ERROR: " << e.what() << std::endl;
	}
	return 0;
}
