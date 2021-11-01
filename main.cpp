#include <iostream>
#include <cstring>
#include <cctype>
#include <vector>
#include "timecalc.h"
#include "lesson.h"
#include "args.h"



int main(int argc, char *argv[])
{
	 
	get_time(); // Get the current time and write it into the variables
	int timeread = read_timeframes(); // Read the timeframes from the file specified in lesson.h
	int lessonread = read_lessondata();

	if (timeread == ERROR && lessonread == ERROR)
	{
		// User probably started the application for the first time
		// Tell the user to do lesson --create
	}
	else if (timeread == ERROR)
	{
		std::cout << C_RED_B << "There is no timeframe datafile at your data location given in the config file:" << std::endl
				  << C_OFF << C_GREEN_U <<  DIR_PREFIX << TIME_FILE_LOC << std::endl
				  << C_OFF << C_RED_B << "Please create one using " << C_OFF << "lesson --create-timeframes " << C_RED_B
				  << "or change the location in your config file to the appropriate one;" << C_OFF << std::endl;
	}
	else if (lessonread == ERROR)
	{
	}
	
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
		std::cout << C_RED_B; // Start making text red
		std::cout << "Warning: There isn't a timeframe for every lesson in the timetable" << std::endl;
		std::cout << C_OFF; // Stop making text red
	}
	 
	// If there are no arguments, show the week and exit
	if (argc == 1)
	{
		show_week();
		return 0;
	}
	
	///// Execute arguments
	 
	for (int my_arg = 1; my_arg < argc; my_arg++)
	{
		// Get if an argument is a weekday and store the weekday_id
		int weekday_check = vecstrcmp(argv[my_arg], weekdays); // Get the responding array entry for the given argument, -1 if nothing corresponds
		 
		// Execute actions based on arguments
		// Currently only incorporates the first argument, 
		// needs to be loop for all in the future
		
		if (weekday_check != ERROR) // Show timetable of given weekday
			show_single_day(weekday_check);
		 
		 
		// Show relative lesson (+1/+3/-5/+0)
		else if (strncmp(argv[my_arg], "+", 1) == 0 || strncmp(argv[my_arg], "-", 1) == 0)
		{
			if (isdigit(argv[my_arg][1]) != 0) // Don't pass other "-" arguments
			{
				rel_lesson(atoi(argv[my_arg])); // Show +n lessons
			}
		}
		 
		 
		else if (strcmp(argv[my_arg], "time") == 0) // Only meant for debug
			std::cout << "Current time: " << hour << ":" << minute << std::endl;
		 
		else
			std::cout << "Invalid argument '" << argv[my_arg] << "'" << std::endl;
	}
	 
	write_table();
	 
	return 0;
}
