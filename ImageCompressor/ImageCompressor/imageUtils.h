#ifndef IMAGE_UTILS 
#define IMAGE_UTILS

#include "includes.h"
#include "fileUtils.h"
#include <turbojpeg.h>
#include <algorithm>

class ImageUtils
{
public:
	static void generateCompressedJPEG(const std::string& path, unsigned short quality);
	static bool isQualityValid(const char* quality);

private:
	static ImageInfo getRawPixels(const std::string& path);
	static ImageInfo compressImage(ImageInfo& rawImage, const short quality);
};

#endif