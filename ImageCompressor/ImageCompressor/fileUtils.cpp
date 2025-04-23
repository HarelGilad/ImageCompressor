#include "fileUtils.h"

/// <summary>
/// Creates a new binary file, and writes the data into it
/// </summary>
/// <param name="path">Path of the new file</param>
/// <param name="data">Buffer to write into the file</param>
/// <param name="size">Buffer size</param>
void FileUtils::createFile(const std::string& path, const byte* data, const size_t size)
{
    std::ofstream file(path, std::ios::binary);
    if (!file) return;

    file.write(reinterpret_cast<const char*>(data), size);
}

/// <summary>
/// Reads binary file content into a buffer
/// </summary>
/// <param name="buffer">Buffer to read into</param>
/// <param name="path">Path of file to read from</param>
/// <returns>Content size (the size of the buffer after calling the function)</returns>
size_t FileUtils::getFileContents(byte*& buffer, const std::string& path)
{
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file) return 0;

    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);

    buffer = new byte[size];
    if (!file.read(reinterpret_cast<char*>(buffer), size))
    {
        delete[] buffer;
        return 0;
    }

    return size;
}