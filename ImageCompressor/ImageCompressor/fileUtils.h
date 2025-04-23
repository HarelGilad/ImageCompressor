#ifndef FILE_UTILS 
#define FILE_UTILS

#include "includes.h"
#include <fstream>

namespace FileUtils
{
	// Methods
	void createFile(const std::string& path, const byte* data, const size_t size); // Creates a new binary file, and writes the data into it
	size_t getFileContents(byte*& buffer, const std::string& path); // Reads binary file content into a buffer
}

#endif