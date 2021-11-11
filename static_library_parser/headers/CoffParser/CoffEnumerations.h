#pragma once
#include <cstdint>
namespace OrangeC
{
namespace COFF
{
#define IFM_(a, b) IMAGE_FILE_MACHINE_##a = b
enum MachineType : uint16_t
{
    IFM_(UNKOWN, 0x0),
    IFM_(AM33, 0x1d3),
    IFM_(AMD64, 0x8664),
    IFM_(ARM, 0x1c0),
    IFM_(ARMV7, 0x1c4),
    IFM_(EBC, 0xebc),
    IFM_(I386, 0x14c),
    IFM_(IA64, 0x200),
    IFM_(M32R, 0x9041),
    IFM_(MIPS16, 0x266),
    IFM_(MIPSFPU, 0x366),
    IFM_(MIPSFPU16, 0x466),
    IFM_(POWERPC, 0x1f0),
    IFM_(POWERPCFP, 0x1f1),
    IFM_(R4000, 0x166),
    IFM_(SH3, 0x1a2),
    IFM_(SH3DSP, 0x1a3),
    IFM_(SH4, 0x1a6),
    IFM_(SH5, 0x1a8),
    IFM_(THUMB, 0x1c2),
    IFM_(WCEMIPSV2, 0x169),
};
#undef IFM_
#define IF_(a, b) IMAGE_FILE_##a = (1 << b)
enum COFFCharacteristics : uint16_t
{
    IF_(RELOCS_STRIPPED, 0),
    IF_(EXECUTABLE_IMAGE, 1),
    IF_(LINE_NUMS_STRIPPED, 2),
    IF_(LOCAL_SYMS_STRIPPED, 3),
    IF_(AGGRESSIVE_WS_TRIM, 4),
    IF_(LARGE_ADDRESS_AWARE, 5),
    IF_(RESERVED, 6),
    IF_(BYTES_RESERVED_LO, 7),  // Dep'd
    IF_(32BIT_MACHINE, 8),
    IF_(DEBUG_STRIPPED, 9),
    IF_(REMOVEABLE_RUN_FROM_SWAP, 10),
    IF_(NET_RUN_FROM_SWAP, 11),
    IF_(SYSTEM, 12),
    IF_(DLL, 13),
    IF_(UP_SYSTEM_ONLY, 14),
    IF_(BYTES_REVERSED_HI, 15)
};
#undef IF_
enum OptionalHeaderMagic : uint16_t
{
    PE32 = 0x10b,
    PE32Plus = 0x20b,
    ROMImage = 0x107
};
#define IS_(a, b) IMAGE_SUBSYSTEM_##a = b
enum WindowsSubsystem : uint16_t
{
    IS_(UNKNOWN, 0),
    IS_(NATIVE, 1),
    IS_(WINDOWS_GUI, 2),
    IS_(WINDOWS_CUI, 3),
    IS_(POSIX_CUI, 7),
    IS_(WINDOWS_CE_GUI, 9),
    IS_(EFI_APPLICATION, 10),
    IS_(EFI_BOOT_SERVICE_DRIVER, 11),
    IS_(EFI_RUNTIME_DRIVER, 12),
    IS_(EFI_ROM, 13),
    IS_(XBOX, 14)
};
#undef IS_
#define IM_(a, b) IMAGE_DLL_CHARACTERISTICS_##a = b
#define IM(a, b) IMAGE_DLLCHARACTERISTICS_##a = b
#define Res(a, b) IM_(Reserved_##a, b)
enum DLLCharacteristics : uint16_t
{
    Res(0, 0x1),
    Res(1, 0x2),
    Res(3, 0x4),
    Res(4, 0x8),
    IM_(DYNAMIC_BASE, 0x40),
    IM_(FORCE_INTEGRITY, 0x80),
    IM_(NX_COMPAT, 0x100),
    IM(NO_ISOLATION, 0x200),
    IM(NO_SEH, 0x400),
    IM(NO_BIND, 0x800),
    IM(WDM_DRIVER, 0x2000),
    IM(TERMINAL_SERVER_AWARE, 0x8000)
};
#undef IM_
#undef IM

#define IM_(a, b) IMAGE_SCN_##a = b
#define Res(a, b) IM_(Reserved_##a, b)
enum SectionCharacteristics : uint64_t
{
    Res(0, 0x0),
    Res(1, 0x1),
    Res(2, 0x2),
    Res(3, 0x4),
    IM_(TYPE_NO_PAD, 0x8),  // Should not pad section to next boundary, obsolete and replaced by IMAGE_SCN_ALIGN_1BYTES. Valid only
                            // for object files
    Res(4, 0x10),
    IM_(CNT_CODE, 0x20),                // The section contains executable code
    IM_(CNT_INITIALIZED_DATA, 0x40),    // The section contains initialized data
    IM_(CNT_UNINITIALIZED_DATA, 0x80),  // the section contains uninitialized data
    IM_(LNK_OTHER, 0x100),              // reserved
    IM_(LNK_INFO, 0x200),  // Section contains comments or other info, .drectve section has this, valid only for objects
    Res(5, 0x400),
    IM_(LNK_REMOVE, 0x800),  // Section will not become part of the image, valid only for object files
    IM_(LNK_COMDAT,
        0x1000),  // Section contains COMDAT data. For more info see section 5.5.6 of pecoff_v8.doc Valid only for object files
    IM_(GPREL, 0x8000),               // Section contains data referenced through the global pointer (GP)
    IM_(SCN_MEM_PURGEABLE, 0x20000),  // Reserved for future use
    IM_(SCN_MEM_16BIT,
        0x20000),  // For ARM machine types, the section contains Thumb code, reserved for future use with other machine types
    IM_(SCN_MEM_LOCKED, 0x40000),   // Reserved for future use
    IM_(SCN_MEM_PRELOAD, 0x80000),  // Reserved for future use

// Disable formatting so it's clear wtf is going on in this section
// clang-format off
// These are clearly not "flags", they are actually just a number that represents something
// Next up: valid only for object files
// COMMENT APPLICABLE TO ALL FOLLOWING alignment bytes: align data on N byte boundary
#define BYTES(a, b) IM_(SCN_ALIGN_##a##BYTES, b)
    BYTES(1,    0x100000),
    BYTES(2,    0x200000),
    BYTES(4,    0x300000),
    BYTES(8,    0x400000),
    BYTES(16,   0x500000),
    BYTES(32,   0x600000),
    BYTES(64,   0x700000),
    BYTES(128,  0x800000),
    BYTES(256,  0x900000),
    BYTES(512,  0xA00000),
    BYTES(1024, 0xB00000),
    BYTES(2048, 0xC00000),
    BYTES(4096, 0xD00000),
    BYTES(8192, 0xE00000),
#undef BYTES
    // clang-format on

    // Valid for anything again
    // Indicates that the count of relocations for the section exceeds the 16 bits that are reserved for it in the section header,
    // if the bit is set, and the NumberOfRelocations field in the section header is 0xffff, the actual relocation count is stored
    // in the 32-bit VirtualAddress field of the first relocation, it is an error if IMAGE_SCN_LNK_NRELOC_OVFL is set and there are
    // fewer than 0xffff relocations in the section.
    IM_(LNK_NRELOC_OVFL, 0x01000000),  // Section contains extended relocations
    IM_(MEM_DISCARDABLE, 0x02000000),  // Can be discarded as needed
    IM_(MEM_NOT_CACHED, 0x04000000),   // Section cannot be cached
    IM_(MEM_NOT_PAGED, 0x08000000),    // Section is not pageable
    IM_(MEM_SHARED, 0x10000000),       // The section can be shared in memory
    IM_(MEM_EXECUTE, 0x20000000),      // The section can be executed as code
    IM_(MEM_READ, 0x40000000),         // The section can be read
    IM_(MEM_WRITE, 0x80000000)         // The section can be written to
};
#undef IM_
#undef Res
}  // namespace COFF
}  // namespace OrangeC