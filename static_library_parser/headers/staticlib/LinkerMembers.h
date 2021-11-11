#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <cstdint>
#include "libheader.h"
#include "../util/endianness.h"
namespace OrangeC
{
namespace StaticLib
{

// Why is this here? It's to remind me of the actual structure of the layout
namespace internal
{
// This is for the first linker member
// NOTE: This must be custom-parsed due to the NumberOfSymbols, this will all be parsed into this header, so...
// ELEMENTS IN THE FIRST LINKER MEMBER ARE ARRANGED IN ASCENDING ORDER, STRING TABLE ORDER IS THE SAME AS THE ORDER IN THE ARCHIVE
// FOR THE FIRST LINKER MEMBER
struct FirstLinkerMember
{
    uint32_t NumberOfSymbols;
    uint32_t* Offsets;   // Offsets, 4 * n of them, n = NumberOfSymbols, this is stored in BIG ENDIAN, for each name in the
                         // string table, the corresponding offset value is the offset of the symbol
    char** StringTable;  // malloc(sizeof(char*) * NumberOfSymbols) then malloc each successive thing, or we can create a wrapper
                         // with std::vector<std::string>
};
struct SecondLinkerMember
{
    uint32_t NumberOfMembers;  // Number of archive members
    uint32_t* Offsets;         // offset of each of the archive members
    uint32_t NumberOfSymbols;  // Same thing as in the first linker member
    uint16_t* Indices;   // An array of 1-based indicies that map symbol names to archive member offsets, n incidies where n is
                         // the number of symbols
    char** StringTable;  // Same as in the first one, same parsing methods
};
// SPECIAL PARSING CONSTRUCTION:
// malloc & fread the entire array
// Pull the entire strings out of the system
// ONLY METHOD TO DO THIS!!!!!!!!!!!!!!!!
struct ThirdLinkerMember
{
    char** strings;
};
}  // namespace internal
struct FirstLinkerMember
{
    uint32_t NumberOfSymbols;
    std::vector<uint32_t> Offsets;
    std::vector<std::string> StringTable;
    void add_symbol_number(uint32_t symbol_count)
    {
        NumberOfSymbols = symbol_count;
        Offsets.reserve(NumberOfSymbols);
        StringTable.reserve(NumberOfSymbols);
    }
    FirstLinkerMember() : NumberOfSymbols(0) {}
    // Exists for posterity reasons, the non-internal is the prefferred implementation due to ease of parsing
    explicit FirstLinkerMember(internal::FirstLinkerMember member) : NumberOfSymbols(member.NumberOfSymbols)
    {
        Offsets.reserve(NumberOfSymbols);
        StringTable.reserve(NumberOfSymbols);
        for (uint32_t i = 0; i < NumberOfSymbols; i++)
        {
            Offsets[i] = member.Offsets[i];
            StringTable[i] = std::string(member.StringTable[i]);
        }
    }
};
struct SecondLinkerMember
{
    uint32_t NumberOfMembers;              // Number of archive members
    std::vector<uint32_t> Offsets;         // offset of each of the archive members
    uint32_t NumberOfSymbols;              // Same thing as in the first linker member
    std::vector<uint16_t> Indices;         // An array of 1-based indicies that map symbol names to archive member offsets, n
                                           // incidies where n is the number of symbols
    std::vector<std::string> StringTable;  // Same as in the first one, same parsing methods
};
struct ThirdLinkerMember
{
    std::vector<std::string> Strings;  // The different names, this requires actually parsing a huge data structure then deallocing,
                                       // no better format for getting this is available at this time...
};

FirstLinkerMember parse_first_member(std::fstream& stream)
{
    FirstLinkerMember linkerMember;
    stream.read((char*)&linkerMember.NumberOfSymbols,
                sizeof(uint32_t));  // must be done this way so that we don't get unwanted conversions from decimal...
    linkerMember.NumberOfSymbols = Utils::big_to_platform(linkerMember.NumberOfSymbols);
    // Speed up that little bit I can by prereserving the correct ammount of data
    // Hope this doesn't crash anything in the process
    for (uint32_t i = 0; i < linkerMember.NumberOfSymbols; i++)
    {
        uint32_t temp = 0;
        stream.read((char*)&temp, sizeof(uint32_t));
        linkerMember.Offsets.push_back(
            Utils::big_to_platform(temp));  // Not reserving due to bad_allocs being thrown due to reservation sizes involved
    }
    for (uint32_t i = 0; i < linkerMember.NumberOfSymbols; i++)
    {
        std::string thing;
        std::getline(stream, thing, '\0');
        // Can't fix this slowdown sadly, TBH should be the fastest possible method as otherwise you have to do the resizing
        // yourself.
        linkerMember.StringTable.push_back(thing);  // This should be it, investigate this thoroughly
    }
    return linkerMember;
}
SecondLinkerMember parse_second_member(std::fstream& stream)
{
#define read_thing(a) stream.read((char*)&a, sizeof(a))
    SecondLinkerMember linkerMember;
    uint32_t member_count = 0;
    read_thing(member_count);
    linkerMember.NumberOfMembers = Utils::little_to_platform(member_count);
    for (unsigned long i = 0; i < linkerMember.NumberOfMembers; i++)
    {
        uint32_t MemberOffsets = 0;
        read_thing(MemberOffsets);
        linkerMember.Offsets.push_back(Utils::little_to_platform(MemberOffsets));
    }
    uint32_t symbol_count = 0;
    read_thing(symbol_count);
    linkerMember.NumberOfSymbols = Utils::little_to_platform(symbol_count);
    for (unsigned long i = 0; i < linkerMember.NumberOfSymbols; i++)
    {
        uint16_t index = 0;
        read_thing(index);
        linkerMember.Indices.push_back(Utils::little_to_platform(symbol_count));
    }
    for (unsigned long i = 0; i < linkerMember.NumberOfSymbols; i++)
    {
        std::string thing;
        std::getline(stream, thing, '\0');
        // Can't fix this slowdown sadly, TBH should be the fastest possible method as otherwise you have to do the resizing
        // yourself.
        linkerMember.StringTable.push_back(thing);  // This should be it, investigate this thoroughly
    }
    return linkerMember;
#undef read_thing
}
// This requires the third member's header due to needing the size...
ThirdLinkerMember parse_third_member(std::fstream& stream, LibHeaderExtended thirdHeader)
{
    ThirdLinkerMember linkerMember;
    uint64_t size = thirdHeader.Size;
    char* string_arr =
        new char[size];  // BIG CHARACTER ARRAY, IS NECESSARY BECAUSE WE DON'T KNOW THE MEMBER COUNT HERE, AT LEAST NOT OFFICIALLY
    stream.read(string_arr, size);  // Read into the BIG CHARACTER ARRAY
#define strlennull(a) (strlen(a) + 1)
    for (uint64_t i = 0; i < size;)
    {
        uint64_t val = strlennull(string_arr + i);
        linkerMember.Strings.push_back(std::string((string_arr + i), val));
        i += val;
    }
#undef strlennull
    delete string_arr;
    return linkerMember;
}
// Intent:
// This is here to allow you to provide your own parsing system within the system itself
// A global locking system will allow for a count eventually so you can just repeatedly call "parse_members" or something similar
// with a template And you will get what you want out of it...
// Nothing might come out of this though, reserving this for future use effectively otherwise
template <typename T>
T parse_nth_member(std::fstream& stream,
                   LibHeaderExtended nthHeader) = delete;  // delete so that this has to be USER DEFINED, if it's not COFF files
}  // namespace StaticLib
}  // namespace OrangeC