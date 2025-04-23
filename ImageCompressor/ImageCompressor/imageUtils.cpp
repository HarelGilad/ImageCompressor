#include "imageUtils.h"
#include <iostream>

/// <summary>
/// Generates a compressed JPEG image file from another JPEG image file
/// </summary>
/// <param name="path">Original JPEG image file path</param>
/// <param name="quality">Quality of compression</param>
void ImageUtils::generateCompressedJPEG(const std::string& path, unsigned short quality)
{
    // Extract raw pixel data from the JPEG image
    ImageInfo rawImage = getRawPixels(path);
    if (!rawImage.data) return;
    
    // Compress the raw pixel data into JPEG format
    ImageInfo compressedImage = compressImage(rawImage, quality);
    if (!compressedImage.data)
    {
        tjFree(rawImage.data);
        return;
    }
    
    // Create new file path and write compressed JPEG image data to file
    std::string newPath = "cmp_" + path;
    FileUtils::createFile(newPath, compressedImage.data, compressedImage.size);
    std::cout << "Generated Compressed File <" << newPath << ">" << std::endl;

    // Cleanup
    tjFree(rawImage.data);
    tjFree(compressedImage.data);
}

/// <summary>
/// Validates the quality argument
/// </summary>
/// <param name="quality">Quality as a c_str</param>
/// <returns>Whether the quality argument is parsable</returns>
bool ImageUtils::isQualityValid(const char* quality)
{
    std::string qualityStr = quality;
    return !qualityStr.empty() && std::all_of(qualityStr.begin(), qualityStr.end(), isdigit);
}

/// <summary>
/// Extracts raw RGB pixel grid from a JPEG image file
/// </summary>
/// <param name="path">JPEG image file path</param>
/// <returns>Raw RGB pixel grid as ImageInfo</returns>
ImageInfo ImageUtils::getRawPixels(const std::string& path)
{
    byte* buffer = nullptr;
    ImageInfo rawImage{ 0 };
    int width = 0, height = 0;

    // Load the file into buffer
    size_t bufferSize = FileUtils::getFileContents(buffer, path);
    if (!bufferSize)
    {
        rawImage.data = nullptr;

        std::cerr << "FileError: Could Not Find <" << path << ">" << std::endl;
        return rawImage;
    }

    // Initialize JPEG decompressor
    tjhandle decompressor = tjInitDecompress();
    if (!decompressor)
    {
        delete[] buffer;
        rawImage.data = nullptr;

        std::cerr << "DecompressionError: " << tjGetErrorStr() << std::endl;
        return rawImage;
    }
    
    int subsamp = 0;
    // Parse image header to get width & height
    if (tjDecompressHeader2(decompressor, buffer, bufferSize, &width, &height, &subsamp) != 0)
    {
        tjDestroy(decompressor);

        delete[] buffer;
        rawImage.data = nullptr;

        std::cerr << "ParsingError: " << tjGetErrorStr() << std::endl;
        return rawImage;
    }

    // Allocate memory for raw RGB pixels (3 bytes per pixel)
    size_t pixelsCount = width * height * 3;
    rawImage.data = tjAlloc(pixelsCount);

    // Decompress JPEG into raw RGB buffer
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

    // Set image metadata
    rawImage.size = pixelsCount;
    rawImage.width = width;
    rawImage.height = height;

    // Cleanup
    delete[] buffer;
    tjDestroy(decompressor);

    return rawImage;
}

/// <summary>
/// Compresses raw RGB pixel grid into JPEG image
/// </summary>
/// <param name="rawImage">Raw RGB pixel grid of an image</param>
/// <param name="quality">Quality of compression</param>
/// <returns>Compressed JPEG image as ImageInfo</returns>
ImageInfo ImageUtils::compressImage(ImageInfo& rawImage, const short quality)
{
    ImageInfo compressedImage{ 0 };

    // Initialize JPEG compressor
    tjhandle compressor = tjInitCompress();
    if (!compressor)
    {
        compressedImage.data = nullptr;

        std::cerr << "CompressionError: " << tjGetErrorStr() << std::endl;
        return compressedImage;
    }

    // Compress raw RGB data into JPEG format
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
