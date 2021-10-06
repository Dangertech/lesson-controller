#pragma once
#include <vector>
#include <string>

struct lesson // Struct storing data of a lesson
{
	std::string subject;
	std::string teacher;
	std::string room;
};

extern std::vector < std::pair<int, int> > timeframes; // The times at which lessons start

extern std::vector < std::vector < struct lesson > > table; // The timetable, a vector of a vector of lessons

void show_single_day(int);

void show_lesson(int, int);

int get_lesson(int, int); // Get the lesson at the given time (int, int)
