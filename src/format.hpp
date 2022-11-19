#include <string>
#include <ranges>

std::string to_string(const std::byte &b)
{
	static auto hex_digit = [](int x) -> char
	{
		if (x >= 10)
		{
			return (x - 10) + 'a';
		}
		return x + '0';
	};
	std::string str;
	// if (show_base)
	// {
	//     str += "0x";
	// }
	int i = static_cast<int>(b);
	str.append(1, hex_digit(i / 0x10));
	str.append(1, hex_digit(i % 0x10));
	return str;
};

using std::to_string;

struct join_with_view : public std::ranges::view_interface<join_with_view>
{
	template <class Range>
	void operator()(Range r, std::string_view delim)
	{

	}
};

// struct join_with: public std::ranges::views::__adaptor::_RangeAdaptor

// TODO create a range adaptor to do laizily
template <std::forward_iterator It, std::sentinel_for<It> Sen>
std::string join_with(It it, Sen sen, std::string delim)
{
	std::string joined;
	if (it != sen) // non empty range
	{
		for ( ; it + 1 != sen; ++it ) // first to second last elements
		{
			joined += ::to_string(*it);
			joined += delim;
		}
		joined += ::to_string(*it); // last element
	}
	return joined;
}

template <std::ranges::range Range>
std::string join_with(Range r, std::string delim)
{
	return join_with(std::ranges::cbegin(r), std::ranges::cend(r), std::move(delim));
}

template <class T>
concept stringable = requires (const T& t)
{
	to_string(t);
};


// template <std::ranges::range Range>
// std::string join_with(std::string_view delim)
// {
// 	using std::to_string;
	
// 	std::string joined;
	
// 	return joined;
// }
