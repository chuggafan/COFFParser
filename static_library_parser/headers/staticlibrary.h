#pragma once
#include "staticlib/libheader.h"
#include "staticlib/LinkerMembers.h"
#include "CoffParser/CoffFiles.h"
namespace OrangeC
{
namespace StaticLib
{
bool check_valid(std::fstream& stream)
{
    if (!stream.is_open())
    {
        return false;
    }
    stream.seekg(stream.beg);
    char arch[8] = {0};
    stream.read(arch, 8);

    return !memcmp(arch, "!<arch>\n", 8);
}

}  // namespace StaticLib
}  // namespace OrangeC