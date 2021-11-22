// Tracking for the input arguments given
#include <iostream>
#include <vector>
#include <string>
#include <string.h>
#include <fstream>
#include <ctype.h>
#include "lesson.h"
#include "timecalc.h"
#include "args.h"


bool terse = false;
bool count_empties = true;

// Just testing if it's better to hide the error vector and only 
// interact with it through functions
std::vector < std::string > error_messages;
void queue_error(std::string message, bool my_write_data, std::string my_print_loc)
{
	error_messages.push_back(message);
	if (my_write_data == false)
		write_data = false;

	if (my_print_loc == "timeframes")
		print_timeframe_loc = true;
	else if (my_print_loc == "lessondata")
		print_lessondata_loc = true;
	else if (my_print_loc == "both")
	{
		print_timeframe_loc = true;
		print_lessondata_loc = true;
	}
	else if(my_print_loc != "none")
		std::cerr << "queue_error() didn't receive a valid print_loc argument!";
}

void print_errors()
{
	// Ensure that the function only runs
	// if there are error messages;
	// Avoids segfaults
	if (error_messages.size() == 0 || terse == true)
		return;
	for (int i = 0; i<error_messages.size(); i++)
	{
									// Turn color off again, just in case
		std::cout << error_messages[i] << C_OFF; 
		if (i != error_messages.size()-1)
			std::cout << std::endl << std::endl;
	}
	if (print_lessondata_loc || print_timeframe_loc)
		std::cout << std::endl << std::endl;
	 
	///// Print file locations in case of errors in the files
	if (print_lessondata_loc)
		std::cout << C_RED_B << "Your lessondata file location: " << C_OFF
				  << "[ " << C_GREEN_U << LESSON_FILE_LOC << C_OFF << " ]"
				  << std::endl << std::endl;
	if (print_timeframe_loc)
		std::cout << C_RED_B << "Your timeframe file location: " << C_OFF
				  << "[ " << C_GREEN_U << TIME_FILE_LOC << C_OFF << " ]"
				  << std::endl << std::endl;

	if (write_data == false)
	{
		std::cout << C_GREEN_U << "Notice:" << C_OFF
				  << " No data was rewritten to avoid damaging "
				  << "your data files." << std::endl;
	}
	
	// Clearly separate errors from output
	std::cout << std::endl << std::endl;
	// Now you can clear the errors as they have been shown
	// This allows for calling this function multiple times during
	// the program flow without showing errors twice
	error_messages.clear();
}

// There might very well be an inbuilt solution but I don't care enough to google it
int int_length (int integer) // That do be a lot of ints
{
	if (integer < 0 && integer > -10)
		return 2;
	if (integer > 0 && integer < 10)
		return 1;
	if (integer > 9 && integer < 100)
		return 2;
	if (integer > 99 && integer < 1000)
		return 3;
	if (integer > 999 && integer < 10000)
		return 4;
	return ERROR;
}

int vecstrcmp(const char* string, std::vector < std::string > string_vec)
{
	for (int i = 0; i < string_vec.size(); i++)
	{
		if (std::string(string) == string_vec[i])
		{
			return i;
		}
	}
	return ERROR;
}

std::string cap(std::string to_cap)
{
	to_cap[0] = toupper(to_cap[0]);
	return to_cap;
}

std::string tabs(int tab_num)
{
	std::string tabs_to_give;
	for (int i = 0; i<tab_num; i++)
	{
		tabs_to_give += "\t";
	}
	return tabs_to_give;
}


//// ARGUMENTS PROCESSING

int check_timeframe_availability()
{
	if (timeframes.size() == 0)
	{
		queue_error(std::string(C_RED_B)
				+ "There are no timeframes set! Check that your "
				+ "config file points to your timeframe file.\n"
				+ "You can reset the timeframe file with " 
				+ std::string(C_OFF) + "lesson --reset-timeframes "
				+ std::string(C_RED_B) + "\nto learn about how your "
				+ "file should look like.", true, "timeframes");
		print_errors();
		exit(77);
	}
	for (int i = 0; i<weekdays.size(); i++)
	{
		if (table[i].size() == 0)
			continue;
		if (timeframes.size() < table[i].size())
			return -1;
	}
	return 0;
}

void rel_lesson(int to_skip)
{
	int current_lesson = get_lesson(hour, minute);
	 
	int targeted_lesson;
	if (current_lesson == -1)
		targeted_lesson = 0;
	else if (current_lesson == -2)
	{
		if (to_skip == 0)
		{
			if (terse == false)
				std::cout << "No active lesson registered." << std::endl;
			return;
		}
		targeted_lesson = timeframes.size() - 1; 
	}
	else
		targeted_lesson = current_lesson;
	int targeted_day = day; // Day of timecalc.cpp
	 
	if (to_skip >= 0)
	{
		// Increase
		for (int i = 0; i<to_skip; i++)
		{
			targeted_lesson++;
			if (targeted_lesson >= timeframes.size() - 1)
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
		// Decrease
		for (int i = 0; i>to_skip; i--)
		{
			targeted_lesson--;
			if (targeted_lesson < 0)
			{
				targeted_day--;
				targeted_lesson = timeframes.size() - 2;
			}
			if (targeted_day < 0)
				targeted_day = 6;
		}
	}
	
	if (terse == false) // Print out the lesson if terse is false
		std::cout << "Lesson " << targeted_lesson + 1 
			  << " on "
			  << cap(weekdays[targeted_day]) << ":" << std::endl;
	 
	show_lessons({ {targeted_day, targeted_lesson } });
}

void show_week()
{
	std::cout << "Instead of this text, the whole week will be shown later on" << std::endl
		  << "Meanwhile, you can do '" << C_GREEN_U 
		  << "lesson monday tuesday wednesday thursday friday" << C_OFF << "'"
		  << "to have a substitute." << std::endl;
}


//////// CONFIG FILE PARSING

std::string CONF_FILE_LOC;
// Set the CONF_FILE_LOC which references the location of config.conf
void set_conf_file_loc()
{
	std::string str_config_location;
	// Check if there's a XDG_CONFIG_HOME environment variable
	// that declares a custom config folder
	
	// Make a config_location var as c string, 
	// to be able to catch NULL pointers
	char *config_location = std::getenv("XDG_CONFIG_HOME");
	if (config_location == NULL)
	{
		// If this doesn't exist check for a custom LESSON_CONFIG variable
		// This can be used to set a custom config location
		// (Hey, which of YOUR programs have that)
		config_location = std::getenv("LESSON_CONFIG");
	}
	if (config_location == NULL)
	{
		// Default to the home directory
		config_location = std::getenv("HOME");
		config_location = strcat(config_location, "/.config/lesson-controller/config.conf");
	}
	if (config_location == NULL)
	{
		// Ok, I give up
		queue_error(std::string(C_RED_B) 
			+ "A valid config location could not be determined.\n"
			+ "Please declare either a " 
			+ std::string(C_OFF) + "XDG_CONFIG_HOME"
			+ std::string(C_RED_B) 
			+ ", a\n" + std::string(C_OFF) +  "HOME " 
			+ std::string(C_RED_B) 
			+ "or the specific " + std::string(C_OFF) 
			+ "LESSON_CONFIG " + std::string(C_RED_B) 
			+ "environment variable,\nwith which the config file location can be determined.");
		print_errors();
		exit(8); // 8 is more or less random, should think about useful exit codes
	}
	CONF_FILE_LOC = std::string(config_location);
}

std::string HOME_DIR = std::string(std::getenv("HOME")); // Declaration only internal (for now)

// Convert a tilde in front of a string to the current users home directory
std::string convert_tilde(std::string input_string)
{
	if (strncmp(input_string.c_str(), "~/", 2) == 0)
	{
		input_string.erase(0, 1);
		input_string = HOME_DIR + input_string;
	}
	return input_string;
}

std::string process_color_input(std::string col_str)
{
	// Remove capitalization
	for (int i = 0;i<col_str.size();i++)
	{
		col_str[i] = tolower(col_str[i]);
	}
	 
	if (col_str == "black")
		return C_BLACK;
	else if (col_str == "red")
		return C_RED;
	else if (col_str == "green")
		return C_GREEN;
	else if (col_str == "yellow")
		return C_YELLOW;
	else if (col_str == "green")
		return C_GREEN;
	else if (col_str == "blue")
		return C_BLUE;
	else if (col_str == "purple")
		return C_PURPLE;
	else if (col_str == "cyan")
		return C_CYAN;
	else if (col_str == "white")
		return C_WHITE;
	// I know, this statement doesn't matter
	else if (col_str == "off" || col_str == "none" || col_str == "no_color")
		return C_OFF;
	return C_OFF;
}

int read_config()
{
	std::ifstream conffile(CONF_FILE_LOC);
	if (conffile.is_open())
	{
		char cur_char;
		std::string str_buf;
		std::string var_name;
		std::string content;
		enum read_op{SKIP, READ_VAR, READ_CONTENT};
		read_op op = SKIP;
		while (conffile.get(cur_char))
		{
			///// Read the syntax
			 
			if (cur_char == '#')
			{
				while (cur_char != '\n')
					conffile.get(cur_char);
			}
			 
			if (op == SKIP)
			{
				if (isalpha(cur_char))
				{
					op = READ_VAR;
				}
			}
			 
			if (op == READ_VAR)
			{
				if (cur_char == '=')
				{
					var_name = str_buf;
					str_buf.clear();
					op = READ_CONTENT;
				}
				if (cur_char != ' ' && cur_char != '=' && cur_char != '\n')
					str_buf = str_buf + cur_char;
			}
			 
			if (op == READ_CONTENT)
			{
				if (cur_char == '\n')
				{
					content = str_buf;
					str_buf.clear();
					op = SKIP;
				}
				if (cur_char != ' ' && cur_char != '=' && cur_char != '\n')
					str_buf = str_buf + cur_char;
			}
			
			// Process the variables var_name and content that store the 
			// most recent parse
			if (content != "") // Only write if the content parsing is finished
			{
				if (var_name == "TIME_FILE_LOC")
					TIME_FILE_LOC = convert_tilde(content);
				else if (var_name == "LESSON_FILE_LOC")
					LESSON_FILE_LOC = convert_tilde(content);
				else if (var_name == "time_color")
					time_color = process_color_input(content);
				else if (var_name == "subject_color")
					subject_color = process_color_input(content);
				else if (var_name == "teacher_color")
					teacher_color = process_color_input(content);
				else if (var_name == "room_color")
					room_color = process_color_input(content);
				else if (var_name == "header_color")
					header_color = process_color_input(content);
				else if (var_name == "separator_color")
					separator_color = process_color_input(content);
				content.clear();
			}
		}
		return 0;
	}
	return ERR_NONEXISTENT_FILE;
}
