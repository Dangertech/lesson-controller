#pragma once
#include <vector>
#include <string>

struct lesson
{
	std::string subject;
	std::string teacher;
	std::string room;
};

extern std::vector < std::vector < struct lesson > > table;
