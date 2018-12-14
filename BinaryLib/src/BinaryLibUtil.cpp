#include "BinaryLibUtil.hpp"

#include <cstring> // strerror

bool BinaryLibUtil::file_exists(const char *filename)
{
	std::ifstream ifile(filename);
	return ifile.good();
}

void BinaryLibUtil::delete_file(const char *filename)
{
	if(!std::remove(filename) && errno != 0)
	{
		throw std::string("error deleting ") + filename + ": " + strerror(errno);
	}
}

void BinaryLibUtil::move_file(const char* src, const char* dst, const bool overwrite)
{
	if(BinaryLibUtil::file_exists(dst))
	{
		if(overwrite)
		{
			delete_file(dst);
		}
		else
		{
			throw std::string("error renaming \"") + src + "\" to \"" + dst + "\": file already exists\n";
		}
	}
	std::rename(src, dst);
}
