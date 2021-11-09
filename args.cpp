// Tracking for the input arguments given
#include <iostream>
#include <vector>
#include <string>
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
		std::cout << std::endl << std::endl;
	}
	 
	///// Print file locations in case of errors in the files
	if (print_lessondata_loc)
		std::cout << C_RED_B << "Your lessondata file location:" << C_OFF
				  << "[ " << C_GREEN_U << DIR_PREFIX + LESSON_FILE_LOC << C_OFF << " ]"
				  << std::endl << std::endl;
	if (print_timeframe_loc)
		std::cout << C_RED_B << "Your timeframe file location:" << C_OFF
				  << "[ " << C_GREEN_U << DIR_PREFIX + TIME_FILE_LOC << C_OFF << " ]"
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
		if (string == string_vec[i])
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
	for (int i = 0; i<weekdays.size(); i++)
	{
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
