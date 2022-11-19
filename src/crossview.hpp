#ifndef BCEDC228_BF2A_4889_8F66_19377E21BFCA
#define BCEDC228_BF2A_4889_8F66_19377E21BFCA

class cross_view : public std::ranges::view_interface<cross_view>
{
	using vec = std::vector<int>;
	using iterator = class iterator {
		public:
		vec::const_iterator a_beg, a_end, a_i, b_i;
		iterator(const vec &a, const vec &b) :
			a_beg{cbegin(a)}, a_end{cend(a)},
			a_i{cbegin(a)}, b_i{cbegin(b)}
		{
		}
		iterator& operator++()
		{
			if (a_i + 1 == a_end)
			{
				a_i = a_beg;
				++b_i;
			}
			else
			{
				++a_i;
			}
			return *this;
		}
		auto operator*()
		{
			return std::make_pair(*a_i, *b_i);
		}
	};

	using sentinel = class sentinel {
		using inner_iter = vec::const_iterator;
		inner_iter b_end;
		public:
		sentinel(const vec &a, const vec &b) : b_end{cend(b)}
		{

		}
		bool operator==(iterator it)
		{
			return b_end == it.b_i;
		}
	};

	const std::vector<int> &a, &b;
public:
	cross_view(const std::vector<int>& a_, const std::vector<int> & b_) : a(a_), b(b_)
	{

	}
	iterator begin()
	{
		return {a, b};
	}
	sentinel end()
	{
		return {a, b};
	}
};

void cross(const std::vector<int> &b)
{
	// return TODO
}


#endif /* BCEDC228_BF2A_4889_8F66_19377E21BFCA */
