#ifndef EC1DEDD8_4E1D_4446_A03E_2E372CF93417
#define EC1DEDD8_4E1D_4446_A03E_2E372CF93417

struct message
{
    std::vector<std::byte> data;
    std::string content_string()
    {
        return { reinterpret_cast<const char*>(data.data()), data.size() };
    }
};

message make_message(std::string_view content)
{
    auto beg = reinterpret_cast<const std::byte*>(content.data());
    auto end = beg + content.size(); 
    return { {beg, end} };
}

#endif /* EC1DEDD8_4E1D_4446_A03E_2E372CF93417 */
