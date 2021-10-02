#include <vector>
#include <string>
#include "lesson.h"
#include "timecalc.h"

int vecstrcmp(const char* string, std::vector < std::string > string_vec)
{
	for (int i = 0; i < string_vec.size(); i++)
	{
		if (string == string_vec[i])
		{
			return i;
		}
	}
	return -1;
}

int check_timeframe_availability()
{
	for (int i = 0; i<weekdays.size(); i++)
	{
		if (timeframes.size() < table[i].size())
			return -1;
	}
	return 0;
}
