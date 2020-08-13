#include "helper_funcs.h"

std::string get_filename(std::string path)
{
	size_t botDirPos = path.find_last_of("\\");
	if (botDirPos > 0)
	{
		// get directory
		std::string dir = path.substr(0, botDirPos);
		// get file
		std::string file = path.substr(botDirPos + 1, path.length());
		return file;
	}
	else
		return path;
}