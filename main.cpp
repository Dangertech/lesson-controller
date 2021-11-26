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

	if (timeread == ERR_NONEXISTENT_FILE && lessonread == ERR_NONEXISTENT_FILE)
	{
		// User probably started the application for the first time
		// Tell the user to do lesson --create
	}
	else if (timeread == ERR_NONEXISTENT_FILE)
	{
		queue_error(std::string(C_RED_B) + "There is no timeframe datafile "
								+ "at your data location given in the config file;\n"
								+ "Please create one using "
								+ std::string(C_OFF) + "lesson --create-timeframes "
								+ std::string(C_RED_B) + "or change the location in your "
								+ "config file to the appropriate one;" 
								+ std::string(C_OFF), true, "timeframes");
	}
	else if (lessonread == ERR_NONEXISTENT_FILE)
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
		// Default end thingy, the switch must be invalid
		// else if (strncmp(argv[my_arg], "-", 1) == 0 && isalpha(argv[my_arg][1]) != 0)
		// 	queue_error("Invalid switch '" + std::string(argv[my_arg]) + "'");
	}
	 
	 
	if (check_timeframe_availability() == -1) // Check if there are enough timeframes to match all lessons
	{					  // issue a warning if not
		queue_error(std::string(C_RED_B) + "There is no timeframe "
				+ "for every lesson in the timetable" + std::string(C_OFF));
	}
	
	 
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
		int weekday_check = vecstrcmp(test_day, weekdays); 
		 
		// Execute actions based on arguments
		// Currently only incorporates the first argument, 
		// needs to be loop for all in the future
		
		if (weekday_check != ERROR) // Show timetable of given weekday
			show_single_day(weekday_check);
		 
 		else if (vecstrcmp(argv[my_arg], {"help", "Help", "--help", "-h", "-help"}) != ERROR)
		{
			std::cout << "Usage: lesson [SWITCHES...] [FILTER...]" << std::endl
				<< std::endl
				<< "Tool to monitor and manage static timetables, like in school\n"
				<< std::endl
				<< "WARNING: THIS HELP TEXT CONTAINS FEATURES THAT ARE YET TO BE IMPLEMENTED\n"
				<< std::endl
				<< "FILTERS\n"
				<< "A filter can be a weekday, a specific lesson, or a relative lesson\n"
				<< std::endl
				<< "   lesson monday/tuesday, etc...\t"
				<< "show all lessons registered on this weekday\n"
				<< "   lesson +n\t\t\t\t"
				<< "show the lesson n lessons ahead of the current one\n"
				<< "   lesson -n\t\t\t\t"
				<< "show the lesson n lessons back of the current one\n"
				<< "   lesson -d n\t\t\t\t"
				<< "show the day n days from this one;\n"
				<< "\t\t\t\t\t\tcan be negative;\n"
				<< "\t\t\t\t\t\tcan be augmented with a specific lesson number\n"
				<< "\t\t\t\t\t\t(e.g. lesson -d 3 1 -> First lesson three days ahead)\n"
				<< "   lesson tomorrow/today/yesterday\t" 
				<< "supported relative weekday displays\n"
				<< std::endl
				<< "SWITCHES\n"
				<< std::endl
				<< "   -t/--terse\t\t\t\t"
				<< "makes all output terse, suited for script parsing\n"
				<< "   -c/--no-count-empties[WIP]t\t"
				<< "relative arguments do not count lessons that don't happen \n"
				<< "\t\t\t\t\t\tbut the timeframes indicate that there might be one\n"
				<< "   -H/--no-show-header\t\t\t"
				<< "don't display the header of the timetable\n"
				<< "   -T/--no-show-title\t\t\t"
				<< "don't show the title of tables that are shown in some situations\n"
				<< "\t\t\t\t\t\t(e.g. omit the \"--- Weekday ---\" or similar things)\n"
				<< std::endl
				<< std::endl
				<< std::endl
				<< "WRITING YOUR OWN TIMETABLE\n"
				<< "This section teaches you how to write your own datafile"
				<< " for the program to use.\n"
				<< "Don't worry, it's easy!\n"
				<< std::endl
				<< "1.   Find your config file\n"
				<< "     Your config file is either located at"
				<< " $XDG_CONFIG_HOME/lesson-controller/config.conf,\n"
				<< "     the $LESSON_CONFIG environment variable,"
				<< " or ~/.config/lesson-controller/config.conf\n"
				<< "2.   Find your data location in the config file\n"
				<< "     The variables LESSON_FILE_LOC and TIME_FILE_LOC"
				<< " set the location of your datafiles.\n"
				<< "     If these files don't exist, create them.\n"
				<< "3.   Syntax of your timeframe file:\n"
				<< "     The timeframe file sets, when your lessons start.\n"
				<< "     AT THE MOMENT, PAUSES BETWEEN LESSONS ARE UNFORTUNATELY NOT POSSIBLE.\n"
				<< "     Between two braces ({}), there is another set of braces for each"
				<< " timestamp.\n"
				<< "     In this other set, write the time, separated by a double colon, e.g. 7:45\n"
				<< "     Example timeframe file:\n"
				<< std::endl
				<< "     { # The minutes your lessons start every day\n"
				<< "     \t{7:45} {8:35} {9:35} {10:25} {11:25} {12:10} {12:55}\n"
				<< "     }\n"
				<< "4.   Syntax of your lessondata file:\n"
				<< "     Similar to the timeframe file, put everything into two braces ({})\n"
				<< "     Then, for every day, starting with sunday, there are seven braces\n"
				<< "     in which you can write the metadata for as many lessons as you have timeframes.\n"
				<< "     The metadata is located in more braces and follows this syntax:\n"
				<< "     {SUBJECT, TEACHER, ROOM}\n"
				<< "     You can mark free lessons with empty braces without commata [WIP]\n"
				<< "     Example lessondata file:\n"
				<< "     {\n"
				<< "        { # Sunday\n"
				<< "        }\n"
				<< std::endl
				<< "        { # Monday\n"
				<< "            { Maths, Smith, 666 } # Maths, with teacher Smith in room 666\n"
				<< "            {} # An empty lesson\n"
				<< "            { Computer_Science, Britta_Britt, My_favorite_room } # For spaces, use underscores\n"
				<< "                 # Underscores are converted to spaces, spaces themselves are not supported\n"
				<< "        }\n"
				<< "     }\n"
				<< "     You don't need braces for every weekday,\n"
				<< "     however, some issues might arise at the moment.\n"
				<< std::endl
				<< "YOU DON'T NEED TO UNDERSTAND THIS:\n"
				<< "If you installed the program with 'make install', stock/example config and data files\n"
				<< "should have been created under ~/.config/lesson-controller/\n"
				<< "You can also create these stock data files at any time using 'lesson --create' (or 'lesson --reset')\n"
				<< "They will appear at the location given in your config file.\n"
				<< std::endl
				<< "Have fun using lesson-controller, but note that it is still a Work in Progress\n"
				<< "Report any bugs to 'https://github.com/Dangertech/lesson-controller/issues'\n"
				<< "Written by Kjell Nagel, student at STAMA, Bad Kreuznach, Germany\n"
				;
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
		else if (strcmp(argv[my_arg], "time") == 0) // Only meant for debug
			std::cout << "Current time: " << hour << ":" << minute << std::endl;
		 
		else
			std::cout << "Invalid argument '" << argv[my_arg] << "'" << std::endl;
	}
	 
	if (write_data == true)
		write_table();

	print_errors();
	
	return 0;
}
