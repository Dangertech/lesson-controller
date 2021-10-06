// Tracking for the input arguments given
#include <iostream>
#include <vector>
#include <string>
#include "lesson.h"
#include "timecalc.h"

bool terse = false;
bool count_empties = true;

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

void rel_lesson(int to_skip)
{
	int current_lesson = get_lesson(hour, minute);
	 
	int targeted_lesson;
	if (current_lesson == -1)
		targeted_lesson = 0;
	else if (current_lesson == -2)
		targeted_lesson = timeframes.size() - 2; 
	else
		targeted_lesson = current_lesson;
	int targeted_day = day; // Day of timecalc.cpp
	 
	if (to_skip >= 0)
	{
		// Increase
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
	 
	show_lesson(targeted_day, targeted_lesson);
}
