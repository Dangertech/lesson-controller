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
	 
	std::vector < std::vector <std::pair <int,int> >> day_vector;
	day_vector.push_back(std::vector <std::pair<int,int>>());
	for (int i = 0; i<table[my_day].size(); i++)
	{
		day_vector[0].push_back( std::make_pair(my_day, i) );
	}
	show_lessons(day_vector);
}

// STUFF FOR SHOW_LESSONS

bool table_header = true;
int spacing = 1;

int get_largest_row(std::vector < std::vector <std::pair<int,int>> > vec)
{
	int largest_sub = 0;
	for (int i = 0; i<vec.size(); i++)
	{
		if (vec[i].size() > largest_sub)
			largest_sub = vec[i].size();
	}
	return largest_sub;
}

// Computes a string for a header with the given content metadata
std::string construct_table_header(int subj_size, int teach_size, int room_size)
{
	std::string header;
	header += header_color;
	header += "=TIME=";
	for (int i = 0; i<spacing+1+spacing; i++) // ===SUBJECT
		header += "=";                        //  | English
	header += "SUBJECT";
	// Fill up until TEACHER
	for (int i = header.size(); i<5+spacing+1+spacing+subj_size+spacing+1+spacing; i++)
		header += "=";
	header += "TEACHER";
	// Fill up until ROOM
	for (int i = header.size(); i<7+spacing*4+subj_size+teach_size+spacing+1+spacing; i++)
		header += "=";
	header += "ROOM";
	// If the room name is long, extend the header
	for (int i = header.size(); i<8+spacing*6+subj_size+teach_size+room_size+1; i++)
		header += "=";
	header += C_OFF;
	return header;
}

void show_lessons(std::vector < std::vector <std::pair<int, int> >> to_show)
{
	// If it's only a single lesson, you can show a sophisticated info
	if (to_show.size() == 1 && to_show[0].size() == 1 && !terse)
	{
		int day = to_show[0][0].first, lesson = to_show[0][0].second;
		if (day > table.size() 
				|| lesson >= table[day].size())
		{
			std::cout << "Lesson " << lesson+1 << " on " 
				<< cap(weekdays[day]) << " does not exist " << std::endl;
			return;
		}
	}
	// GET NEEDED MAXIMUM SIZES
	int m_subj_size = 0, m_teach_size = 0, m_room_size = 0;
	for (int i = 0; i<to_show.size(); i++)
	{
		for (int j = 0; j<to_show[i].size(); j++)
		{
			// Avoid crash in case function is called incorrectly
			if (to_show[i][j].second < table[to_show[i][j].first].size())
			{
				lesson comp = table[to_show[i][j].first][to_show[i][j].second];
				 
				if (comp.subject.size() > m_subj_size)
					m_subj_size = comp.subject.size();
				if (comp.teacher.size() > m_teach_size)
					m_teach_size = comp.teacher.size();
				if (comp.room.size() > m_room_size)
					m_room_size = comp.room.size();
			}
		}
	}
	/*
	std::cout << "Max subject size: " << m_subj_size << std::endl
		<< "Max teacher size: " << m_teach_size << std::endl
		<< "Max room size: " << m_room_size << std::endl;
	*/
	 
	// PRINT TABLE HEADERS
	for (int i = 0; i<get_largest_row(to_show); i++)
	{
		std::cout << construct_table_header(m_subj_size, m_teach_size, m_room_size) << " ";
	}
	std::cout << std::endl;
	 
	// PRINT GIVEN ENTRIES
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
					if (cur_char != '_')
						str_buf.push_back(cur_char);
					else // Convert underscores to spaces
						str_buf.push_back(' ');
					break;
			}
			 
		}
		// Push back the remaining days
		// if there are not all brackets for all days in the file
		// Fixes a LOT of weird things while reading the data
		for (int i=0; i<(table.size()-7); i++)
			table.push_back(std::vector<struct lesson>());
		lessonfile.close();
		return 0;
	}
	return ERR_NONEXISTENT_FILE;
}

// Converts converted special characters back to their
// original state in the file
std::string convert_specials (std::string to_convert)
{
	for (int i = 0; i<to_convert.size(); i++)
	{
		if (to_convert[i] == ' ')
			to_convert[i] = '_';
	}
	return to_convert;
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
		<< "# To reset this file when you messed up, run: 'lesson --reset-data'. All your data will be lost." << std::endl
		<< "# This also gives a comprehensive tutorial about this file." << std::endl;
	 
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
				lessonfile << convert_specials(table[iday][ilesson].subject) << ", "
					<< convert_specials(table[iday][ilesson].teacher) << ", "
					<< convert_specials(table[iday][ilesson].room);
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
