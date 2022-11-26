#ifndef A477C3C4_722C_4710_AA39_05881B959ACD
#define A477C3C4_722C_4710_AA39_05881B959ACD

#include <filesystem>
#include <iostream>
#include <ranges>
#include "packet.hpp"

namespace fs = std::filesystem;

static void close_file(std::FILE *file_)
{
    std::fclose(file_);
}

using unique_file = std::unique_ptr<std::FILE, decltype(&close_file)>;

static unique_file new_read_file(std::string path)
{
    return { std::fopen(path.c_str(), "rb"), &close_file };
}


class ReadFile 
{
    std::string path;
    unique_file f;
        void open()
        {

        }
    public:
        bool is_open()
        {
            return bool(f);
        }
        bool eof()
        {
            return std::feof(f.get());
        }
        bool has_more()
        {
            return is_open() && !eof();
        }

        auto read(unsigned char *dest, size_t buflen)
        {
            if (!has_more())
            {
                throw std::runtime_error("attempted to read from closed file");
            }
            return std::fread(dest, 1, buflen, f.get());
        }

        std::vector<unsigned char> chunk()
        {
            if (!has_more())
            {
                return  {};
            }
            int chunk_size = 400;
            std::vector<unsigned char> data;
            data.resize(chunk_size); // expand
            auto got = read(data.data(), chunk_size);
            data.resize(got);        // shrink
            return data;
        }
        
        ReadFile(std::string path_) : path{std::move(path_)}, f{new_read_file(path)}
        {
            if (!f)
            {
                throw std::runtime_error("failed to open file");
            }
        }
};

class file_view : public std::ranges::view_interface<file_view>
{
    fs::path path;
public:
    static constexpr size_t FILE_READ_CHUNK_SIZE = 10000;
    using iterator = class iterator
    {
        auto fillbuf()
        {
            input_stream.read(reinterpret_cast<char*>(data.data()), data.size());
            return input_stream.gcount();
        }
        public:
        std::vector<std::byte> data{FILE_READ_CHUNK_SIZE, std::byte{0}};
        std::basic_ifstream<char> input_stream;
        std::streamsize len = 0;
        auto operator*()
        {
            return std::ranges::subrange(data.cbegin(), data.cbegin() + len);
        }
        iterator &operator++()
        {
            if (finished())
            {
                len = 0;
                data.clear();
            }
            else
            {
                len = fillbuf();
            }
            return *this;
        }
        bool finished() const
        {
            return input_stream.eof() || input_stream.bad();
        }
        iterator(const fs::path &path_) :input_stream{path_}
        {
            fillbuf();
        }
    };

    using sentinel = struct sentinel {
        bool operator==(const iterator& it) const
        {
            return it.finished() && !it.data.empty();
        }
    };

    iterator begin()
    {
        return {path};
    }

    sentinel end()
    {
        return {};
    }
    // BROKEN! DO NOT USE
    file_view(fs::path path_) : path{path_}
    {

    }
};

class outgoing_file : PACKET
{
    fs::path file_path;
public:
    outgoing_file (fs::path file_path_)
        : file_path(file_path_)
    {
        if (!fs::exists(file_path))
        {
            std::stringstream ss;
            ss<< std::quoted(file_path.string());
            throw std::runtime_error("path " + ss.str() + " does not exist");
        }
        if (fs::is_directory(file_path))
        {
            std::stringstream ss;
            ss<< std::quoted(file_path.string());
            throw std::runtime_error("path " + ss.str() + " given is a directory");
        }
    }
    // optional<char> next_byte()
    // {
    //     if ()
    // }
};

#endif /* A477C3C4_722C_4710_AA39_05881B959ACD */
