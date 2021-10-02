#include <iostream>
#include <cstring>
#include <vector>
#include "timecalc.h"
#include "lesson.h"
#include "utils.h"



int main(int argc, char *argv[])
{
	get_time();
	if (check_timeframe_availability() == -1) // Check if there are enough timeframes to match all lessons
	{					  // issue a warning if not
		std::cout << "\033[1;31m"; // Start making text red
		std::cout << "Warning: There isn't a timeframe for every lesson in the timetable" << std::endl;
		std::cout << "\033[0m"; // Stop making text red
	}
	 
	// Get if an argument is a weekday and store the weekday_id
	int weekday_check = vecstrcmp(argv[1], weekdays);

	// Execute actions based on arguments
	// Currently only incorporates the first argument, 
	// needs to be loop for all in the future
	if (weekday_check != -1)
	{
		std::cout << "--- " << weekdays[weekday_check] << " ---" << std::endl;
		if (table[weekday_check].size() == 0)
			std::cout << "No lessons registered" << std::endl;
		for (int i = 0; i < table[weekday_check].size(); i++)
		{
			show_lesson(weekday_check, i);
		}
	}
	else if (strncmp(argv[1], "+", 1) == 0 || strncmp(argv[1], "-", 1) == 0)
	{
		int to_skip = atoi(argv[1]);
		std::cout << to_skip << std::endl;
	}
	else if (strcmp(argv[1], "time") == 0) //Only meant for debug
		std::cout << "Current time: " << hour << ":" << minute << std::endl;
	else
		std::cout << "Invalid argument '" << argv[1] << "'" << std::endl;
	 
	return 0;
}
