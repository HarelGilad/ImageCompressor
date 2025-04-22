#include "fileUtils.h"

void fileUtils::createFile(const std::string& path, const byte* data, const size_t size)
{
    std::ofstream file(path, std::ios::binary);
    if (!file) return;

    file.write(reinterpret_cast<const char*>(data), size);
}

byte* fileUtils::getFileContents(const std::string& path)
{
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file) return nullptr;

    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);

    byte* buffer = new byte[size];
    if (!file.read(reinterpret_cast<char*>(buffer), size))
    {
        delete[] buffer;
        return nullptr;
    }

    return buffer;
}