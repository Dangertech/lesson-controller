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
	else if (check_timeframe_availability() == -1)
	{ // There aren't enough timeframes for every lesson
			queue_error(std::string(C_RED_B) + "There is no timeframe "
					+ "for every lesson in the timetable" + std::string(C_OFF));
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
	 
	print_errors();
	 
	///// Process arguments
	process_args(argc, argv);
	 
	
	
	// Print errors here already, in
	// case the programe doesn't reach
	// the end because of an error
	print_errors();
	 
	 
	if (write_data == true)
		write_table();
	 
	print_errors();
	
	return 0;
}
