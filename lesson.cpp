#include <vector>
#include <iostream>
#include <cmath>
#include <fstream>
#include "lesson.h"
#include "timecalc.h"
#include "args.h"



std::vector < std::vector < struct lesson > > table;
std::vector < std::pair<int, int> > timeframes;

std::string
	time_color = C_OFF,
	subject_color = C_OFF,
	teacher_color = C_OFF,
	room_color = C_OFF,
	header_color = C_BLUE,
	separator_color = C_BLUE;

////// Show a day with some prettifications around it
bool title = true; // Track if the title (e.g. --- Thursday ---) should be shown
void show_single_day (int my_day)
{
	if (terse == false && title) // Show the requested weekday in a fancy format
	{
		std::string my_weekday = weekdays[my_day];
		my_weekday[0] = toupper(my_weekday[0]);
		std::cout << "--- " << my_weekday << " ---" << std::endl;
	}
	 
	if (table[my_day].size() == 0)
	{
		if (terse == false)
			std::cout << "No lessons registered" << std::endl;
		return;
	}
	 
	std::vector < std::pair <int,int> > day_vector;
	for (int i = 0; i<table[my_day].size(); i++)
	{
		day_vector.push_back( std::make_pair(my_day, i) );
		if (i < table[my_day].size() -1)
			day_vector.push_back( std::make_pair(-1, -1) );
	}
	show_lessons(day_vector);
}




///// All this leads up to show_lessons
//
int spacing = 1;
int time_size = 8, subj_size, teach_size, room_size;
int x_pos;


void print_chars(const char* my_char, int num_of_spaces) // [Internal only]
{
	for (int i = 0; i<num_of_spaces; i++)
	{
		std::cout << my_char;
	}
}

void terse_space(int custom_spaces) // [Internal only], used for show_lessons
{
	if (terse == false)
	{
		// Make spaces according to the input argument given
		print_chars(" ", custom_spaces);
		x_pos += custom_spaces;
		std::cout << separator_color << "|" << C_OFF;
		x_pos++;
		// Make spacing spaces
		print_chars(" ",spacing);
		x_pos += spacing;
	}
	else
		std::cout << "/";
}

void print_header()
{
	x_pos = 0;
	// Store the x position where the table headers should start
	// Just to make my head hurt less
	int des_pos; 
	
	// Print in header_color from config file
	std::cout << header_color;
	 
	std::cout << "=TIME";
	x_pos += 5;
	 
	des_pos = time_size+spacing;
	 
	print_chars("=", des_pos-x_pos-1);
	x_pos += des_pos-x_pos-1;
	 
	std::cout << "SUBJECT";
	x_pos += 7;
	
	des_pos = time_size+spacing+subj_size+spacing+1+spacing;
	 
	print_chars("=", des_pos - x_pos - 1);
	x_pos += des_pos - x_pos - 1;
	 
	std::cout << "TEACHER";
	x_pos += 7;
	 
	des_pos += teach_size + spacing + 1 + spacing;
	 
	print_chars("=", des_pos - x_pos - 1);
	x_pos += des_pos - x_pos - 1;
	 
	std::cout << "ROOM";
	x_pos += 4;
	 
	des_pos += room_size;
	print_chars("=", des_pos - x_pos - 1);
	 
	std::cout << std::endl;
	x_pos = 0;
	
	std::cout << C_OFF;
}


bool table_header = true;

// Show data for any lessons
// Takes a vector of pairs with days and lesson numbers
// Prints a table to stdout with the given lessons
void show_lessons(std::vector < std::pair<int, int> > to_show)
{
	x_pos = 0; // Reset X Position in case the user queries another day; Without this,
		   // the spacing of the first row would be messed up
	// Get how big the table cells should be
	for (int i = 0; i < to_show.size(); i++)
	{
		int day = to_show[i].first;
		int lesson = to_show[i].second;
		if (day == -1 || lesson == -1)
			continue;
		if (table.size() > day && table[day].size() > lesson)
		{
			if (subj_size < table[day][lesson].subject.length())
				subj_size = table[day][lesson].subject.length();
			if (teach_size < table[day][lesson].teacher.length())
				teach_size = table[day][lesson].teacher.length();
			if (room_size < table[day][lesson].room.length())
				room_size = table[day][lesson].room.length();
		}
		// Ugly but works just fine for only three variables
	}
	 
	bool construct = true;
	for (int i = 0; i < to_show.size(); i++)
	{
		 
		int day = to_show[i].first;
		int lesson = to_show[i].second;
		 
		if (day == -1 && lesson == -1)
		{
			std::cout << std::endl;
			construct = false;
			x_pos = 0;
			continue;
		}
		 
		 
		if (day < table.size() && lesson < table[day].size()) // Check to avoid crashes
		{
			// Make the header of the table (only if terse is false of course)
			if (construct && !terse && table_header)
			{
				print_header();
			}
			 
			// Show time
			if (terse == false)
			{
				std::cout << time_color; 
				std::cout << "[";
				x_pos++;
			}
			if (timeframes[lesson].first != 0) // Check to warn if the timeframe doesn't exist
			{
				std::cout << timeframes[lesson].first;
				x_pos += int_length(timeframes[lesson].first);
			}
			else
			{
				std::cout << C_RED_B << "!" << C_OFF << time_color;
				x_pos++;
			}
			 
			std::cout << ":";
			x_pos++;
			if (timeframes[lesson].second != 0)
			{
				std::cout << timeframes[lesson].second;
				x_pos += int_length(timeframes[lesson].second);
			}
			else
			{
				std::cout << C_RED_B << "!" << C_OFF << time_color;
				x_pos++;
			}
			 
			if (terse == false)
			{
				std::cout << "]"; 
				x_pos++;
				for (int spaces = 0; spaces < time_size+spacing-x_pos; spaces++)
				{
					std::cout << " ";
					x_pos++;
				}
			}
			else
			{
				std::cout << "/";
				x_pos++;
			}
			// Time end 
			std::cout << C_OFF; 
			 
			if (terse == false)
				std::cout << subject_color;
			std::cout << table[day][lesson].subject << C_OFF;
			x_pos += table[day][lesson].subject.size();
			terse_space((time_size+subj_size+spacing) - x_pos);
			 
			if (terse == false)
				std::cout << teacher_color;
			std::cout << table[day][lesson].teacher << C_OFF;
			x_pos += table[day][lesson].teacher.size();
			// 3* spacing bcs the spacing was applied three times up to here
			// +1 bcs there was an additional | character
			// I hate this calculation too, by the way
			terse_space((time_size+subj_size+teach_size+spacing*3+1) - x_pos);
			 
			if (terse == false)
				std::cout << room_color;
			std::cout << table[day][lesson].room << C_OFF;
			x_pos += table[day][lesson].room.size();
			 
			// Enforce break after the last lesson is printed
			if (i == to_show.size() - 1)
				std::cout << std::endl; 
		}
		else if (!terse)
		{
			std::cout << "Lesson " << lesson+1 << " on weekday " 
				  <<  cap(weekdays[day]) << " does not exist!" << std::endl;
		}
	}
}

int get_lesson(int c_hour, int c_minute) // Get the current lesson
{
	// Converts time to minutes in this day
	// Maybe timeframes should be like that out of the box?
	std::vector < int > conv_frames;
	int current_minute = c_hour * 60 + c_minute;
	for (int i=0; i<timeframes.size(); i++) // Create array of converted timeframes
		conv_frames.push_back(timeframes[i].first * 60 + timeframes[i].second);
	for (int i=0; i<conv_frames.size(); i++)
	{
		if (current_minute >= conv_frames[i] && current_minute <= conv_frames[i+1])
			return i;
	}
	 
	if (current_minute < conv_frames[0])
		return -1; // Before today's lessons
	else
		return -2; // After today's lessons
}


/////////////// Data reading and writing

bool write_data = true;
bool print_timeframe_loc = false;
bool print_lessondata_loc = false;

std::string TIME_FILE_LOC;
std::string LESSON_FILE_LOC;

int validate_datafile(std::string file_loc, int minimum_brackets)
{
	std::ifstream file(file_loc);
	char cur_char;
	int open_brackets = 0;
	int closed_brackets = 0;
	if (file.is_open())
	{
		while ( file.get(cur_char) )
		{
			if (cur_char == '{')
				open_brackets++;
			else if (cur_char == '}')
				closed_brackets++;
		}
		if (open_brackets != closed_brackets)
			return ERR_UNEQUAL_BRACKETS;
		if (open_brackets < minimum_brackets)
			return ERR_TOO_FEW_BRACKETS;
		file.close();
		return 0;
	}
	return ERR_NONEXISTENT_FILE;
}

// Convert a vector of digits (e.g. {4, 5}) to an integer (e.g. 45)
int vectoint(std::vector <int> vector)
{
	int vecint = 0;
	for (int i=0; i<vector.size(); i++)
	{
		vecint += vector[vector.size()-1-i]*pow(10, i);
	}
	return vecint;
}

int read_timeframes()
{
	// Space - and linebreak agnostic function that reads in the timeframes
	 
	// Pre-validation
	int file_validity = validate_datafile(TIME_FILE_LOC, 2);
	if (file_validity == ERR_UNEQUAL_BRACKETS)
	{
		queue_error(std::string(C_RED_B) + "There are not as many open brackets"
					+ "as closed brackets in your timeframe file. \n"
					+ C_OFF, false, "timeframes");
	
	}
	else if(file_validity == ERR_TOO_FEW_BRACKETS)
	{
		queue_error(std::string(C_RED_B) + "You have too few brackets in your "
					+ "timeframe file for it to be valid" + std::string(C_OFF),
					true, "timeframes");
	}
	else if(file_validity == ERR_NONEXISTENT_FILE)
		return ERR_NONEXISTENT_FILE;
	 
	// Read the data
	std::ifstream timefile(TIME_FILE_LOC);
	if (timefile.is_open())
	{
		char cur_char;
		int brackets = 0; // Stores, on which 'indentation level' the parser is.
		std::vector < int > num_buf; // Buffer to store parsed digits temporarily
		enum read_op { SKIP, READ_HOUR, READ_MINUTE };
		read_op op = SKIP;
		bool invalid_int_warn = false;
		while ( timefile.get(cur_char) )
		{
			if ( cur_char == ' ' || cur_char == '\n')
				continue;
			 
			if ( cur_char == '#' )
			{
				while ( cur_char != '\n')
					timefile.get(cur_char);
			}
			
			if (cur_char == '{')
				brackets++;
			if (cur_char == '}')
				brackets--;
			 
			if (brackets == 2 && cur_char == '{')
			{
				// Store the operation that will be carried 
				// out over the following loops
				op = READ_HOUR;
				timeframes.push_back( std::make_pair(0,0) );
				continue; // Don't read in the current character
			}
			if (brackets == 2 && cur_char == ':')
			{
				op = READ_MINUTE;
				// Just switched from the hour, 
				// so get that and clear the buffer
				int framehour = vectoint(num_buf);
				//std::cout << framehour << std::endl;
				timeframes[timeframes.size()-1].first = framehour;
				num_buf.clear();
				continue;
			}
			if (brackets == 1 && cur_char == '}')
			{
				op = SKIP;
				// Just switched from minute 
				int framemin = vectoint(num_buf);
				//std::cout << framemin << std::endl;
				timeframes[timeframes.size()-1].second = framemin;
				num_buf.clear();
			}
			 
			 
			 
			int asint = cur_char - '0';
			switch (op)
			{
				case SKIP:
					break;
				case READ_MINUTE:
					if (asint >= 0 && asint <= 9)
						num_buf.push_back(asint);
					else
						invalid_int_warn = true;
					//std::cout << "\tMinutestore: " << asint << std::endl;
					break;
				case READ_HOUR:
					if (asint >= 0 && asint <= 9)
						num_buf.push_back(asint);
					else
						invalid_int_warn = true;
					//std::cout << "\tHourstore: " << asint << std::endl;
					break;
			}
			
		}
		 
		if (invalid_int_warn)
		{
			queue_error(std::string(C_RED_B) + "Your timeframe file contains "
						+ "characters in the time location that are not numbers.\n"
						+ std::string(C_OFF) + "Advice:\n\t- Edit your timeframe file\n"
						+ "\t- Issue 'lesson --reset-timeframes' (Your data will be lost)",
						false, "timeframes");
		}
		timefile.close();
		return 0;
	}
	return ERR_NONEXISTENT_FILE;
}

// Converts a vector of chars to a string
std::string vectostr(std::vector < char > vector)
{
	std::string final_str;
	for (int i = 0; i<vector.size(); i++)
	{
		final_str.push_back(vector[i]);
	}
	return final_str;
}

int read_lessondata()
{
	// Function to read in the lessondata
	int file_validity = validate_datafile(LESSON_FILE_LOC, 16);
	if (file_validity == ERR_UNEQUAL_BRACKETS)
	{
		queue_error(std::string(C_RED_B) + "There are not as many "
				+ "open brackets as closed brackets in your lessondata "
				+ "file. Please edit your lessondata file to fix this issue.",
				false, "lessondata");
	}
	// Read the data
	std::ifstream lessonfile(LESSON_FILE_LOC);
	if (lessonfile.is_open())
	{
		char cur_char;
		int brackets = 0;
		std::vector < char > str_buf;
		enum read_op { SKIP, READ_SUBJECT, READ_TEACHER, READ_ROOM};
		read_op op = SKIP;
		while ( lessonfile.get(cur_char) )
		{
			if ( cur_char == '#' )
			{
				while ( cur_char != '\n')
					lessonfile.get(cur_char);
			}
			if (cur_char == ' ' || cur_char == '\n')
				continue;
			 
			if (cur_char == '{')
				brackets++;
			if (cur_char == '}')
				brackets--;
			
			// Mode setter and Data writer to the vector
			if (brackets == 2 && cur_char == '{')
				table.push_back(std::vector<struct lesson>());
			if (brackets == 3 && cur_char == '{')
			{
				op = READ_SUBJECT;
				table[table.size() -1].push_back(lesson());
				continue;
			}
			if (brackets == 3 && cur_char == ',')
			{
				switch(op)
				{
					case READ_SUBJECT:
						op = READ_TEACHER; 
						// Why am I like this.
						// Could I make a variable for that with pointers,
						// but I am just too stupid?
						table[table.size()-1][table[table.size() -1].size() -1].subject = vectostr(str_buf);
						str_buf.clear();
						continue;
					case READ_TEACHER:
						op = READ_ROOM; 
						table[table.size()-1][table[table.size() -1].size() -1].teacher = vectostr(str_buf);
						str_buf.clear();
						continue;
					default:
						// Config file fucked up somehow, should send a message
						break;
				}
			}
			if (brackets == 2 && cur_char == '}')
			{
				op = SKIP;
				table[table.size()-1][table[table.size()-1].size()-1].room = vectostr(str_buf);
				str_buf.clear();
				continue;
			}
			 
			// Data reader
			switch (op)
			{
				case SKIP:
					break;
				case READ_SUBJECT: case READ_TEACHER: case READ_ROOM:
					str_buf.push_back(cur_char);
					break;
			}
			 
		}
		lessonfile.close();
		return 0;
	}
	return ERR_NONEXISTENT_FILE;
}

int write_table()
{
	// This function should create a human readable file with the table data in it
	
	// The file with the timestamps
	std::ofstream timefile(TIME_FILE_LOC);
	 
	///// Writing the timeframes
	 
	timefile << "{ # The minutes your lessons start every day" << std::endl;
	timefile << tabs(1);
	for (int stamp = 0; stamp < timeframes.size(); stamp++)
	{
		timefile << "{"
			<< timeframes[stamp].first << ":"
			<< timeframes[stamp].second << "} ";
	}
	timefile << std::endl << "}" << std::endl << std::endl;
	timefile.close();
	 
	 
	 
	///// Writing the timetable
	 
	std::ofstream lessonfile(LESSON_FILE_LOC); // Open file stream
	
	///// Writing a little help text
	lessonfile << "# This is the location where the data about your lessons is stored." << std::endl
		<< "# It is designed to be human readable so that you can easily edit it," << std::endl
		<< "# but note that in the future, there are plans to have arguments that let you edit your lessons with lesson-controller itself." << std::endl
		<< "# " << std::endl
		<< "# Lesson-controller is space- and linebreak-agnostic while reading this and the timedata.dat file, so you can format it pretty much however you want, BUT:" << std::endl
		<< "# The file is reset every time the program performs a write operation (Right now, at every successful termination)" << std::endl
		<< "# You have to enclose every day in curly brackets and every lesson in every day as well." << std::endl
		<< "# The 3 Metadata that are stored (Subject, Teacher and Room) must be separated by commata." << std::endl
		<< "# Right now, you see the stock configuration. To reset to it when you messed up, run: 'lesson --reset-data'. All your data will be lost." << std::endl;
	 
	lessonfile << "{ # Your timetable" << std::endl;
	lessonfile << tabs(1) << " " << std::endl;
	for (int iday = 0; iday<table.size(); iday++)
	{
		lessonfile << tabs(1) << "{" << " # " << cap(weekdays[iday]) <<  std::endl; // '# Weekday'
		if (table[iday].size() > 0)
		{
			for (int ilesson = 0; ilesson<table[iday].size(); ilesson++)
			{
				lessonfile << tabs(2) << "{ ";
				lessonfile << table[iday][ilesson].subject << ", "
					<< table[iday][ilesson].teacher << ", "
					<< table[iday][ilesson].room;
				lessonfile << " }" << std::endl;
			}
		}
		else
			lessonfile << tabs(2) << " " << std::endl;
		lessonfile << tabs(1) << "}" << std::endl
			<< tabs(1) << std::endl;
	}
	lessonfile << "}" << std::endl;
	lessonfile.close();
	return 0;
}
