#include <fstream>
#include <iostream>
#include "headers/staticlibrary.h"
#include "headers/util/endianness.h"
#include <exception>
int main(int argc, char* argv[])
{
    using namespace OrangeC::StaticLib;
    std::fstream stream;
    if (argc > 1)
    {
        stream.open(argv[1], std::ios_base::binary | std::ios_base::in);
    }
    bool is_valid_stream = check_valid(stream);
    if (!is_valid_stream)
    {
        return 1;
    }
    LibHeader header = parse_lib_header(stream, true);
    std::cout << "First: " << verify_valid_header(header) << std::endl;
    FirstLinkerMember linkerMember;
    linkerMember = parse_first_member(stream);
    uint32_t comp_val = 0;
    for (auto&& val : linkerMember.Offsets)
    {
        if (val != comp_val)
        {
            // std::cout << val << std::endl;  // check
            comp_val = val;
        }
    }
    LibHeader secondHeader = parse_lib_header(stream, false);
    bool isvalid = verify_valid_header(secondHeader);
    std::cout << "Second: " << isvalid << std::endl;
    if (!isvalid)
    {
        std::cout << std::string(secondHeader.Name, 60) << std::endl;
    }
    SecondLinkerMember secondLinkerMember = parse_second_member(stream);
    LibHeader third_header = parse_lib_header(stream, false);

    isvalid = verify_valid_header(third_header);
    std::cout << "Third: " << isvalid << std::endl;
    if (!isvalid || isvalid)
    {
        std::cout << std::string(third_header.Name, 60) << std::endl;
    }
    ThirdLinkerMember third_member = parse_third_member(stream, (LibHeaderExtended)third_header);
}