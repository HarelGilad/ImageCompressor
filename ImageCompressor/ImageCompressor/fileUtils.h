#ifndef FILE_UTILS 
#define FILE_UTILS

#include "includes.h"
#include <fstream>

namespace FileUtils
{
	void createFile(const std::string& path, const byte* data, const size_t size);
	size_t getFileContents(byte*& buffer, const std::string& path);
}

#endif