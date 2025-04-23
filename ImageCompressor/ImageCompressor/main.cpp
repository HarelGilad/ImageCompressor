#include "imageUtils.h"
#include <iostream>

#define DEFAULT_QUALITY_VALUE 75

enum ARGUMENT_OPTION
{
	DEFAULT_QUALITY = 2,
	CUSTOM_QUALITY = 3
};

enum ARGUMENT_INDEX
{
	PATH = 1,
	QUALITY = 2
};

int main(int argc, char** argv)
{
	// Check for valid number of arguments
	if (argc != ARGUMENT_OPTION::DEFAULT_QUALITY && argc != ARGUMENT_OPTION::CUSTOM_QUALITY)
	{
		std::cout << "Usage: ImageCompressor <filepath> <quality>" << std::endl;
		return 1;
	}

	// Get filepath argument
	std::string path = argv[ARGUMENT_INDEX::PATH];
	unsigned short quality = DEFAULT_QUALITY_VALUE;

	// If quality specified
	if (argc == ARGUMENT_OPTION::CUSTOM_QUALITY)
	{
		// If quality argument is valid
		if (ImageUtils::isQualityValid(argv[ARGUMENT_INDEX::QUALITY]))
		{
			// Clamp between 0-100
			quality = std::clamp(std::atoi(argv[ARGUMENT_INDEX::QUALITY]), 0, 100);
		}
		else
		{
			std::cerr << "Invalid Quality Value: " << argv[ARGUMENT_INDEX::QUALITY] << std::endl;
			return 1;
		}
	}

	// Perform compression
	ImageUtils::generateCompressedJPEG(path, quality);

	return 0;
}