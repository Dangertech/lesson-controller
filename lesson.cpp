#include <vector>
#include <iostream>
#include <fstream>
#include "lesson.h"
#include "timecalc.h"
#include "args.h"


std::vector < std::vector < struct lesson > > table =
{ 
	{ // Sunday
	},
	 
	{ // Monday
		{"English", "Ehlers", "333"}, 
		{"French", "Lopez", "333"},
		{"History", "Scherbel", "333"},
		{"Biology", "Schwarz", "Bio2-223"},
		{"Ethics", "Scherbel", "334"},
		{"Latin", "Scheib", "333"},
	},
	 
	{ // Tuesday
		{"Maths", "Baerbel", "333"},
		{"Maths", "Baerbel", "333"},
		{"English", "Ehlers", "333"},
		{"Biology", "Schwarz", "Bio1-221"},
		{"Music", "Theisen", "Mu1"},
		{"German", "Hegemann", "333"},
	},
	 
	{ // Wednesday
		{"German", "Hegemann", "333"},
		{"Latin", "Scheib", "333"},
		{"French", "Lopez", "333"},
		{"Physics", "Eschborn", "Ph2-213"},
		{"Chemistry", "Bender", "Ch2-216"},
		{"Geography", "Freistein-Vogt", "333"}
	},
	 
	{ // Thursday
		{"Physics", "Eschborn", "Ph2-213"},
		{"Maths", "Baerbel", "333"},
		{"Physical Education", "Diehl", "Ha1"},
		{"Physical Education", "Diehl", "Ha1"},
		{"Geography", "Freistein-Vogt", "333"},
		{"English", "Ehlers", "333"}
	},
	 
	{ // Friday
		{"Ethics", "Scherbel", "334"},
		{"German", "Hegemann", "333"},
		{"Latin", "Scheib", "333"},
		{"French", "Lopez", "333"},
		{"Chemistry", "Ch2-216", "333"},
	},
	 
	{ // Saturday
	} 
};

std::vector < std::pair<int, int> > timeframes; //{ {7, 45}, {8, 35}, {9, 35}, {10, 25}, {11, 25}, {12, 10}, {12, 55} };

void show_single_day (int my_day)
{
	if (terse == false) // Show the requested weekday in a fancy format
	{
		std::string my_weekday = weekdays[my_day];
		my_weekday[0] = toupper(my_weekday[0]);
		std::cout << "--- " << my_weekday << " ---" << std::endl;
	}
	if (table[my_day].size() == 0 && terse == false)
		std::cout << "No lessons registered" << std::endl;
	 
	std::vector < std::pair <int,int> > day_vector;
	for (int i = 0; i<table[my_day].size(); i++)
	{
		day_vector.push_back( std::make_pair(my_day, i) );
		if (i < table[my_day].size() -1)
			day_vector.push_back( std::make_pair(-1, -1) );
	}
	show_lessons(day_vector);
}




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
		std::cout << C_BLUE << "|" << C_OFF;
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
	
	// Blue color underlined
	std::cout << C_BLUE_U;
	 
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
		if (subj_size < table[day][lesson].subject.length())
			subj_size = table[day][lesson].subject.length();
		if (teach_size < table[day][lesson].teacher.length())
			teach_size = table[day][lesson].teacher.length();
		if (room_size < table[day][lesson].room.length())
			room_size = table[day][lesson].room.length();
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
		 
		 
		 
		if (to_show[i].first < table.size() && to_show[i].second < table[day].size()) // Check to avoid crashes
		{
			// Make the header of the table (only if terse is false of course)
			if (construct && !terse)
			{
				print_header();
			}
			 
			 
			// Show time
			if (terse == false)
			{
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
				std::cout << C_RED_B << "!" << C_OFF;
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
				std::cout << C_RED_B << "!" << C_OFF;
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
			 
			 
			std::cout << table[day][lesson].subject;
			x_pos += table[day][lesson].subject.size();
			terse_space((time_size+subj_size+spacing) - x_pos);
			 
			std::cout << table[day][lesson].teacher;
			x_pos += table[day][lesson].teacher.size();
			// 3* spacing bcs the spacing was applied three times up to here
			// +1 bcs there was an additional | character
			// I hate this calculation too, by the way
			terse_space((time_size+subj_size+teach_size+spacing*3+1) - x_pos);
			 
			std::cout << table[day][lesson].room;
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

//TODO: Find a way to let the user edit all this without recompiling (Config File!?)
const std::string DIR_PREFIX = "/home/dangertech/OneDrive/Code/lesson-controller/";
const std::string TIME_FILE_LOC = "timedata.dat";
const std::string LESSON_FILE_LOC = "lessondata.dat";

int read_table()
{
	// Space - and linebreak agnostic function that reads in the lessondata
	///// Timeframe data
	std::ifstream timefile(TIME_FILE_LOC);
	if (timefile.is_open())
	{
		char cur_char;
		int digit_warn = false;
		int brackets = 0; // Stores, on which 'indentation level' the parser is.
		std::vector < int > num_buf; // Buffer to store parsed digits temporarily
		while ( timefile.get(cur_char) )
		{
			if ( cur_char == ' ' || cur_char == '\n')
				continue;
			else if (cur_char == '{')
			{
				brackets++;
				continue;
			}
			else if (cur_char == '}')
				brackets--;
			 
			if (brackets == 2)
			{
				if (cur_char != ':')
				{
					// Write this number into the num_buffer
					int this_buf = cur_char - '0'; // Make an int that represents the correct digit
					std::cout << "This_buf: " << this_buf << std::endl;
					num_buf.push_back(this_buf);
				}
				else
				{
					std::cout << "num_buf: " << num_buf[0] << std::endl;
					if (num_buf.size() == 1)
					{
						int hour = num_buf[0];
					}
					else if (num_buf.size() == 2)
					{
						int hour = num_buf[0] * 10 + num_buf[1];
					}
					else if (digit_warn == false)
					{
						std::cout << C_RED_B
							  << "You have a timeframe that has more "
							  << "digits than two.\nPlease stop "
							  << "violating standard 24 hour time "
							  << "units and correct this issue in\n"
							  << C_GREEN_U << DIR_PREFIX << TIME_FILE_LOC
							  << C_OFF << std::endl;
						digit_warn = true;
					}
					
					timeframes.push_back( std::make_pair(hour, 0) );
					num_buf.clear();
				}
			}
		}
		std::cout << timeframes[0].first;
		timefile.close();
		return 0;
	}
	else
	{
		std::cout << "Yeah shit, there's no file here!" << std::endl;
		// Add something to address this issue
		// Probably ask the user if a file should be created
		// But do it in main(), please
	}
	return ERROR;
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
