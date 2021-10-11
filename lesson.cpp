#include <vector>
#include <iostream>
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
		{"Maths", "Bärbel", "333"},
		{"Maths", "Bärbel", "333"},
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
		{"Maths", "Bärbel", "333"},
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

std::vector < std::pair<int, int> > timeframes = { {7, 45}, {8, 35}, {9, 35}, {10, 25}, {11, 25}, {12, 10}, {12, 55} };

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

int time_size = 8, subj_size, teach_size, room_size;

void terse_space() // [Internal only], used for show_lessons
{
	if (terse == false)
	{
		std::cout << " | ";
	}
	else
		std::cout << "/";
}

void show_lessons(std::vector < std::pair<int, int> > to_show) // Show data for a specific lesson
{
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
			continue;
		}
		 
		if (construct && !terse)
		{
			std::cout << "TIME" << std::endl;
		}
		 
		if (to_show[i].first < table.size() && to_show[i].second < table[day].size()) // Check to avoid crashes
		{
			// Show time
			if (terse == false)
				std::cout << "[";
			if (timeframes[lesson].first != 0) // Check to warn if the timeframe doesn't exist
				std::cout << timeframes[lesson].first;
			else
				std::cout << "\033[1;31m" << "!" << "\033[0m";
			std::cout << ":";
			if (timeframes[lesson].second != 0)
				std::cout << timeframes[lesson].second;
			else
				std::cout << "\033[1;31m" << "!" << "\033[0m";
			if (terse == false)
				std::cout << "] "; 
			else
				std::cout << "/";
			 
			std::cout << table[day][lesson].subject;
			terse_space();
			std::cout << table[day][lesson].teacher;
			terse_space();
			std::cout << table[day][lesson].room;
		}
		else
		{
			std::cout << "Lesson " << lesson << " on weekday " <<  weekdays[day] << " does not exist!" << std::endl;
		}
	}
	std::cout << std::endl; // Enforce break after the last lesson is printed
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
