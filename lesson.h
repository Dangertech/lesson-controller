// Things that define data for the program to show and lower-level output
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


extern bool title;
void show_single_day(int);

extern bool table_header; // Track if a table header should be shown
void show_lessons(std::vector < std::pair<int, int> >); // Show a range of lessons given in the vector of pairs
// The first item in the pair is the day, the second the lesson

int get_lesson(int, int); // Get the lesson at the given time (int, int)


///// Data reading and writing
extern std::string TIME_FILE_LOC;
extern std::string LESSON_FILE_LOC;

int read_timeframes();
int read_lessondata();
int write_table();
