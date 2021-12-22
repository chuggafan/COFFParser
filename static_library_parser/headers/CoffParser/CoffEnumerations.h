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
// clang-format off
#define IR_(arch, a, b) IMAGE_REL_##arch##_##a = b
enum COFFRelocType : uint16_t
{
#define IRA_(a, b) IR_(AMD64, a, b)
    IRA_(ABSOLUTE, 0x0),
    IRA_(ADDR64,   0x1),
    IRA_(ADDR32,   0x2),
    IRA_(ADDR32NB, 0x3),
    IRA_(REL32,    0x4),
    IRA_(REL32_1,  0x5),
    IRA_(REL32_2,  0x6),
    IRA_(REL32_3,  0x7),
    IRA_(REL32_4,  0x8),
    IRA_(REL32_5,  0x9),
    IRA_(SECTION,  0xA),
    IRA_(SECREL,   0xB),
    IRA_(SECREL7,  0xC),
    IRA_(TOKEN,    0xD),
    IRA_(SREL32,   0xE),
    IRA_(PAIR,     0xF),
    IRA_(SSPAN32,  0x10),
#undef IRA_
#define IRA_(a, b) IR_(ARM, a, b)
    IRA_(ABSOLUTE,  0x00),
    IRA_(ADDR32,    0x01),
    IRA_(ADDR32NB,  0x02),
    IRA_(BRANCH24,  0x03),
    IRA_(BRANCH11,  0x04),
    IRA_(TOKEN,     0x05),
    IRA_(BLX24,     0x08),
    IRA_(BLX11,     0x09),
    IRA_(SECTION,   0x0E),
    IRA_(SECREL,    0x0F),
    IRA_(MOV32A,    0x10),
    IRA_(MOV32T,    0x11),
    IRA_(BRANCH20T, 0x12),
    IRA_(BRANCH24T, 0x14),
    IRA_(BLX23T,    0x15),
#undef IRA_
#define IR3_(a, b) IR_(SH3, a, b)
#define IRM_(a, b) IR_(SHM, a, b)
    IR3_(ABSOLUTE,        0x0),
    IR3_(DIRECT16,        0x1),
    IR3_(DIRECT32,        0x2),
    IR3_(DIRECT8,         0x3),
    IR3_(DIRECT8_WORD,    0x4),
    IR3_(DIRECT8_LONG,    0x5),
    IR3_(DIRECT4,         0x6),
    IR3_(DIRECT4_WORD,    0x7),
    IR3_(DIRECT4_LONG,    0x8),
    IR3_(PCREL8_WORD,     0x9),
    IR3_(PCREL8_LONG,     0xA),
    IR3_(PCREL12_WORD,    0xB),
    IR3_(STARTOF_SECTION, 0xC),
    IR3_(SIZEOF_SECTION,  0xD),
    IR3_(SECTION,         0xE),
    IR3_(SECREL,          0xF),
    IR3_(DIRECT32_NB,     0x10),
    IR3_(GPREL4_LONG,     0x11),
    IR3_(TOKEN,           0x12),
    IRM_(PCRELPT,         0x13),
    IRM_(REFLO,           0x14),
    IRM_(REFHALF,         0x15),
    IRM_(RELLO,           0x16),
    IRM_(RELHALF,         0x17),
    IRM_(PAIR,            0x18),
    IRM_(NOMODE,          0x8000),
#undef IR3_
#undef IRM_
#define IRP_(a, b) IR_(PPC, a, b)
    IRP_(ABSOLUTE, 0x0),
    IRP_(ADDR64,   0x1),
    IRP_(ADDR32,   0x2),
    IRP_(ADDR24,   0x3),
    IRP_(ADDR16,   0x4),
    IRP_(ADDR14,   0x5),
    IRP_(REL24,    0x6),
    IRP_(REL14,    0x7),
    IRP_(ADDR32NB, 0xA),
    IRP_(SECREL,   0xB),
    IRP_(SECTION,  0xC),
    IRP_(SECREL16, 0xF),
    IRP_(REFHI,    0x10),
    IRP_(REFLO,    0x11),
    IRP_(PAIR,     0x12),
    IRP_(SECRELLO, 0x13),
    IRP_(GPREL,    0x15),
    IRP_(TOKEN,    0x16),
#undef IRP_
#define IRI_(a, b) IR_(I386, a, b)
    IRI_(ABSOLUTE, 0x0),
    IRI_(DIR16,    0x1),
    IRI_(REL16,    0x2),
    IRI_(DIR32,    0x6),
    IRI_(DIR32NB,  0x7),
    IRI_(SEG12,    0x9),
    IRI_(SECTION,  0xA),
    IRI_(SECREL,   0xB),
    IRI_(TOKEN,    0xC),
    IRI_(SECREL7,  0xD),
    IRI_(REL32,    0x14),
#undef IRI_
#define IRI_(a, b) IR_(IA64, a, b)
    IRI_(ABSOLUTE,   0x0),
    IRI_(IMM14,      0x1),
    IRI_(IMM22,      0x2),
    IRI_(IMM64,      0x3),
    IRI_(DIR32,      0x4),
    IRI_(DIR64,      0x5),
    IRI_(PCREL21B,   0x6),
    IRI_(PCREL32M,   0x7),
    IRI_(PCREL21F,   0x8),
    IRI_(GPREL22,    0x9),
    IRI_(LTOFF22,    0xA),
    IRI_(SECTION,    0xB),
    IRI_(SECREL22,   0xC),
    IRI_(SECREL64I,  0xD),
    IRI_(SECREL32,   0xE),
    IRI_(DIR32NB,    0x10),
    IRI_(SREL14,     0x11),
    IRI_(SREL22,     0x12),
    IRI_(SREL32,     0x13),
    IRI_(UREL32,     0x14),
    IRI_(PCREL60X,   0x15),
    IRI_(PCREL60B,   0x16),
    IRI_(PCREL60F,   0x17),
    IRI_(PCREL60I,   0x18),
    IRI_(PCREL60M,   0x19),
    IRI_(IMMGPREL64, 0x1a),
    IRI_(TOKEN,      0x1b),
    IRI_(GPREL32,    0x1c),
    IRI_(ADDEND,     0x1f),
#undef IRI_
#define IRM_(a, b) IR_(MIPS, a, b)
    IRM_(ABSOLITE,  0x0),
    IRM_(REFHALF,   0x1),
    IRM_(REFWORD,   0x2),
    IRM_(IMPADDR,   0x3),
    IRM_(REFHI,     0x4),
    IRM_(REFLO,     0x5),
    IRM_(GPREL,     0x6),
    IRM_(LITERAL,   0x7),
    IRM_(SECTION,   0xA),
    IRM_(SECREL,    0xB),
    IRM_(SECRELLO,  0xC),
    IRM_(SECRELHI,  0xD),
    IRM_(IMPADDR16, 0x10),
    IRM_(REFWORDNB, 0x22),
    IRM_(PAIR,      0x25),
#undef IRM_
#define IRM_(a, b) IR_(M32R, a, b)
    IRM_(ABSOLUTE,  0x0),
    IRM_(ADDR32,    0x1),
    IRM_(ADDR32NB,  0x2),
    IRM_(ADDR24,    0x3),
    IRM_(GPREL16,   0x4),
    IRM_(PCREL24,   0x5),
    IRM_(PCREL16,   0x6),
    IRM_(PCREL8,    0x7),
    IRM_(REFHALF,   0x8),
    IRM_(REFHI,     0x9),
    IRM_(REFLO,     0xA),
    IRM_(PAIR,      0xB),
    IRM_(SECTION,   0xC),
    IRM_(SECREL,    0xD),
    IRM_(TOKEN,     0xE),
#undef IRM_
};
#undef IR_
// clang-format on
enum SectionNumberSpecial
{
    IMAGE_SYM_UNDEFINED = 0,
    IMAGE_SYM_ABSOLUTE = -1,
    IMAGE_SYM_DEBUG = -2,
};
// Type representation: MSB is complex, LSB is the base
#define IST_(a) IMAGE_SYM_TYPE_##a
// MS tools leave the base type as 0 usually
enum CoffSymbolTableTypeBase : uint8_t
{
    IMAGE_SYM_TYPE_NULL = 0,
    IST_(VOID),
    IST_(CHAR),
    IST_(SHORT),
    IST_(INT),
    IST_(LONG),
    IST_(FLOAT),
    IST_(DOUBLE),
    IST_(STRUCT),
    IST_(UNION),
    IST_(ENUM),
    IST_(MOE),
    IST_(BYTE),
    IST_(WORD),
    IST_(UINT),
    IST_(DWORD),
};
#undef IST_
#define ISD_(a) IMAGE_SYM_DTYPE_##a
enum CoffSymbolTableDerived : uint8_t
{
    IMAGE_SYM_DTYPE_NULL = 0,
    ISD_(POINTER),
    ISD_(FUNCTION),
    ISD_(ARRAY),
};
#undef ISD_
#define ISC_(a, b) IMAGE_SYM_CLASS_##a = b
// All documentation is in section 4.4.4 of pecoff_v8.doc
enum CoffSymbolClass : uint8_t
{
    ISC_(END_OF_FUNCTION, -1),
    ISC_(NULL, 0),
    ISC_(AUTOMATIC, 1),
    ISC_(EXTERNAL, 2),
    ISC_(STATIC, 3),
    ISC_(REGISTER, 4),
    ISC_(EXTERNAL_DEF, 5),
    ISC_(LABEL, 6),
    ISC_(UNDEFINED_LABEL, 7),
    ISC_(MEMBER_OF_STRUCTU, 8),
    ISC_(ARGUMENT, 9),
    ISC_(STRUCT_TAG, 10),
    ISC_(MEMBER_OF_UNION, 11),
    ISC_(UNION_TAG, 12),
    ISC_(TYPE_DEFINITION, 13),
    ISC_(UNDEFINED_STATIC, 14),
    ISC_(ENUM_TAG, 15),
    ISC_(MEMBER_OF_ENUM, 16),
    ISC_(REGISTER_PARAM, 17),
    ISC_(BIT_FIELD, 18),
    ISC_(CLASS_BLOCK, 100),
    ISC_(CLASS_FUNCTION, 101),
    ISC_(END_OF_STRUCT, 102),
    ISC_(FILE, 103),
    ISC_(SECTION, 104),
    ISC_(WEAK_EXTERNAL, 105),
    ISC_(CLR_TOKEN, 107)
};
#undef ISC_
// Using the LLVM definition here since it's not given *anywhere* in the microsoft documentation, presumably found in the dark
// underbelly known as winnt.h
enum WeakExternalCharacteristics : uint32_t
{
    IMAGE_WEAK_EXTERN_SEARCH_NOLIBRARY = 1,
    IMAGE_WEAK_EXTERN_SEARCH_LIBRARY = 2,
    IMAGE_WEAK_EXTERN_SEARCH_ALIAS = 3,
    IMAGE_WEAK_EXTERN_ANTI_DEPENDENCY = 4
};
#define ICS_(a) IMAGE_COMDAT_SELECT_##a
enum COFFComdatSelection : uint8_t
{
    ICS_(NODPULICATES),
    ICS_(ANY),
    ICS_(SAME_SIZE),
    ICS_(EXACT_MATCH),
    ICS_(ASSOCIATIVE),
    ICS_(LARGEST)
};
#undef ICS_
enum WinCertRevisions : uint16_t
{
    WIN_CERT_REVISION_1_0 = 0x0100,
    WIN_CERT_REVISION_2_0 = 0x0200,
};
enum WinCertificateType : uint16_t
{
    WIN_CERT_TYPE_X509 = 1,
    WIN_CERT_TYPE_PKCS_SIGNED_DATA = 2,
    WIN_CERT_TYPE_RESERVED_1 = 3,
    WIN_CERT_TYPE_TS_STACK_SIGNED = 4,  // Not supported
};
}  // namespace COFF
}  // namespace OrangeC