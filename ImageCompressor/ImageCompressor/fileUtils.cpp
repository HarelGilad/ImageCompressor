#include "fileUtils.h"

void FileUtils::createFile(const std::string& path, const byte* data, const size_t size)
{
    std::ofstream file(path, std::ios::binary);
    if (!file) return;

    file.write(reinterpret_cast<const char*>(data), size);
}

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