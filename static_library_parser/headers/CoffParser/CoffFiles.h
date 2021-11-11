#pragma once
#include <cstdint>
#include <istream>
#include "CoffEnumerations.h"

namespace OrangeC
{
namespace COFF
{
// This will be embedded in the PE32 Header with a union because this is a subset of the PE32 Header
// Why? Ask microsoft, I'm just here to make this easier on myself if I want to do absolutely stupid stuff
// "Image Only" btw
struct PE32PlusOptionalHeader
{
    OptionalHeaderMagic Magic;   // A magic value of which describes what type of header this is, ROM, PE32+ or PE32
    uint8_t MajorLinkerVersion;  // Linker version compiled with
    uint8_t MinorLinkerVersion;  // Linker version compiled with
    uint32_t SizeOfCode;         // Text section size or sum of all code sections if there's multiple
    uint32_t SizeOfInitializedData;
    uint32_t SizeOfUninitializedData;
    uint32_t AddressOfEntryPoint;
    uint32_t BaseOfCode;
};
static_assert(sizeof(PE32PlusOptionalHeader) == 24, "The PE32+ Optional Header must be 24 in size for correct parsing!");
struct PE32OptionalHeader
{
    // Treat these as the same but different ;)
    union
    {
        PE32PlusOptionalHeader
            PlusHeader;  // extract this into it's equivalent for direct usage, anonymous unions FTW! in this case
        struct
        {
            OptionalHeaderMagic Magic;
            uint8_t MajorLinkerVersion;
            uint8_t MinorLinkerVersion;
            uint32_t SizeOfCode;  // Text section size or sum of all code sections if there's multiple
            uint32_t SizeOfInitializedData;
            uint32_t SizeOfUninitializedData;
            uint32_t AddressOfEntryPoint;
            uint32_t BaseOfCode;
        };
    };
    uint32_t BaseOfData;
};
static_assert(sizeof(PE32OptionalHeader) == 28, "The PE32 Optional Header must be 28 in size for correct parsing!");
// Write this template once and we can use it for both PE32 and PE32plus
template <typename T>
struct WindowsSpecificOptionalHeader
{
    static_assert(sizeof(T) == 4 || sizeof(T) == 8,
                  "The size of the integer passed to here must be 4 or 8, no inbetweens for this to work");
    T ImageBase;
    uint32_t SectionAlignment;
    uint32_t FileAlignment;
    uint16_t MajorOperatingSystemVersion;
    uint16_t MinorOperatingSystemVersion;
    uint16_t MajorImageVersion;
    uint16_t MinorImageVersion;
    uint16_t MajorSubsystemVersion;
    uint16_t MinorSubsystemVersion;
    uint32_t Win32VersionValue;
    uint32_t SizeOfImage;             // Must be a multiple of SectionAlignment
    uint32_t SizeOfHeaders;           // Rounded up to a multiple of FileAlignment
    uint32_t CheckSum;                // IMAGHELP.DLL has the thing for computing checksums
    WindowsSubsystem Subsystem;       // What subsytem are we using? E.g. GUI vs CUI
    DLLCharacteristics DllSubsystem;  // What are the characteristics of this DLL
    T SizeOfStackReserve;             // Size of stack RESERVED, not committed, there is a difference
    T SizeOfStackCommit;              // Size of stack to commit
    T SizeOfHeapReserve;              // Size of local heap to reserve, same policy of StackReserve vs Commit
    T SizeOfHeapCommit;               // Size of LOCAL heap to commit
    uint32_t LoaderFlags;             // Reserved, must be 0
    uint32_t NumberOfRVAAndSizes;     // Number of data-size pairs of something
};
using PE32WindowsSpecificOptionalHeader = WindowsSpecificOptionalHeader<uint32_t>;
using PE32PlusWindowsSpecificOptionalHeader = WindowsSpecificOptionalHeader<uint64_t>;
static_assert(sizeof(PE32WindowsSpecificOptionalHeader) == 68, "The PE windows optional header should be 68 bytes in size");
static_assert(sizeof(PE32PlusWindowsSpecificOptionalHeader) == 88,
              "The PE32Plus windows optional header should be 88 bytes in size");

struct COFFFileHeader
{
    MachineType Machine;            // MachineType is an enum that is uint16_t as well
    uint16_t NumberOfSections;      // the windows loader limits this to 96, this indicates the size of the section table btw
    uint32_t TimeDateStamp;         // Low 32 bits, this will *NEVER* be bad ever lmao
    uint32_t PointerToSymbolTable;  // 0 if no COFF symbol table is present, 0 for image because COFF debugging data is deprecated
    uint32_t NumberOfSymbols;       // 0 for image, same as above
    uint16_t SizeOfOptionalHeader;  // WEEEWOOWEEOOO, 0 for object
    COFFCharacteristics Characteristics;  // Image characteristics, check the enum for more
};
static_assert(sizeof(COFFFileHeader) == 20, "The COFF header is statically 20 bytes in size");
// Define this the exact same way
typedef struct _OCC_IMAGE_DATA_DIRECTORY
{
    uint32_t VirtualAddress;  // Actually the RVA, which is the address of the table relative to the base address of the image when
                              // the table is loaded
    uint32_t Size;
} OCC_IMAGE_DATA_DIRECTORY, *POCC_IMAGE_DATA_DIRECTORY;
struct OptionalHeaderDataDirectories
{
    OCC_IMAGE_DATA_DIRECTORY ExportTable;          // .edata section (image only)
    OCC_IMAGE_DATA_DIRECTORY ImportTable;          // .idata section
    OCC_IMAGE_DATA_DIRECTORY ResourceTable;        // .rsrc table
    OCC_IMAGE_DATA_DIRECTORY ExceptionTable;       // .pdata section
    OCC_IMAGE_DATA_DIRECTORY CertificateTable;     // Attribute certificate table (image only)
    OCC_IMAGE_DATA_DIRECTORY BaseRelocationTable;  // .reloc section (image only)
    OCC_IMAGE_DATA_DIRECTORY Debug;                // .debug Section
    OCC_IMAGE_DATA_DIRECTORY Architecture;         // Reserved: must be 0
    OCC_IMAGE_DATA_DIRECTORY GlobalPtr;  // Size member here must be 0, RVA of the value to be stored in the global pointer register
    OCC_IMAGE_DATA_DIRECTORY TLSTable;   // .tls section
    OCC_IMAGE_DATA_DIRECTORY LoadConfigTable;        // Load Configuration Structure (Image Only)
    OCC_IMAGE_DATA_DIRECTORY BoundImport;            // Bound import table address and size
    OCC_IMAGE_DATA_DIRECTORY IAT;                    // Import address table address and size
    OCC_IMAGE_DATA_DIRECTORY DelayImportDescriptor;  // Delay Load Import Tables (Image Only)
    OCC_IMAGE_DATA_DIRECTORY CLRRuntimeHeader;       // .cormeta section (OBJECT ONLY!!!!!!!!!!!)
    OCC_IMAGE_DATA_DIRECTORY Reserved;               // must be 0
};
static_assert(sizeof(OptionalHeaderDataDirectories) == 128,
              "The optional image header for data directories must be 128 bytes in size");
template <typename T, typename K>
struct FullOptionalHeader
{
    T OptionalHeaderHeader;
    K WindowsSpecificOptionalHeader;
    OptionalHeaderDataDirectories OptionalDataDirectories;
};
using PE32FullOptionalHeader = FullOptionalHeader<PE32OptionalHeader, PE32PlusWindowsSpecificOptionalHeader>;
using PE32PlusFullOptionalHeader = FullOptionalHeader<PE32PlusOptionalHeader, PE32PlusWindowsSpecificOptionalHeader>;
}  // namespace COFF
}  // namespace OrangeC