// This file currently has 3 goals:
// - Process direct arguments
// - Various utilities (also color constants and Stuff)
// - Read the config file
#pragma once
#include <string>
#include <vector>

struct argrules
{
	bool terse = false,
		 count_empties = true,
		 header = true,
		 title = true,
		 color = true;
};
extern struct argrules rules;

// Actual argument processing
extern void process_args(int argc, char *argv[]);

// Gets stuff ing *argv[] for --next/-n argument, calls next() to actually do it
extern int process_next(int &i, int argc, char *argv[]);

// UTILITIES

#define ERROR -666 // General error value
#define ERR_NONEXISTENT_FILE -528
#define ERR_UNEQUAL_BRACKETS -1010
#define ERR_TOO_FEW_BRACKETS -900

// Variables that store how the program should
// respond to an error
// e.g. only write data if write_data == true
extern bool write_data;
extern bool print_timeframe_loc;
extern bool print_lessondata_loc;


void queue_error(std::string message, bool my_write_data = true , std::string my_print_loc = "none");
void print_errors();

// Debug Helper
void print_vecvecpair(std::vector<std::vector<std::pair<int,int>>>);

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

// Takes a C string and a vector of std::strings, gives the position
// of the c string in the vector, ERROR if the vector doesn't contain it
int vecstrcmp (const char* string, std::vector <std::string>);

std::string cap(std::string); // Capitalize the first letter in a string

std::string tabs(int); // Give a number of tabs


// ARGUMENT PROCESSING

int check_timeframe_availability(); // Best name, I know

// Handles all cases and gets the position of a relative lesson from right now
std::pair<int,int> get_rel_lesson(int); 

// Wrapper and show_lessons executer for get_rel_lesson
void rel_lesson(int);

enum match_enum{m_subj, m_teach, m_room, m_none};
struct match
{
	match_enum match_type;
	int day;
	int lesson;
	match( match_enum my_match_type, int my_day, int my_lesson)
	{
		match_type = my_match_type;
		day = my_day;
		lesson = my_lesson;
	}
};
// Search for the next to_skip lesson where subject, teacher or room match the query
match next(int to_skip, std::string query); 

void show_week(int tables_per_row); // Show the whole week

void show_help();

bool query_reset(std::string file, bool ask = true); // Resets the given file
// if ask is true, it asks the user if they want to reset the file before commencing.
// Returns true if the operation was completed, false if not

// CONFIG FILE PARSING

extern std::string CONF_FILE_LOC;

void set_conf_file_loc();

std::string process_color_input(std::string);

void clear_colors(); // Sets all colors to C_OFF

int read_config();

