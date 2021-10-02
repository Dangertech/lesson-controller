#include <vector>
#include <iostream>
#include "lesson.h"


std::vector < std::vector < struct lesson > > table = 
{ 
	{ // Sunday
	},
	 
	{ // Monday
		{"English", "Ehlers", "333"}, 
		{"French", "Lopez", "333"},
		{"History", "Scherbel", "333"}
	},
	 
	{ // Tuesday
		{"Maths", "Bärbel", "333"},
		{"Maths", "Bärbel", "333"},
		{"English", "Ehlers", "333"}
	},
	 
	{ // Wednesday
	},
	 
	{ // Thursday
	},
	 
	{ // Friday
	},
	 
	{ // Saturday
	} 
};

std::vector < std::pair<int, int> > timeframes = { {7, 45}, {8, 35}, {9, 35}, {10, 25}, {11, 25}, {12, 10}, {12, 55} };

void show_lesson(int day, int lesson)
{
	// Can easily be expanded to only show specific data or make output terse
	//
	// Show time
	std::cout << "[";
	if (timeframes[lesson].first != 0) // Check to warn if the timeframe doesn't exist
		std::cout << timeframes[lesson].first;
	else
		std::cout << "\033[1;31m" << "!" << "\033[0m";
	std::cout << ":";
	if (timeframes[lesson].second != 0)
		std::cout << timeframes[lesson].second;
	else
		std::cout << "\033[1;31m" << "!" << "\033[0m";
	std::cout << "] "; 
	std::cout << table[day][lesson].subject;
	std::cout << " ";
	std::cout << table[day][lesson].teacher;
	std::cout << " ";
	std::cout << table[day][lesson].room;
	std::cout << std::endl;
}
