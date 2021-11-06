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

void show_single_day(int);

void show_lessons(std::vector < std::pair<int, int> >); // Show a range of lessons given in the vector of pairs
// The first item in the pair is the day, the second the lesson

int get_lesson(int, int); // Get the lesson at the given time (int, int)


///// Data reading and writing

// main() only writes data if this is set to true
extern bool write_data;
extern bool print_timeframe_loc;
extern bool print_lessondata_loc;

extern const std::string DIR_PREFIX;
extern const std::string TIME_FILE_LOC;
extern const std::string LESSON_FILE_LOC;

int read_timeframes();
int read_lessondata();
int write_table();
