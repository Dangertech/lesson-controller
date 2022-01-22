// Tracking for the input arguments given
#include <iostream>
#include <vector>
#include <string>
#include <string.h>
#include <fstream>
#include <ctype.h>
#include <climits>
#include <limits>
#include "lesson.h"
#include "timecalc.h"
#include "args.h"

struct argrules rules;

bool begmatch(const char* arg, const char* check)
{
	if (strncmp(arg, check, strlen(check)) == 0)
		return true;
	return false;
}


std::vector <std::string> lswitches = 
{ 
	// Change output properties
	"--terse", 
	"--no-count-empties", 
	"--no-show-header", 
	"--no-show-title", 
	"--no-show-color", 
	 
	 "--help",
	 
	// Filters
	"--next"
};

std::vector <std::string> sswitches = 
{
	"-t", // Terse
	"-c", // No count empties
	"-H", // No show header
	"-T", // No show title
	"-C", // No show color
	"-h", // Show help
	"-n"  // --next
};

 
// Process ALL given arguments
void process_args(int argc, char *argv[])
{
	/* Track inputs that are not switches, 
	 * to determine if the whole week should be shown
	 */
	for (int i = 1; i<argc; i++)
	{
		if (begmatch(argv[i], "--"))
		{
			// Compare to all long switches
			int entry = vecstrcmp(argv[i], lswitches);
			switch (entry)
			{
				case ERROR:
					if (!rules.terse)
						std::cout << "Switch \"" << argv[i] << "\" not recognized." << std::endl;
					break;
				case 0:
					rules.terse = true;
					break;
				case 1:
					rules.count_empties = false;
					break; 
				case 2:
					rules.header = false;
					break;
				case 3:
					rules.title = false;
					break;
				case 4:
					rules.color = false;
					clear_colors();
					break;
				case 5:
					show_help();
					break;
				case 6: 
					process_next(i, argc, argv);
					break;
				default:
					std::cout 
						<< "A painful problem with lesson controller has occured "
						<< "which you are not responsible for. Please report this bug to "
						<< C_GREEN_U
						<< "https://github.com/Dangertech/lesson-controller/issues" 
						<< C_OFF
						<< std::endl;
					break;
			}
			if (entry > 4 && entry != ERROR)
				return;
		}
										   // Avoid relative lessons (-3)
		else if (begmatch(argv[i], "-") && !isdigit(argv[i][1]))
		{
			// Compare to all short switches
			int entry = vecstrcmp(argv[i], sswitches);
			switch(entry)
			{
				case ERROR:
					std::cout << "Switch \"" << argv[i] << "\" not recognized." << std::endl;
				case 0:
					rules.terse = true;
					break;
				case 1:
					rules.count_empties = false;
					break;
				case 2:
					rules.header = false;
					break;
				case 3:
					rules.title = false;
					break;
				case 4:
					rules.color = false;
					clear_colors();
					break;
				case 5:
					show_help();
					return;
					break;
				case 6:
					process_next(i, argc, argv);
					return;
					break;
				default:
					std::cout 
						<< "A painful problem with lesson controller has occured "
						<< "which you are not responsible for. Please report this bug to "
						<< C_GREEN_U
						<< "https://github.com/Dangertech/lesson-controller/issues" 
						<< C_OFF
						<< std::endl;
					break;
			}
		}
		else
		{
			// Some other argument that isn't a switch
			 
			// Check weekday
			char *test_day = argv[i];
			for (int l = 0; l<strlen(argv[i])-1; l++)
				test_day[l] = tolower(test_day[l]);
			if (vecstrcmp(test_day, weekdays) != ERROR)
			{
				show_single_day(vecstrcmp(test_day, weekdays));
				return;
			}
			
			// Check rellesson
			if (begmatch(argv[i], "+") || begmatch(argv[i], "-"))
			{
				rel_lesson(atoi(argv[i]));
				return;
			}
			// Check relday through language
			int relday_check = vecstrcmp(argv[i], {"yesterday", "today", "tomorrow"}) - 1;
			if (relday_check != ERROR-1)
			{
				show_single_day(day + relday_check);
				return;
			}
			 
			if (strcmp(argv[i], "0") == 0)
			{
				rel_lesson(atoi(argv[i]));
				return;
			}
			if (strcmp(argv[i], "help") == 0)
			{
				show_help();
				return;
			}
			if (!rules.terse)
				std::cout << "Argument \"" << argv[i] << "\" not recognized." << std::endl;
		}
	}
	// Didn't return because of a function execution, show week
	show_week(2);
}

int process_next(int &i, int argc, char *argv[])
{
	std::string error_msg = 
		std::string("The \"-n\"/\"--next\" flag requires a subject, teacher or room to look for\n")
		+ "and optionally a number for how many matches to skip;\n Example:\n"
		+ C_GREEN + "lesson --next Physics" + C_OFF + "   => The next physics lesson\n"
		+ C_GREEN + "lesson --next 3 Schmidt" + C_OFF 
		+ " => 3rd lesson with teacher Schmidt from now\n";

	if (i+1 < argc)
	{
		i++;
		// Get next skips
		int next_skips = 0;
		bool isdig = true;
		for (int x = 0; x<strlen(argv[i]); x++)
			if (!isdigit(argv[i][x]))
				isdig = false;
		if (isdig)
			next_skips = atoi(argv[i]);
		else
			next_skips = 1; // Search string was put directly in the next arg
		// Get query
		std::string query;
		if (isdig && i+1 < argc)
		{
			i++;
			query = std::string(argv[i]);
		}
		else if (!isdig)
			query = std::string(argv[i]);
		else
		{
			std::cout << error_msg;
			return ERROR;
		}
		match found = next(next_skips, query);
		if (rules.title)
		{
			if (!rules.terse)
			{
				std::string type;
				switch (found.match_type)
				{
					case 0: type = "subject"; break;
					case 1: type = "teacher"; break;
					case 2: type = "room"; break;
					case 3: type = "NONE!"; break;
					default: type = "ERROR"; break;
				}
				std::cout << "Found " << type << " \"" << query << "\" on " << cap(weekdays[found.day]) 
					<< ", at lesson " << found.lesson + 1 << std::endl;
			}
			else
			{
				std::cout << found.match_type << "/" << found.day+1 << "/" << found.lesson+1 << std::endl;
			}
		}
		show_lessons({ {{found.day, found.lesson}} });
	}
	else
	{
		std::cout << error_msg;
		return ERROR;
	}
	return 0;
}

// Error messages

std::vector < std::string > error_messages;
void queue_error(std::string message, bool my_write_data, std::string my_print_loc)
{
	error_messages.push_back(message);
	if (my_write_data == false)
		write_data = false;

	if (my_print_loc == "timeframes")
		print_timeframe_loc = true;
	else if (my_print_loc == "lessondata")
		print_lessondata_loc = true;
	else if (my_print_loc == "both")
	{
		print_timeframe_loc = true;
		print_lessondata_loc = true;
	}
	else if(my_print_loc != "none")
		std::cerr << "queue_error() didn't receive a valid print_loc argument!";
}

void print_errors()
{
	// Ensure that the function only runs
	// if there are error messages;
	// Avoids segfaults
	if (error_messages.size() == 0 || rules.terse == true)
		return;
	for (int i = 0; i<error_messages.size(); i++)
	{
									// Turn color off again, just in case
		std::cout << error_messages[i] << C_OFF; 
		if (i != error_messages.size()-1)
			std::cout << std::endl << std::endl;
	}
	if (print_lessondata_loc || print_timeframe_loc)
		std::cout << std::endl << std::endl;
	 
	///// Print file locations in case of errors in the files
	if (print_lessondata_loc)
		std::cout << C_RED_B << "Your lessondata file location: " << C_OFF
				  << "[ " << C_GREEN_U << LESSON_FILE_LOC << C_OFF << " ]"
				  << std::endl << std::endl;
	if (print_timeframe_loc)
		std::cout << C_RED_B << "Your timeframe file location: " << C_OFF
				  << "[ " << C_GREEN_U << TIME_FILE_LOC << C_OFF << " ]"
				  << std::endl << std::endl;

	if (write_data == false)
	{
		std::cout << C_GREEN_U << "Notice:" << C_OFF
				  << " No data was rewritten to avoid damaging "
				  << "your data files." << std::endl;
	}
	
	// Clearly separate errors from output
	std::cout << std::endl << std::endl;
	// Now you can clear the errors as they have been shown
	// This allows for calling this function multiple times during
	// the program flow without showing errors twice
	error_messages.clear();
}

void print_vecvecpair(std::vector <std::vector<std::pair<int,int>>> vec)
{
	for (int i = 0; i<vec.size(); i++)
	{
		for (int j = 0; j<vec[i].size(); j++)
		{
			std::cout << vec[i][j].first << ", " << vec[i][j].second << "; ";
		}
		std::cout << std::endl;
	}
}

// There might very well be an inbuilt solution but I don't care enough to google it
int int_length (int integer) // That do be a lot of ints
{
	if (integer < 0 && integer > -10)
		return 2;
	if (integer > 0 && integer < 10)
		return 1;
	if (integer > 9 && integer < 100)
		return 2;
	if (integer > 99 && integer < 1000)
		return 3;
	if (integer > 999 && integer < 10000)
		return 4;
	return ERROR;
}

int vecstrcmp(const char* string, std::vector < std::string > string_vec)
{
	for (int i = 0; i < string_vec.size(); i++)
	{
		if (std::string(string) == string_vec[i])
		{
			return i;
		}
	}
	return ERROR;
}

std::string cap(std::string to_cap)
{
	to_cap[0] = toupper(to_cap[0]);
	return to_cap;
}

std::string tabs(int tab_num)
{
	std::string tabs_to_give;
	for (int i = 0; i<tab_num; i++)
	{
		tabs_to_give += "\t";
	}
	return tabs_to_give;
}


//// ARGUMENTS PROCESSING

int check_timeframe_availability()
{
	if (timeframes.size() == 0)
	{
		queue_error(std::string(C_RED_B)
				+ "There are no timeframes set! Check that your "
				+ "config file points to your timeframe file.\n"
				+ "You can reset the timeframe file with " 
				+ std::string(C_OFF) + "lesson --reset-timeframes "
				+ std::string(C_RED_B) + "\nto learn about how your "
				+ "file should look like.", true, "timeframes");
		print_errors();
		exit(77);
	}
	for (int i = 0; i<weekdays.size(); i++)
	{
		if (table[i].size() == 0)
			continue;
		if (timeframes.size() < table[i].size())
			return -1;
	}
	return 0;
}

std::pair<int,int> get_rel_lesson(int to_skip)
{
	int current_lesson = get_lesson(hour, minute);
	 
	int targeted_lesson;
	if (current_lesson == -1)
		targeted_lesson = 0;
	else if (current_lesson == -2)
	{
		if (to_skip == 0)
			return std::make_pair(-1,-1);
		targeted_lesson = timeframes.size() - 1; 
	}
	else
		targeted_lesson = current_lesson;
	int targeted_day = day; // Day of timecalc.cpp
	 
	if (to_skip >= 0)
	{
		// Increase
		for (int i = 0; i<to_skip; i++)
		{
			targeted_lesson++;
			if (targeted_lesson >= timeframes.size() - 1)
			{
				targeted_day++;
				targeted_lesson = 0;
			}
			if (targeted_day > 6)
				targeted_day = 0;
		}
	}
	else
	{
		// Decrease
		for (int i = 0; i>to_skip; i--)
		{
			targeted_lesson--;
			if (targeted_lesson < 0)
			{
				targeted_day--;
				targeted_lesson = timeframes.size() - 2;
			}
			if (targeted_day < 0)
				targeted_day = 6;
		}
	}
	return std::make_pair(targeted_day, targeted_lesson);
}

void rel_lesson(int to_skip)
{
	std::pair<int,int> table_pos = get_rel_lesson(to_skip); 
	if (table_pos.first == -1 && table_pos.second == -1)
	{
		if (!rules.terse)
			std::cout << "No active lesson registered." << std::endl;
		return;
	}
	if (!rules.terse && rules.title)
		std::cout << "Lesson " << table_pos.second + 1 
			  << " on "
			  << cap(weekdays[table_pos.first]) << ":" << std::endl;
	 
	show_lessons({ {{table_pos.first, table_pos.second}} });
}

match next(int to_skip, std::string query)
{
	int ilesson = get_lesson(hour, minute); // iterator lesson
	int iday = day;
	if (ilesson == -1)
		ilesson = 0;
	else if (ilesson == -2)
		ilesson = table[iday].size() -1;
	int slesson = ilesson; // start lesson for later reference
	bool stop = false;
	bool matched = false;
	while (!stop)
	{
		if (table[iday].size() > ilesson)
		{
			match_enum my_match = m_none;
			if (table[iday][ilesson].subject == query)
				my_match = m_subj;
			if (table[iday][ilesson].teacher == query)
				my_match = m_teach;
			if (table[iday][ilesson].room == query)
				my_match = m_room;
			 
			if (my_match != m_none)
			{
				matched = true;
				to_skip--;
				if (to_skip == 0)
					return match(my_match, iday, ilesson);
			}
		}
		// Increase iterators
		ilesson++;
		if (ilesson >= timeframes.size())
		{
			iday++;
			ilesson = 0;
		}
		if (iday > 6)
		{
			iday = 0;
			ilesson = 0;
		}
		if (ilesson == slesson && iday == day && !matched) // Looped around without a match yet
			return match(m_none, ERROR, ERROR);
	}
	std::cout << "lesson: Could not process --next argument! \
		Please report at: https://github.com/dangertech/lesson-controller/issues!" << std::endl;
	return match(m_none, ERROR, ERROR);
}

void show_week(int tables_per_row)
{
	std::vector <std::vector <std::pair<int,int>> > dayarr;
	int day = 0;
	while (day <= 7)
	{
		/* -1 because else show_lessons would print a lesson
		 * beginning at the end of the last lesson;
		 */
		for (int row = 0; row<timeframes.size()-1; row++)
		{
				dayarr.push_back(std::vector<std::pair<int,int>>());
				for (int cell = day; cell<day+tables_per_row; cell++)
				{
					if (cell >= 7)
						break;
					dayarr[dayarr.size()-1].push_back(std::make_pair(cell, row));
				}
		}
		show_lessons(dayarr);
		dayarr.clear();
		day += tables_per_row;
	}
}

void show_help()
{
	std::cout << "Usage: lesson [SWITCHES...] [FILTER...]" << std::endl
		<< std::endl
		<< "Tool to monitor and manage static timetables, like in school\n"
		<< std::endl
		<< "WARNING: THIS HELP TEXT CONTAINS FEATURES THAT ARE YET TO BE IMPLEMENTED\n"
		<< std::endl
		<< "FILTERS\n"
		<< "A filter can be a weekday, a specific lesson, or a relative lesson\n"
		<< std::endl
		<< "   lesson monday/tuesday, etc...\t"
		<< "show all lessons registered on this weekday\n"
		<< "   lesson +n\t\t\t\t"
		<< "show the lesson n lessons ahead of the current one\n"
		<< "   lesson -n\t\t\t\t"
		<< "show the lesson n lessons back of the current one\n"
		<< "   lesson -d n\t\t\t\t"
		<< "show the day n days from this one; (WIP!)\n"
		<< "\t\t\t\t\t\tcan be negative;\n"
		<< "\t\t\t\t\t\tcan be augmented with a specific lesson number\n"
		<< "\t\t\t\t\t\t(e.g. lesson -d 3 1 => First lesson three days ahead)\n"
		<< "   lesson tomorrow/today/yesterday\t" 
		<< "supported relative weekday displays\n"
		<< "   lesson --next/-n [n] string\t\t"
		<< "shows the next n'th lesson where either subject, teacher or room\n"
		<< "\t\t\t\t\t\tmatch the given string\n"
		<< "\t\t\t\t\t\t(e.g. lesson --next Geography => next Geography lesson\n"
		<< "\t\t\t\t\t\t lesson --next 2 Geography => the Geography lesson \n"
		<< "\t\t\t\t\t\t\tafter the next Geography lesson\n"
		<< "\t\t\t\t\t\t lesson --next 1 333 => next lesson in the room 333)\n"
		<< std::endl
		<< "SWITCHES\n"
		<< std::endl
		<< "   -t/--terse\t\t\t\t"
		<< "makes all output terse, suited for script parsing\n"
		<< "   -c/--no-count-empties\t\t"
		<< "relative arguments do not count lessons that don't happen (WIP!) \n"
		<< "\t\t\t\t\t\tbut the timeframes indicate that there might be one\n"
		<< "   -H/--no-show-header\t\t\t"
		<< "don't display the header of the timetable\n"
		<< "   -T/--no-show-title\t\t\t"
		<< "don't show the title of tables that are shown in some situations\n"
		<< "\t\t\t\t\t\t(e.g. omit the \"--- Weekday ---\" or similar things)\n"
		<< "   -C/--no-show-color\t\t\t"
		<< "don't show the colors set in your config file\n"
		<< "\t\t\t\t\t\t(to disable ALL colors, you have to recompile with 'CFLAGS=NO_COLOR'\n"
		<< "   -h/--help\t\t\t\t"
		<< "show this help text\n"
		<< std::endl
		<< std::endl
		<< std::endl
		<< "WRITING YOUR OWN TIMETABLE\n"
		<< "This section teaches you how to write your own datafile"
		<< " for the program to use.\n"
		<< "Don't worry, it's easy!\n"
		<< std::endl
		<< "1.   Find your config file\n"
		<< "     Your config file is either located at"
		<< " $XDG_CONFIG_HOME/lesson-controller/config.conf,\n"
		<< "     the $LESSON_CONFIG environment variable,"
		<< " or ~/.config/lesson-controller/config.conf\n"
		<< "2.   Find your data location in the config file\n"
		<< "     The variables LESSON_FILE_LOC and TIME_FILE_LOC"
		<< " set the location of your datafiles.\n"
		<< "     If these files don't exist, create them.\n"
		<< "3.   Syntax of your timeframe file:\n"
		<< "     The timeframe file declares when your lessons start.\n"
		<< "     AT THE MOMENT, PAUSES BETWEEN LESSONS ARE UNFORTUNATELY NOT POSSIBLE.\n"
		<< "     Between two braces ({}), there is another set of braces for each"
		<< " timestamp.\n"
		<< "     In this other set, write the time, separated by a double colon, e.g. 7:45\n"
		<< "     The last entry is the END of your last lesson, not the beginning.\n"
		<< "     Example timeframe file:\n"
		<< std::endl
		<< "     { # The minutes your lessons start every day\n"
		<< "     \t{7:45} {8:35} {9:35} {10:25} {11:25} {12:10} {12:55}\n"
		<< "     }\n"
		<< "4.   Syntax of your lessondata file:\n"
		<< "     Similar to the timeframe file, put everything into two braces ({})\n"
		<< "     Then, for every day, starting with sunday, there are seven braces\n"
		<< "     in which you can write the metadata for as many lessons as you have timeframes.\n"
		<< "     The metadata is located in more braces and follows this syntax:\n"
		<< "     {SUBJECT, TEACHER, ROOM}\n"
		<< "     You can mark free lessons with empty braces without commata\n"
		<< "     Example lessondata file:\n"
		<< "     {\n"
		<< "        { # Sunday\n"
		<< "        }\n"
		<< std::endl
		<< "        { # Monday\n"
		<< "            { Maths, Smith, 666 } # Maths, with teacher Smith in room 666\n"
		<< "            {} # An empty lesson\n"
		<< "            { Computer_Science, Britta_Britt, My_favorite_room } # For spaces, use underscores\n"
		<< "                 # Underscores are converted to spaces, underscores themselves are not supported\n"
		<< "        }\n"
		<< "     }\n"
		<< "     You don't need braces for every weekday,\n"
		<< "     however, some issues might arise at the moment.\n"
		<< std::endl
		<< "YOU DON'T NEED TO UNDERSTAND THIS:\n"
		<< "If you installed the program with 'make install', stock/example config and data files\n"
		<< "should have been created under ~/.config/lesson-controller/\n"
		<< "You can also create these stock data files at any time using 'lesson --create' (or 'lesson --reset')\n"
		<< "They will appear at the location given in your config file.\n"
		<< std::endl
		<< "Have fun using lesson-controller, but note that it is still a Work in Progress\n"
		<< "Report any bugs to 'https://github.com/Dangertech/lesson-controller/issues'\n"
		<< "Written by Kjell Nagel, student at STAMA, Bad Kreuznach, Germany\n"
	;
}

//////// CONFIG FILE PARSING

std::string CONF_FILE_LOC;
// Set the CONF_FILE_LOC which references the location of config.conf
void set_conf_file_loc()
{
	std::string str_config_location;
	// Check if there's a XDG_CONFIG_HOME environment variable
	// that declares a custom config folder
	
	// Make a config_location var as c string, 
	// to be able to catch NULL pointers
	char *config_location = std::getenv("XDG_CONFIG_HOME");
	if (config_location == NULL)
	{
		// If this doesn't exist check for a custom LESSON_CONFIG variable
		// This can be used to set a custom config location
		// (Hey, which of YOUR programs have that)
		config_location = std::getenv("LESSON_CONFIG");
	}
	if (config_location == NULL)
	{
		// Default to the home directory
		config_location = std::getenv("HOME");
		config_location = strcat(config_location, "/.config/lesson-controller/config.conf");
	}
	if (config_location == NULL)
	{
		// Ok, I give up
		queue_error(std::string(C_RED_B) 
			+ "A valid config location could not be determined.\n"
			+ "Please declare either a " 
			+ std::string(C_OFF) + "XDG_CONFIG_HOME"
			+ std::string(C_RED_B) 
			+ ", a\n" + std::string(C_OFF) +  "HOME " 
			+ std::string(C_RED_B) 
			+ "or the specific " + std::string(C_OFF) 
			+ "LESSON_CONFIG " + std::string(C_RED_B) 
			+ "environment variable,\nwith which the config file location can be determined.");
		print_errors();
		exit(8); // 8 is more or less random, should think about useful exit codes
	}
	CONF_FILE_LOC = std::string(config_location);
}

std::string HOME_DIR = std::string(std::getenv("HOME")); // Declaration only internal (for now)

// Convert a tilde in front of a string to the current users home directory
std::string convert_tilde(std::string input_string)
{
	if (strncmp(input_string.c_str(), "~/", 2) == 0)
	{
		input_string.erase(0, 1);
		input_string = HOME_DIR + input_string;
	}
	return input_string;
}



// Convert a base sixteen DIGIT to it's base 10 equivalent
int sixteen_to_ten(char ten_str)
{
	ten_str = tolower(ten_str);
	if(isalpha(ten_str))
	{
		if (ten_str < 103)
			return ten_str-87;
		else
			// That's more than an f and not a hex code
			return ERROR;
	}
	else
		return ten_str-48;
}

std::string process_color_input(std::string col_str)
{
	// Remove capitalization
	for (int i = 0;i<col_str.size();i++)
	{
		col_str[i] = tolower(col_str[i]);
	}
	 
	if (col_str == "black")
		return C_BLACK;
	else if (col_str == "red")
		return C_RED;
	else if (col_str == "green")
		return C_GREEN;
	else if (col_str == "yellow")
		return C_YELLOW;
	else if (col_str == "blue")
		return C_BLUE;
	else if (col_str == "purple")
		return C_PURPLE;
	else if (col_str == "cyan")
		return C_CYAN;
	else if (col_str == "white")
		return C_WHITE;
	// I know, this statement doesn't matter
	else if (col_str == "off" || col_str == "none" || col_str == "no_color")
		return C_OFF;
	 
	// Uh shit, User wrote a HEX code (probably, can't force # because of comments), shit's about to get hot
	if (col_str.size() == 6)
	{
		// The HEX value has to be converted to RGB to then plug it into
		// the ANSI escape sequence
		// Maybe there's a inbuilt function, but I don't care
		//
		// sixteen_to_ten() converts As to a 10 and all that, so we can calculate with base 10
		int red = sixteen_to_ten(col_str[0])*16 + sixteen_to_ten(col_str[1]);
		int green = sixteen_to_ten(col_str[2])*16 + sixteen_to_ten(col_str[3]);
		int blue = sixteen_to_ten(col_str[4])*16 + sixteen_to_ten(col_str[5]);
		if (red >= 0 && red <= 255 && green >= 0 && green <= 255 && blue >= 0 && blue <= 255)
			return "\033[38;2;" + std::to_string(red) + ";" + std::to_string(green) + ";" + std::to_string(blue) + "m";
		else
		{
			return C_OFF;
		}
	}
	return C_OFF;
}

void clear_colors()
{
	time_color = C_OFF;
	subject_color = C_OFF;
	teacher_color = C_OFF;
	room_color = C_OFF;
	header_color = C_OFF;
	separator_color = C_OFF;
}

int read_config()
{
	std::ifstream conffile(CONF_FILE_LOC);
	if (conffile.is_open())
	{
		char cur_char;
		std::string str_buf;
		std::string var_name;
		std::string content;
		enum read_op{SKIP, READ_VAR, READ_CONTENT};
		read_op op = SKIP;
		while (conffile.get(cur_char))
		{
			///// Read the syntax
			 
			if (cur_char == '#')
			{
				while (cur_char != '\n')
					conffile.get(cur_char);
			}
			 
			if (op == SKIP)
			{
				if (isalpha(cur_char))
				{
					op = READ_VAR;
				}
			}
			 
			if (op == READ_VAR)
			{
				if (cur_char == '=')
				{
					var_name = str_buf;
					str_buf.clear();
					op = READ_CONTENT;
				}
				if (cur_char != ' ' && cur_char != '=' && cur_char != '\n')
					str_buf = str_buf + cur_char;
			}
			 
			if (op == READ_CONTENT)
			{
				if (cur_char == '\n')
				{
					content = str_buf;
					str_buf.clear();
					op = SKIP;
				}
				if (cur_char != ' ' && cur_char != '=' && cur_char != '\n')
					str_buf = str_buf + cur_char;
			}
			
			// Process the variables var_name and content that store the 
			// most recent parse
			if (content != "") // Only write if the content parsing is finished
			{
				if (var_name == "TIME_FILE_LOC")
					TIME_FILE_LOC = convert_tilde(content);
				else if (var_name == "LESSON_FILE_LOC")
					LESSON_FILE_LOC = convert_tilde(content);
				else if (var_name == "time_color")
					time_color = process_color_input(content);
				else if (var_name == "subject_color")
					subject_color = process_color_input(content);
				else if (var_name == "teacher_color")
					teacher_color = process_color_input(content);
				else if (var_name == "room_color")
					room_color = process_color_input(content);
				else if (var_name == "header_color")
					header_color = process_color_input(content);
				else if (var_name == "separator_color")
					separator_color = process_color_input(content);
				content.clear();
			}
		}
		return 0;
	}
	return ERR_NONEXISTENT_FILE;
}
