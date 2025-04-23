#ifndef INCLUDES
#define INCLUDES

using byte = unsigned char;

struct ImageInfo
{
	byte* data;
	size_t size;
	int width;
	int height;
};

#endif