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

void show_lesson(int day, int lesson)
{
	// Can easily be expanded to only show specific data or make output terse
	std::cout << table[day][lesson].subject;
	std::cout << " ";
	std::cout << table[day][lesson].teacher;
	std::cout << " ";
	std::cout << table[day][lesson].room;
	std::cout << std::endl;
}
