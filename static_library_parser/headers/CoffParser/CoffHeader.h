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
    // 8 byte, null-padded UTF-8 encoded string If it is exactly 8 characters long, there is no terminating null
    char Name[8];
    // total size of section when loaded into memory, if this value is greater than SizeOfawData, the section
    // is zero padded, only valid for executable images and should be set to zero for object files
    uint32_t VirtualSize;
    // For exec, addr of first byte of the section relative to the image base when section is loaded into
    // mem, for object, the field is the addr of the first byte before relocation applied, for simplicity,
    // 0, otherwise it is arbitrary that is subtracted from offsets during relocation.
    uint32_t VirtualAddress;
    // Size of the section (for object files) or the size of the initialized data on the disk (for image
    // files), for executable images, must be a multiple of FileAlignment from the optional header, if this
    // is less than VirtualSize, the remainder of the section is zero-filled. This is rounded, so this must
    // be LESS than VirtualSize as well, when a section contains only uninitialized data, this should be ZERO
    uint32_t SizeOfRawData;
    // The file pointer to the first page of the section within the COFF file, for executable images, this
    // must be a multiple of FileAlignment from the optional header. For object files, FOR BEST PERFORMANCE:
    // align to a 4-byte boundary, when there is only uninitialized data, field should be zero
    uint32_t PointerToRawData;
    // The file pointer to the beginning of relocation entries for the section. Zero for executables
    // or if no relocs
    uint32_t PointerToRelocations;
    // File pointer to the beginning of line-number entries for the section, should be ZZERO if
    // there are no COFF line numbers, should also be 0 since COFF debugging info is deprecated
    uint32_t PointerToLineNumbers;
    // Number of relocation entries for the section, ZERO for executables
    uint32_t NumberOfRelocations;
    // line-number entries, should be ZERO for an image due to deprecation of COFF debugging info
    uint32_t NumberOfLineNumbers;
    // Flags that describe characteristics of section, see SectionCharacteristics or
    // section 4.1 "section flags" of pecoff_v8.doc
    SectionCharacteristics Characteristics;
};
// OBJECT ONLY!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
struct COFFRelocations
{
    // Address of the item to which relocation is applied, This is offset from beginning of section, plus
    // the value of the RVA/Offset field, See section 4 of pecoff_v8.doc for more.
    uint32_t VirtualAddress;
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
    // When non-zero, specifies a one based line number, when zero, the Type field is interpreted as a symbol
    // table index for a function
    uint16_t LineNumber;
};
union CoffSymbolName
{
    // An array of 8 bytes, the array is padded with nulls on the right if the name is less than 8 bytes long
    char ShortName[8];
    struct
    {
        // Zero if the data is longer than 8 bytes
        uint32_t Zeroes;
        // Offset into the string table(?) if it's longer than 8 bytes
        uint32_t Offset;
    };
};
struct CoffSymbolTableRecord
{
    // Array of 8 bytes if not longer than 8 bytes long
    CoffSymbolName Name;
    // The value that is associated with the symbol. The interpretation of this field depends on SectionNumber and
    // StorageClass, a typical meaning is the relocatable address
    uint32_t Value;
    // A signed integer that identifies the section, using a 1 based index into the scetion table, 5.4.2
    // section number values pecoff_v8.doc
    int16_t SectionNumber;
    // A number that represents a type, 0x20 for function, 0x0 for not a function
    uint16_t Type;
    // An enumerated value that represents storage class, see section 4.4.4 of pecoff_v8 for more information
    CoffSymbolClass StorageClass;
    // Typically no more than 1
    uint8_t NumberOfAuxSymbols;
};
#define aux_check_size(a) static_assert(sizeof(a) == 18, #a " is sized incorrectly");
#pragma pack(push)
#pragma pack(2)
// Function-definition symbol records are followed by an auxiliary record in the format described in the following table.
struct AuxFunctionDefinition
{
    // Symbol Table index of the corresponding .bf (begin function) symbol record
    uint32_t TagIndex;
    // The size of the executable code, if the function is in its own section, the SizeOfRawData in the section header is greater
    // than or equal to this field depending on the alignment considered
    uint32_t TotalSize;
    // The file offset of the first COFF line-number entry for the function, or zero if none exists, for more information, see
    // section 4.3: "COFF Line numbers (Deprecated)"
    uint32_t PointerToLineNumber;
    // The symbol table index of the record for the next function. If the function is the last in the symbol table, this field is
    // set to zero
    uint32_t PointerToNextFunction;
    // Unused
    uint16_t Unused;
};
aux_check_size(AuxFunctionDefinition);
// BF and EF records are followed by this
struct AuxFunctionBFEF
{
    uint32_t Unused1;
    // The actual ordinal line number (1, 2, 3, etc.) within the source file, corresponding to the bf or ef record
    uint16_t LineNumber;
    uint8_t Unused2[6];
    // The symbol table index of the next .bf symbol record, if the function is last in the symbol table, this field is set to zero.
    // It is not used for .ef records
    uint32_t PointerToNextFunction;
    uint16_t Unused;
};
aux_check_size(AuxFunctionBFEF);
// Weak externals are represented by a symbol table record with EXTERNAL storage class, UNDEF section number, and a value of zero.
// The weak-external symbol record is followed by an auxiliary record with the following format.
// Note that the Characteristics field is not defined in WINNT.H; instead, the Total Size field is used.
struct AuxWeakExternals
{
    // The symbol-table index of Sym2, the symbol to be linked if sym1 is not found
    uint32_t TagIndex;
    WeakExternalCharacteristics Characteristics;
    uint8_t Unused[10];
};
aux_check_size(AuxWeakExternals);
// This format follows a symbol-table record with storage class FILE (103). The symbol name itself should be .file, and the
// auxiliary record that follows it gives the name of a source-code file.
struct AuxFiles
{
    // ANSI string of the name of the file, Zero-padded with nulls if it is less than the maximum length of a file name
    char FileName[18];
};
aux_check_size(AuxFiles);
struct AuxSectionDefinition
{
    // Size of the section header, same as SizeOfRawData
    uint32_t Length;
    // Relocation entries for section
    uint16_t NumberOfRelocations;
    // The number of line-number entires for the section
    uint16_t NumberOfLineNumbers;
    // The checksum for communal data, it is applicable if IMAGE_SCN_LNK_COMDAT is set in the section header, for more information
    // see section 4.5.6 "COMDAT Sections (Object Only)"
    uint32_t CheckSum;
    // One based index into the section table for the associated section, this is used when the COMDAT selection setting is 5
    uint16_t Number;
    // COMDAT selection number, only applicable if COMDAT Section
    uint8_t Selection;
    uint8_t padding[3];
};
aux_check_size(AuxSectionDefinition);
struct CLRTokenDefinition
{
    // Must be IMAGE_AUX_SYMBOL_TYPE_TOKEN (1)
    uint8_t bAuxType;
    // Reserved: must be zero
    uint8_t bReserved;
    // The symbol index of the COFF symbol to which this CLR token definition refers
    uint32_t SymbolTableIndex;
    uint8_t ReservedAgain[12];
};
aux_check_size(CLRTokenDefinition);
union AuxSymRecord
{
    AuxFiles files;
    AuxSectionDefinition sectionDefinition;
    AuxWeakExternals weakExternal;
    AuxFunctionBFEF BFEF;
    AuxFunctionDefinition functionDefinition;
    CLRTokenDefinition CLRToken;
};
aux_check_size(AuxSymRecord);
#undef aux_check_size
#pragma pack(pop)
// Y O U M U S T S E E: Section 4.7 Attribute Certificate Table (Image Only)
struct AttributeCertificateTable
{
    // Specifies the length of bCertificate
    uint32_t dwLength;
    // Contains the certificate version number, see section 4.7 of pecoff_v8.doc
    WinCertRevisions wRevision;
    // Specifies the type of content in bCertificate, see section 4.7 of pecoff_v8.doc
    WinCertificateType wCertificateType;
    // Contains a certificate ??????????????????????????, such as an authenticode signature, can be A N Y S I Z E, so we define it
    // as one
    uint8_t bCertificate[1];
};
struct DelayLoadImportTable
{
    // Reserved, must be 0
    uint32_t Attributes;
    // RVA Name of the DLL to be loaded, the name resides in the read-only section of the image
    uint32_t Name;
    // RVA of the Module Handle of the DLL to be delay-loaded, it is used for storage by the routine that is supplied to manage
    // delay-loading
    uint32_t ModuleHandle;
    // The RVA of the delay-load import address table, for more information, see section 4.8.5 "Delay Import Address Table (IAT)" in
    // pecoff_v8.doc
    uint32_t DelayImportAddressTable;
    // The RVA of the delay-load name table, which contains the names of the imports that might need to be loaded. This matches the
    // layout of the import name table. For more information, see section 5.4.3 "Hint/Name Table" of pecoff_v8.doc
    uint32_t DelayImportNameTable;
    // The RVA of the bound delay-load address table, if it exists
    uint32_t BoundDelayImportTable;
    // The RVA of the unload delay-load address table, if it exists. This is an exact copy of the delay import address table. If the
    // caller unloads the DLL, this table should be copied back over the delay import address table so that subsequent calls to the
    // DLL continue to use the thunking method correctly
    uint32_t UnloadDelayImportTable;
    // The timestamp of the DLL to which this image has been bound
    uint32_t TimeStamp;
};

}  // namespace COFF
}  // namespace OrangeC