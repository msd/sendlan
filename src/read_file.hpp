#ifndef D98A6679_676E_4063_AEE7_4F387A8D069F
#define D98A6679_676E_4063_AEE7_4F387A8D069F

#include <filesystem>

std::string read_file(std::string file_path)
{
	using namespace std;
	using namespace std::filesystem;

    if (!exists(file_path))
    {
        throw runtime_error("file does not exist");
    }
    ifstream ifs{ file_path };
    // std::string::_s
    return string{istreambuf_iterator<char>{ifs},{}};
}

template <typename Byte = char>
std::basic_string<Byte> read_file_binary(std::string_view file_path)
{
    using string = std::basic_string<Byte>;
    using ifstream = std::basic_ifstream<Byte>;
	using namespace std;
	using namespace std::filesystem;

    if (!exists(file_path))
    {
        throw runtime_error("file does not exist");
    }
    ifstream ifs{ file_path, ios_base::binary };
    // std::string::_s
    return string{istreambuf_iterator<Byte>{ifs},{}};
}

#endif /* D98A6679_676E_4063_AEE7_4F387A8D069F */
