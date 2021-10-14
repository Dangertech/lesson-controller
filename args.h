// Functions and variables that control the given arguments
// Also a utility dump, expect everything here that doesn't really fit elsewhere
#pragma once


extern bool terse; // Terse output for script processing (Off by default)
extern bool count_empties; // Should empty lessons in the timetable be counted when parsing? (On by default)


// UTILITIES

#define ERROR -666 // General error value

int int_length(int);

int vecstrcmp (const char* string, std::vector <std::string>);


// ARGUMENT PROCESSING

int check_timeframe_availability(); // Best name, I know

void rel_lesson(int);

void show_week(); // Show the whole week

