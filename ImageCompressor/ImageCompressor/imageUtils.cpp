#include "imageUtils.h"
#include <iostream>

void ImageUtils::generateCompressedJPEG(const std::string& path, unsigned short quality)
{
    ImageInfo rawImage = getRawPixels(path);
    if (!rawImage.data) return;
    
    ImageInfo compressedImage = compressImage(rawImage, quality);
    if (!compressedImage.data)
    {
        tjFree(rawImage.data);
        return;
    }
    
    std::string newPath = "compressed_" + path;
    FileUtils::createFile(newPath, compressedImage.data, compressedImage.size);
    
    // Cleanup
    tjFree(rawImage.data);
    tjFree(compressedImage.data);
}

ImageInfo ImageUtils::getRawPixels(const std::string& path)
{
    byte* buffer = nullptr;
    ImageInfo rawImage{ 0 };
    int width = 0, height = 0;

    size_t bufferSize = FileUtils::getFileContents(buffer, path);
    if (!bufferSize)
    {
        rawImage.data = nullptr;

        std::cerr << "FileError: Could Not Find " << path << std::endl;
        return rawImage;
    }

    tjhandle decompressor = tjInitDecompress();
    if (!decompressor)
    {
        delete[] buffer;
        rawImage.data = nullptr;

        std::cerr << "DecompressionError: " << tjGetErrorStr() << std::endl;
        return rawImage;
    }
    
    int subsamp = 0;
    if (tjDecompressHeader2(decompressor, buffer, bufferSize, &width, &height, &subsamp) != 0)
    {
        tjDestroy(decompressor);

        delete[] buffer;
        rawImage.data = nullptr;

        std::cerr << "ParsingError: " << tjGetErrorStr() << std::endl;
        return rawImage;
    }

    size_t pixelsCount = width * height * 4;
    rawImage.data = tjAlloc(pixelsCount);

    if (tjDecompress2(decompressor, buffer, bufferSize, rawImage.data, width, 0, height, TJPF_RGB, TJFLAG_FASTDCT) != 0)
    {
        tjFree(rawImage.data);
        tjDestroy(decompressor);

        delete[] buffer;
        delete[] rawImage.data;
        rawImage.data = nullptr;

        std::cerr << "ParsingError: " << tjGetErrorStr() << std::endl;
        return rawImage;
    }

    rawImage.size = pixelsCount;
    rawImage.width = width;
    rawImage.height = height;

    // Cleanup
    delete[] buffer;
    tjDestroy(decompressor);

    return rawImage;
}

ImageInfo ImageUtils::compressImage(ImageInfo& rawImage, const short quality)
{
    ImageInfo compressedImage{ 0 };

    tjhandle compressor = tjInitCompress();
    if (!compressor)
    {
        compressedImage.data = nullptr;

        std::cerr << "CompressionError: " << tjGetErrorStr() << std::endl;
        return compressedImage;
    }

    if (tjCompress2(compressor, rawImage.data, rawImage.width ,0, rawImage.height ,TJPF_RGB ,&compressedImage.data , reinterpret_cast<unsigned long*>(&compressedImage.size), TJSAMP_420, quality, TJFLAG_FASTDCT) != 0)
    {
        tjDestroy(compressor);
        compressedImage.data = nullptr;

        std::cerr << "CompressionError: " << tjGetErrorStr() << std::endl;
        return compressedImage;
    }

    // Cleanup
    tjDestroy(compressor);

    return compressedImage;
}
