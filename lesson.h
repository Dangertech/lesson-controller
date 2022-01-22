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

///// Colors for the timetable shown
// The values here are the defaults
// They are meant to be set by the config file
extern std::string 
	   time_color,
	   subject_color,
	   teacher_color,
	   room_color,
	   header_color,
	   separator_color;

void show_single_day(int);

extern int spacing; // The spacing between the columns
void show_lessons(std::vector < std::vector <std::pair<int, int>> >); // Show a range of lessons given in the vector of pairs
// The first item in the pair is the day, the second the lesson number (e.g. 0 for the first lesson, 3 for the fourth, etc)

int get_lesson(int, int); // Get the lesson number at the given time (hour, minute)


///// Data reading and writing
extern std::string TIME_FILE_LOC;
extern std::string LESSON_FILE_LOC;

int read_timeframes();
int read_lessondata();
int write_table();
