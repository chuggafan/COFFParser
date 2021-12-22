#pragma once
#include <string>
#include <cstring>
#include <fstream>
#include <iostream>
namespace OrangeC
{
namespace StaticLib
{
// Member headers start on the first even address after the end of the previous header
// This is right-padded in case there is nothing more to the thing
struct LibHeader
{  // NAME field
    // "name/" name is the archive member name
    // "/" <- just a "/" The archive member is one of the two linker members, both of the linker members have this name
    // "//" <- The archive member is the longnames member, which consists of a series of null-terminated ASCII strings, the
    // longnames member is the third archive member and must always be present even if the contents are empty, this is only used if
    // there are names longer than 16 bytes in the file
    // "/n" <- The name of the archive member is located at offset n within the longnames member, the number n being the decimal
    // representation of the offset, e.g. "/26" indicates that the name of the archive member is located 26 bytes beyond the
    // beginning of the longnames member contents
    char Name[16];    // a / terminates, unless it starts, then it's special, see more below
    char Date[12];    // seconds since 1/1/1970 UTC in ASCII
    char UserID[6];   // Meaningless
    char GroupID[6];  // Meaningless
    char Mode[8];     // ASCII octal representation of the member's file mode
    char Size[10];    // ASCII decimal representation of the total size of the archive member, not including the size of the header
    char HeaderEnd[2];  // This should ALWAYS be equal to `\n (0x60 0x0A)
};
static_assert(sizeof(LibHeader) == 60, "The LibHeader for COFF files is 60 bytes in size as per the pecoff_v8.docx spec");
// This must be gotten after a full-parse of the LIB section
// This exists for two MAJOR reasons:
// Reason number 1: Name can actually be greater than 16 bytes because of the 3rd linker section
// Reason number 2: std::string is marginally faster than non-std::string
struct LibHeaderExtended
{  // NAME field
    // "name/" name is the archive member name
    // "/" <- just a "/" The archive member is one of the two linker members, both of the linker members have this name
    // "//" <- The archive member is the longnames member, which consists of a series of null-terminated ASCII strings, the
    // longnames member is the third archive member and must always be present even if the contents are empty, this is only used if
    // there are names longer than 16 bytes in the file
    // "/n" <- The name of the archive member is located at offset n within the longnames member, the number n being the decimal
    // representation of the offset, e.g. "/26" indicates that the name of the archive member is located 26 bytes beyond the
    // beginning of the longnames member contents
    std::string Name;
    uint64_t Date;
    std::string UserID;
    std::string GroupID;
    uint32_t Mode;
    uint64_t Size;
    std::string HeaderEnd;
#define chararrtostr(a) a, sizeof(a)
    explicit LibHeaderExtended(LibHeader header) :
        Name(chararrtostr(header.Name)),
        UserID(chararrtostr(header.UserID)),
        GroupID(chararrtostr(header.GroupID)),
        HeaderEnd(chararrtostr(header.HeaderEnd))
    {
        Date = std::stoull(std::string(chararrtostr(header.Date)));
        Mode = std::stoul(std::string(chararrtostr(header.Mode)));
        Size = std::stoull(std::string(chararrtostr(header.Size)));
    }
#undef chararrtostr
};
// Aligns to the even address if not already on the even address
void align_stream_to_even_input(std::istream& stream)
{
    std::streampos pos = stream.tellg();
    if (pos % 2)
    {
        stream.seekg(1, std::ios_base::cur);
    }
}
bool verify_valid_header(LibHeader header) { return !memcmp(header.HeaderEnd, "`\n", 2); }
bool verify_valid_header(LibHeaderExtended header) { return header.HeaderEnd == std::string("`\n"); }
// NOTE: UNLESS THIS IS THE FIRST LINKER MEMBER THIS MUST BE ALIGNED TO THE FIRST EVEN ADDRESS AFTER THE PARSING, SINCE I HAVE NO
// WAY OF ENSURING WHAT IS OR IS NOT THE FIRST LINKER MEMBER OTHER THAN DUMB HACKERY THIS NEEDS TO BE ENSURED BY PROGRAMS USING THIS
// LIBRARY
LibHeader parse_lib_header(std::fstream& stream, bool isFirst)
{
    if (!isFirst)
    {
        align_stream_to_even_input(stream);
    }
    LibHeader header;
    stream.read((char*)&header, sizeof(header));  // Abuse layouts to read directly into the structure
    return header;
}
LibHeaderExtended parse_extended_lib_header(std::fstream& stream, bool isFirst)
{
    return (LibHeaderExtended)parse_lib_header(stream, isFirst);
}
}  // namespace StaticLib
}  // namespace OrangeC