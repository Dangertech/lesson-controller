#include <vector>
#include <string>

int vecstrcmp(const char* string, std::vector < std::string > string_vec)
{
	for (int i = 0; i < string_vec.size(); i++)
	{
		if (string == string_vec[i])
		{
			return i;
		}
	}
	return -1;
}
