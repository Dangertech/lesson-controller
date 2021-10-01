#include <iostream>
#include <cstring>
#include "timecalc.h"
#include "lesson.h"

int main(int argc, char *argv[])
{
	get_time();
	if (strcmp(argv[1], "time") == 0) //Only meant for debug
	{
		std::cout << "Current time: " << hour << ":" << minute << std::endl;
	} 
	else
	{
		std::cout << "Invalid argument '" << argv[1] << "'" << std::endl;
	}
	 
	std::cout << table[0][1].subject << std::endl;
	return 0;
}
