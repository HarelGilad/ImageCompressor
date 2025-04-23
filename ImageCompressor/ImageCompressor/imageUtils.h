#ifndef IMAGE_UTILS 
#define IMAGE_UTILS

#include "includes.h"
#include "fileUtils.h"
#include <turbojpeg.h>

class ImageUtils
{
public:
	static void generateCompressedJPEG(const std::string& path, unsigned short quality);

private:
	static ImageInfo getRawPixels(const std::string& path);
	static ImageInfo compressImage(ImageInfo& rawImage, const short quality);
};

#endif