// Tracking for the input arguments given
#include <iostream>
#include <vector>
#include <string>
#include "lesson.h"
#include "timecalc.h"
#include "args.h"


bool terse = false;
bool count_empties = true;


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



//// ARGUMENTS PROCESSING

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
			  << weekdays[targeted_day] << ":" << std::endl;
	 
	show_lessons({ {targeted_day, targeted_lesson } });
}

void show_week()
{
	std::cout << "Ima show you the week!" << std::endl;
}