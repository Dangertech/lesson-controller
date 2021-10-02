#include <iostream>
#include <cstring>
#include <vector>
#include "timecalc.h"
#include "lesson.h"
#include "utils.h"



int main(int argc, char *argv[])
{
	get_time();
	 
	int weekday_check = vecstrcmp(argv[1], weekdays);
	if (weekday_check != -1)
	{
		std::cout << "--- " << weekdays[weekday_check] << " ---" << std::endl;
		if (table[weekday_check].size() == 0)
			std::cout << "No lessons registered" << std::endl;
		for (int i = 0; i < table[weekday_check].size(); i++)
		{
			show_lesson(weekday_check, i);
		}
	}
	else if (strcmp(argv[1], "time") == 0) //Only meant for debug
		std::cout << "Current time: " << hour << ":" << minute << std::endl;
	else
		std::cout << "Invalid argument '" << argv[1] << "'" << std::endl;
	 
	return 0;
}
