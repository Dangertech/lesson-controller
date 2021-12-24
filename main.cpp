#include <iostream>
#include <cstring>
#include <cctype>
#include <vector>
#include "timecalc.h"
#include "lesson.h"
#include "args.h"



int main(int argc, char *argv[])
{
	// set the string CONF_FILE_LOC
	set_conf_file_loc();
	// Read the config file where all other locations are stored
	if (read_config() == ERR_NONEXISTENT_FILE)
		queue_error(std::string(C_RED_B)
				+ "The config location the program tried to determine, failed.\n"
				+ "The determined file does not exist. \nPlease fix this by either creating\n"
				+ std::string(C_OFF) + CONF_FILE_LOC + std::string (C_RED_B)
				+ "\nor setting the " + std::string(C_OFF) + "LESSON_CONFIG " + std::string(C_RED_B)
				+ "environment variable to your config file.");
	get_time(); // Get the current time and write it into the variables
	int timeread = read_timeframes(); // Read the timeframes from the file specified in lesson.h
	int lessonread = read_lessondata();

	if (timeread == ERR_NONEXISTENT_FILE)
	{
		queue_error(std::string(C_RED_B) + "There is no timeframe datafile "
								+ "at your data location given in the config file;\n"
								+ "Please create one using "
								+ std::string(C_OFF) + "lesson --create-timeframes "
								+ std::string(C_RED_B) + "or change the location in your "
								+ "config file to the appropriate one;" 
								+ std::string(C_OFF), true, "timeframes");
	}
	if (lessonread == ERR_NONEXISTENT_FILE)
	{
		queue_error(std::string(C_RED_B) + "There is no lessondata file "
					+ "at your data location given in the config file;\n"
					+ "Please create one using "
					+ std::string(C_OFF) + "lesson --create-datafile "
					+ std::string(C_RED_B) + "or change the location in your "
					+ "config file to the appropriate one;"
					+ std::string(C_OFF), true, "lessondata");
	}
	 
	///// Get config arguments
	 
	for (int my_arg = 1; my_arg < argc; my_arg++)
	{
		if (strcmp(argv[my_arg], "-t") == 0 || strcmp(argv[my_arg], "--terse") == 0)
			terse = true;
		else if (strcmp(argv[my_arg], "-c") == 0 || strcmp(argv[my_arg], "--no-count-empties") == 0)
			count_empties = false;
		else if (strcmp(argv[my_arg], "-H") == 0 || strcmp(argv[my_arg], "--no-show-header") == 0)
			table_header = false;
		else if (strcmp(argv[my_arg], "-T") == 0 || strcmp(argv[my_arg], "--no-show-title") == 0)
			title = false;
		else if (strcmp(argv[my_arg], "-C") == 0 || strcmp(argv[my_arg], "--no-show-color") == 0)
			clear_colors();
		else if (vecstrcmp(argv[my_arg], {"help", "Help", "--help", "-h", "-help"}) != ERROR)
			show_help();
		// Reset datafiles
		else if (strcmp(argv[my_arg], "--create") == 0)
		{
			// Create only differs from reset in that it does not ask the user
			query_reset(LESSON_FILE_LOC, false);
			query_reset(TIME_FILE_LOC, false);
			return 0;
		}
		else if (strcmp(argv[my_arg], "--create-lessondata") == 0)
		{
			query_reset(LESSON_FILE_LOC, false);
			return 0;
		}
		else if (strcmp(argv[my_arg], "--create-timeframes") == 0)
		{
			query_reset(TIME_FILE_LOC, false);
			return 0;
		}
		else if (strcmp(argv[my_arg], "--reset") == 0)
		{
			query_reset(LESSON_FILE_LOC);
			query_reset(TIME_FILE_LOC);
			// Return because the program tries to read the example
			// files now, although they should be modified beforehand
			return 0;
		}
		else if (strcmp(argv[my_arg], "--reset-lessondata") == 0)
		{
			query_reset(LESSON_FILE_LOC);
			return 0;
		}
		else if (strcmp(argv[my_arg], "--reset-timeframes") == 0)
		{
			query_reset(TIME_FILE_LOC);
			return 0;
		}
		// Default end thingy, the switch must be invalid
		else if (strncmp(argv[my_arg], "-", 1) == 0 && isalpha(argv[my_arg][1]) != 0)
		{
			 std::cout << "Invalid switch '" << std::string(argv[my_arg]) << "'" << std::endl
					   << "To get info on how to use this program, use " << C_GREEN_U << "lesson --help" << C_OFF << std::endl;
		}
	}
	 
	 
	if (check_timeframe_availability() == -1) // Check if there are enough timeframes to match all lessons
	{					  // issue a warning if not
		queue_error(std::string(C_RED_B) + "There is no timeframe "
				+ "for every lesson in the timetable" + std::string(C_OFF));
	}
	
	/*
	// TO TEST THE NEW show_lessons() FUNCTION, REMOVE AT MERGE!!!
	std::vector <std::vector <std::pair<int,int>>> test =
	{
		{ {0,0},{1,0},{2,0} },
		{ {0,1},{1,1},{2,1} },
		{ {0,2},{1,2},{2,2} }
	};
	show_lessons(test);
	return 0;
	*/
	 
	// If there are no arguments, show the week and exit
	if (argc == 1)
	{
		show_week();
		return 0;
	}
	
	// Print errors here already, in
	// case the programe doesn't reach
	// the end because of an error
	print_errors();
	 
	//////////////// Execute arguments
	 
	for (int my_arg = 1; my_arg < argc; my_arg++)
	{
		// Get if an argument is a weekday and store the weekday_id
		// Get the responding array entry for the given argument, ERROR if nothing corresponds
		char* test_day = argv[my_arg];
		for (int i = 0; i<strlen(argv[my_arg]); i++)
			test_day[i] = tolower(test_day[i]);
		 
		// Execute actions based on arguments
		int relday_check = vecstrcmp(argv[my_arg], {"yesterday", "today", "tomorrow"});
		// Show timetable of given weekday
		if (vecstrcmp(test_day, weekdays) != ERROR) 
			show_single_day(vecstrcmp(test_day, weekdays));
		else if ( relday_check != ERROR)
		{
			int show;
			if (relday_check == 0)
				show = day-1;
			else if (relday_check == 1)
				show = day;
			else if (relday_check == 2)
				show = day+1;
			show_single_day(show);
		}
		// Show relative lesson (+1/+3/-5/+0)
		else if (strncmp(argv[my_arg], "+", 1) == 0 || strncmp(argv[my_arg], "-", 1) == 0)
		{
			if (isdigit(argv[my_arg][1]) != 0) // Don't pass other "-" arguments
			{
				rel_lesson(atoi(argv[my_arg])); // Show +n lessons
			}
		}
		else if (strcmp(argv[my_arg], "0") == 0)
			rel_lesson(atoi(argv[my_arg]));
		else
		{
			std::cout << "Invalid argument '" << argv[my_arg] << "'" << std::endl
					  << "To get info on how to use this program, use " << C_GREEN_U << "lesson --help" << C_OFF << std::endl;
		}
	}
	 
	if (write_data == true)
		write_table();

	print_errors();
	
	return 0;
}
