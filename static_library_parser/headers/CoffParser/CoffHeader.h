#pragma once
#include <cstdint>
#include "CoffEnumerations.h"
namespace OrangeC
{
namespace COFF
{
struct CoffHeader
{
    MachineType Machine;        // Type of target machine
    uint16_t NumberOfSections;  // Windows loader limits this to 96, size of section table indicated by this
    uint32_t
        TimeDateStamp;  // Low 32 bits of the number of seconds since 00:00 Jan 1, 1970 that indicates when the file was created
    uint32_t PointerToSymbolTable;  // File offset of the COFF symbol table, or 0 if no COFF symbol table is present, should be 0
                                    // for image since COFF debugging info is deprecated
    uint32_t NumberOfSymbols;       // Number of entries in the symbol table, this data can be used to local string table, which
                                    // immediately follows it. Should be 0 for an image like above
    uint16_t SizeOfOptionalHeader;  // Size of the optional header which is required for executable files but not for object files,
                                    // Seealso: section 3.4 in pecoff_v8.doc or CoffFiles.h where the relevant OptionalHeader
                                    // structures live
    COFFCharacteristics Characteristics;  // Attributes of the file
};
struct SectionTable
{
    // VVVVVVVV Find way to deal with UTF-8 cross-platform so as to be able to know if it's a null terminator or something else
    // because UTF-8 can be weird
    char Name[8];  // 8 byte, null-padded UTF-8 encoded string If it is exactly 8 characters long, there is no terminating null
    uint32_t VirtualSize;  // total size of section when loaded into memory, if this value is greater than SizeOfawData, the section
                           // is zero padded, only valid for executable images and should be set to zero for object files
    uint32_t VirtualAddress;  // For exec, addr of first byte of the section relative to the image base when section is loaded into
                              // mem, for object, the field is the addr of the first byte before relocation applied, for simplicity,
                              // 0, otherwise it is arbitrary that is subtracted from offsets during relocation.
    uint32_t SizeOfRawData;   // Size of the section (for object files) or the size of the initialized data on the disk (for image
                             // files), for executable images, must be a multiple of FileAlignment from the optional header, if this
                             // is less than VirtualSize, the remainder of the section is zero-filled. This is rounded, so this must
                             // be LESS than VirtualSize as well, when a section contains only uninit data, this should be ZERO
    uint32_t
        PointerToRawData;  // The file pointer to the first page of the section within the COFF file, for executable images, this
                           // must be a multiple of FileAlignment from the optional header. For object files, FOR BEST PERFORMANCE:
                           // align to a 4-byte boundary, when there is only uninitialized data, field should be zero
    uint32_t PointerToRelocations;  // The file pointer to the beginning of relocation entries for the section. Zero for executables
    // or if no relocs
    uint32_t PointerToLineNumbers;  // File pointer to the beginning of line-number entries for the section, should be ZZERO if
    // there are no COFF line numbers, should also be 0 since COFF debugging info is deprecated
    uint32_t NumberOfRelocations;  // Number of relocation entries for the section, ZERO for executables
    uint32_t NumberOfLineNumbers;  // line-number entries, should be ZERO for an image due to deprecation of COFF debugging info
    SectionCharacteristics Characteristics;  // Flags that describe characteristics of section, see SectionCharacteristics or
                                             // section 4.1 "section flags" of pecoff_v8.doc
};
// OBJECT ONLY!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
struct COFFRelocations
{
    uint32_t VirtualAddress;  // Address of the item to which relocation is applied, This is offset from beginning of section, plus
                              // the value of the RVA/Offset field, See section 4 of pecoff_v8.doc for more.
    uint32_t SymbolTableIndex;
    COFFRelocType Type;
};
union CoffDebugType
{
    uint32_t SymbolTableIndex;
    uint32_t VirtualAddress;
};
struct CoffLineNumber
{
    CoffDebugType Type;
    // Note: this is 1-based, relative to the function
    uint16_t LineNumber;  // When non-zero, specifies a one based line number, when zero, the Type field is interpreted as a symbol
                          // table index for a function
};
union CoffSymbolName
{
    char ShortName[8];  // An array of 8 bytes, the array is padded with nulls on the right if the name is less than 8 bytes long
    struct
    {
        uint32_t Zeroes;
        uint32_t Offset;
    };
};
struct CoffSymbolTable
{
    CoffSymbolName Name;  // Array of 8 bytes if not longer than 8 bytes long
    uint32_t Value;  // The value that is associated with the symbol. The interpretation of this field depends on SectionNumber and
                     // StorageClass, a typical meaning is the relocatable address
    int16_t SectionNumber;     // A signed integer that identifies the section, using a 1 based index into the scetion table, 5.4.2
                               // section number values pecoff_v8.doc
    uint16_t Type;             // A number that represents a type, 0x20 for function, 0x0 for not a function
    uint8_t StorageClass : 1;  // An enumerated value that represents storage class, see section 5.4.4
    uint8_t NumberOfAuxSymbols : 1;  // Typically no more than 1
};

}  // namespace COFF
}  // namespace OrangeC