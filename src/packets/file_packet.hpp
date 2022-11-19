#ifndef A477C3C4_722C_4710_AA39_05881B959ACD
#define A477C3C4_722C_4710_AA39_05881B959ACD

#include <filesystem>
#include <iostream>
#include <ranges>
#include "packet.hpp"

namespace fs = std::filesystem;

class file_view : std::ranges::view_interface<file_view>
{
    fs::path path;
    using iterator = struct iterator
    {
        std::vector<std::byte> data;
        std::basic_ifstream<std::byte> input_stream;
        const auto &operator*()
        {
            return data;
        }
        iterator &operator++()
        {
            if (eof())
            {
                data.clear();
            }
            else // not eof
            {
                input_stream.read(data.data(), data.size());
                //            // read data
                //            
                //            if // success
                //            {
                //                // data = new data
                //            }
                //            else
                //            {
                //                data.empty();
                //            }
                //        }
            }
            
            return *this;
        }
        bool eof() const
        {
            return input_stream.eof();
        }
        iterator(const fs::path &path_) :input_stream{path_}
        {

        }
    };

    using sentinel = struct sentinel {
        bool operator==(const iterator& it) const
        {
            return it.eof() && it.data.empty();
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
};

#endif /* A477C3C4_722C_4710_AA39_05881B959ACD */
