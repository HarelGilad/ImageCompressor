#ifndef IMAGE_UTILS 
#define IMAGE_UTILS

#include "includes.h"
#include "fileUtils.h"
#include <turbojpeg.h>
#include <algorithm>

class ImageUtils
{
public:
	// Methods
	static void generateCompressedJPEG(const std::string& path, unsigned short quality); // Generates a compressed JPEG image file from another JPEG image file
	static bool isQualityValid(const char* quality); // Validates the quality argument

private:
	// Helper Methods
	static ImageInfo getRawPixels(const std::string& path); // Extracts raw RGB pixel grid from a JPEG image file
	static ImageInfo compressImage(ImageInfo& rawImage, const short quality); // Compresses raw RGB pixel grid into JPEG image
};

#endif