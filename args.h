// Functions and variables that control the given arguments
// Also a utility dump, expect everything here that doesn't really fit elsewhere
#pragma once
#include <string>
#include <vector>


extern bool terse; // Terse output for script processing (Off by default)
extern bool count_empties; // Should empty lessons in the timetable be counted when parsing? (On by default)


// UTILITIES

#define ERROR -666 // General error value
#define ERR_NONEXISTENT_FILE -528
#define ERR_UNEQUAL_BRACKETS -1010
#define ERR_TOO_FEW_BRACKETS -900

#define C_RED_B		"\033[1;31m"
#define C_GREEN_U	"\033[4;32m"
#define C_BLUE		"\033[34m"
#define C_BLUE_U	"\033[4;34m"
#define C_OFF		"\033[0m"

int int_length(int);

int vecstrcmp (const char* string, std::vector <std::string>);

std::string cap(std::string); // Capitalize the first letter in a string

std::string tabs(int); // Give a number of tabs

// ARGUMENT PROCESSING

int check_timeframe_availability(); // Best name, I know

void rel_lesson(int);

void show_week(); // Show the whole week

