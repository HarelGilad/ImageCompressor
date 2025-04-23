#include "imageUtils.h"
#include <iostream>

#define DEFUALT_QUALITY 75

int main(int argc, char** argv)
{
	unsigned short quality = DEFUALT_QUALITY;
	if (argc == 3)
	{
		quality = std::atoi(argv[2]);
	}
	else if (argc != 2)
	{
		std::cout << "Usage: ImageCompressor <filepath> <quality>" << std::endl;
		return 0;
	}

	std::string path = std::string(argv[1]);
	ImageUtils::generateCompressedJPEG(path, quality);

	return 0;
}