#ifndef FILE_UTILS 
#define FILE_UTILS

#include <fstream>

using byte = uint8_t;

namespace fileUtils
{
	void createFile(const std::string& path, const byte* data, const size_t size);
	byte* getFileContents(const std::string& path);
}

#endif