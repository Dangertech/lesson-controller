// This file currently has 3 goals:
// - Various utilities (also color constants and Stuff)
// - Process direct arguments
// - Read the config file
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

// Variables that store how the program should
// respond to an error
extern bool write_data;
extern bool print_timeframe_loc;
extern bool print_lessondata_loc;


void queue_error(std::string message, bool my_write_data = true , std::string my_print_loc = "none");
void print_errors();

#ifndef NO_COLOR
	#define C_BLACK		"\033[30m"
	#define C_RED		"\033[31m"
	#define C_RED_B		"\033[1;31m"
	#define C_GREEN		"\033[32m"
	#define C_GREEN_U	"\033[4;32m"
	#define C_YELLOW	"\033[33m"
	#define C_BLUE		"\033[34m"
	#define C_BLUE_U	"\033[4;34m"
	#define C_PURPLE	"\033[35m"
	#define C_CYAN		"\033[36m"
	#define C_WHITE		"\033[37m"
	#define C_OFF		"\033[0m"
// Compile without color support
#else
	#define C_BLACK		""
	#define C_RED		""
	#define C_RED_B		""
	#define C_GREEN		""
	#define C_GREEN_U	""
	#define C_YELLOW	""
	#define C_BLUE		""
	#define C_BLUE_U	""
	#define C_PURPLE	""
	#define C_CYAN		""
	#define C_WHITE		""
	#define C_OFF		""
#endif

int int_length(int);

int vecstrcmp (const char* string, std::vector <std::string>);

std::string cap(std::string); // Capitalize the first letter in a string

std::string tabs(int); // Give a number of tabs


// ARGUMENT PROCESSING

int check_timeframe_availability(); // Best name, I know

void rel_lesson(int);

void show_week(); // Show the whole week


// CONFIG FILE PARSING

extern std::string CONF_FILE_LOC;

void set_conf_file_loc();

std::string process_color_input(std::string);

void clear_colors(); // Sets all colors to C_OFF

int read_config();

