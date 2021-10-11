// Functions and variables that control the given arguments
#pragma once

int vecstrcmp (const char* string, std::vector <std::string>);

int check_timeframe_availability(); // Best name, I know

void rel_lesson(int);

void show_week();

extern bool terse; // Terse output for script processing (Off by default)
extern bool count_empties; // Should empty lessons in the timetable be counted when parsing? (On by default)
