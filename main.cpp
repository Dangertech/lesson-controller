#include <iostream>
#include <cstring>
#include <cctype>
#include <vector>
#include "timecalc.h"
#include "lesson.h"
#include "utils.h"
#include "args.h"



int main(int argc, char *argv[])
{
	get_time();
	
	///// Get config arguments
	 
	// Two loops are needed to ensure config like terse output can be put anywhere in the arguments
	for (int my_arg = 1; my_arg < argc; my_arg++)
	{
		if (strcmp(argv[my_arg], "-t") == 0 || strcmp(argv[my_arg], "--terse") == 0)
			terse = true;
		else if (strcmp(argv[my_arg], "-c") == 0 || strcmp(argv[my_arg], "--no-count-empties") == 0)
			count_empties = false;
	}
	 
	 
	if (check_timeframe_availability() == -1) // Check if there are enough timeframes to match all lessons
	{					  // issue a warning if not
		std::cout << "\033[1;31m"; // Start making text red
		std::cout << "Warning: There isn't a timeframe for every lesson in the timetable" << std::endl;
		std::cout << "\033[0m"; // Stop making text red
	}
	 
	
	///// Execute arguments
	 
	for (int my_arg = 1; my_arg < argc; my_arg++)
	{
		// Get if an argument is a weekday and store the weekday_id
		int weekday_check = vecstrcmp(argv[my_arg], weekdays);
		 
		// Execute actions based on arguments
		// Currently only incorporates the first argument, 
		// needs to be loop for all in the future
		if (weekday_check != -1) // Show timetable of given weekday
		{
			if (terse == false) // Show the requested weekday in a fancy format
			{
				std::string my_weekday = weekdays[weekday_check];
				my_weekday[0] = toupper(my_weekday[0]);
				std::cout << "--- " << my_weekday << " ---" << std::endl;
			}
			if (table[weekday_check].size() == 0 && terse == false)
				std::cout << "No lessons registered" << std::endl;
			for (int i = 0; i < table[weekday_check].size(); i++)
			{
				show_lesson(weekday_check, i);
			}
		}
		else if (strncmp(argv[my_arg], "+", 1) == 0 || strncmp(argv[my_arg], "-", 1) == 0)
		{ // Show +n lessons
			if (isdigit(argv[my_arg][1]) != 0) // Don't process other "-" arguments
			{
				int to_skip = atoi(argv[my_arg]);
				int current_lesson = get_lesson(hour, minute);
				 
				int targeted_lesson;
				if (current_lesson == -1)
					targeted_lesson = 0;
				else if (current_lesson == -2)
					targeted_lesson = timeframes.size() - 1; 
				else
					targeted_lesson = current_lesson;
				int targeted_day = day; // Day of timecalc.cpp
				 
				if (to_skip >= 0)
				{
					for (int i = 0; i<to_skip; i++)
					{
						targeted_lesson++;
						if (targeted_lesson > timeframes.size() - 1)
						{
							targeted_day++;
							targeted_lesson = 0;
						}
						if (targeted_day > 6)
							targeted_day = 0;
					}
				}
				else
				{
					for (int i = 0; i>to_skip; i--)
					{
						targeted_lesson--;
						if (targeted_lesson < 0)
						{
							targeted_day--;
							targeted_lesson = timeframes.size() - 1;
						}
						if (targeted_day < 0)
							targeted_day = 6;
					}
				}
				
				if (terse == false)
					std::cout << "Targeted day: " << weekdays[targeted_day] 
						  << "; Targeted lesson: " << targeted_lesson << std::endl;
				 
				show_lesson(targeted_day, targeted_lesson);
			}
		}
		else if (strcmp(argv[my_arg], "time") == 0) // Only meant for debug
			std::cout << "Current time: " << hour << ":" << minute << std::endl;
		else
			std::cout << "Invalid argument '" << argv[my_arg] << "'" << std::endl;
	}
	 
	return 0;
}
