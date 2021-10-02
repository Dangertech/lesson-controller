#include <string>
#include <time.h>
#include <vector>
#include "timecalc.h"

int day, hour, minute;

void get_time()
{
	time_t epoch = time(NULL);
	struct tm *time_struct = localtime(&epoch);

	day = time_struct->tm_wday;
	hour = time_struct->tm_hour;
	minute = time_struct->tm_min;
}

std::vector < std::string > weekdays = { "sunday", "monday", "tuesday", "wednesday", "thursday", "friday", "saturday" };
