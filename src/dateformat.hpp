
// Format https://en.cppreference.com/w/cpp/io/manip/get_time
// a 	parses the name of the day of the week, either full or abbreviated, e.g. Fri
// d 	parses the day of the month as a decimal number (range [01,31]), leading zeroes permitted but not required 
// m 	parses the month as a decimal number (range [01,12]), leading zeroes permitted but not required 
// Y 	parses full year as a 4 digit decimal number, leading zeroes permitted but not required 

#ifndef B19B29F2_EB57_4F63_B0E0_4DEAE7F64746
#define B19B29F2_EB57_4F63_B0E0_4DEAE7F64746

const char *DATE_FORMAT = "%a %d.%m.%Y";

std::string make_daytime_string()
{
	using namespace std;
	using std::chrono::system_clock;

	time_t now_count = system_clock::to_time_t(system_clock::now());
	tm now  = *localtime(&now_count);

	ostringstream oss;
	oss << put_time(&now, DATE_FORMAT);
	return oss.str();
}


#endif /* B19B29F2_EB57_4F63_B0E0_4DEAE7F64746 */
