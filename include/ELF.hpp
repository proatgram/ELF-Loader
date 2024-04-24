#include <stdint.h>
#include <stdlib.h>

#include "llist.hpp"

struct ELFHeader {
    enum class E_TYPE : uint16_t {
        ET_NONE = 0,
        ET_REL = 1,
        ET_EXEC = 2,
        ET_DYN = 3,
        ET_CORE = 4,
        ET_LOOS = 0xFE00,
        ET_HIOS = 0xFEFF,
        ET_LOPROC = 0xFF00,
        ET_HIPROC = 0xFFFF
    };

    enum class E_MACHINE : uint16_t {
        EM_NONE = 0,
        EM_M32 = 1,
        EM_SPARC = 2,
        EM_386 = 3,
        EM_68K = 4,
        EM_88K = 5,
        EM_860 = 7,
        EM_MIPS = 8,
        EM_S370 = 9,
        EM_MIPS_RS3_LE = 10,
        EM_PARISC = 15,
        EM_VPP500 = 17,
        EM_SPARC32PLUS = 18,
        EM_960 = 19,
        EM_PPC = 20,
        EM_PPC64 = 21,
        EM_S390 = 22,
        EM_V800 = 36,
        EM_FR20 = 37,
        EM_RH32 = 38,
        EM_RCE = 39,
        EM_ARM = 40,
        EM_ALPHA = 41,
        EM_SH = 42,
        EM_SPARCV9 = 43,
        EM_TRICORE = 44,
        EM_ARC = 45,
        EM_H8_300 = 46,
        EM_H8_300H = 47,
        EM_H8S = 48,
        EM_H8_500 = 49,
        EM_IA_64 = 50,
        EM_MIPS_X = 51,
        EM_COLDFIRE = 52,
        EM_68HC12 = 53,
        EM_MMA = 54,
        EM_PCP = 55,
        EM_NCPU = 56,
        EM_NDR1 = 57,
        EM_STARCORE = 58,
        EM_ME16 = 59,
        EM_ST100 = 60,
        EM_TINYJ = 61,
        EM_X86_64 = 62,
        EM_PDSP = 63,
        EM_PDP10 = 64,
        EM_PDP11 = 65,
        EM_FX66 = 66,
        EM_ST9PLUS = 67,
        EM_ST7 = 68,
        EM_68HC16 = 69,
        EM_68HC11 = 70,
        EM_68HC08 = 71,
        EM_68HC05 = 72,
        EM_SVX = 73,
        EM_ST19 = 74,
        EM_VAX = 75,
        EM_CRIS = 76,
        EM_JAVELIN = 77,
        EM_FIREPATH = 78,
        EM_ZSP = 79,
        EM_MMIX = 80,
        EM_HUANY = 81,
        EM_PRISM = 82,
        EM_AVR = 83,
        EM_FR30 = 84,
        EM_D10V = 85,
        EM_D30V = 86,
        EM_V850 = 87,
        EM_M32R = 88,
        EM_MN10300 = 89,
        EM_MN10200 = 90,
        EM_PJ = 91,
        EM_OPENRISC = 92,
        EM_ARC_A5 = 93,
        EM_XTENSA = 94,
        EM_VIDEOCORE = 95,
        EM_TMM_GPP = 96,
        EM_NS32K = 97,
        EM_TPC = 98,
        EM_SNP1K = 99,
        EM_ST200 = 100
    };

    enum class E_VERSION : uint32_t {
        EV_NONE = 0,
        EV_CURENT = 1
    };

    enum class Indexes : uint8_t {
        EI_MAG0 = 0,
        EI_MAG1 = 1,
        EI_MAG2 = 2,
        EI_MAG3 = 3,
        EI_CLASS = 4,
        EI_DATA = 5,
        EI_VERSION = 6,
        EI_PAD = 7,
        EI_NIDENT = 16
    };

    unsigned char e_ident[static_cast<uint8_t>(Indexes::EI_NIDENT)];
    E_TYPE e_type;
    E_MACHINE e_machine;
    E_VERSION e_version;
    uint32_t e_entry;
    uint32_t e_phoff;
    uint32_t e_shoff;
    uint32_t e_flags;
    uint16_t e_ehsize;
    uint16_t e_phnum;
    uint16_t e_shentsize;
    uint16_t e_shnum;
    uint16_t e_shstrndx;
};

struct ProgramHeader {
    enum class P_TYPE : uint32_t {
        PT_NULL = 0,
        PT_LOAD = 1,
        PT_DYNAMIC = 2,
        PT_INTERP = 3,
        PT_NOTE = 4,
        PT_SHLIB = 5,
        PT_PHDR = 6,
        PT_TLS = 7,
        PT_LOOS = 0x60000000,
        PT_HIOS = 0x6FFFFFFF,
        PT_LOPROC = 0x70000000,
        PT_HIPROC = 0x7FFFFFFF
    };

    enum class P_FLAG : uint32_t {
        PF_X = 1,
        PF_W = 2,
        PF_R = 4
    };

    P_TYPE type;
    uint32_t offset;
    uint32_t vaddr;
    uint32_t paddr;
    uint32_t filesz;
    uint32_t memsz;
    P_FLAG flags;
    uint32_t align;
};

struct SymbolTableEntry {
    uint32_t name;
    uint32_t value;
    uint32_t size;
    uint8_t info;
    uint8_t other;
    uint16_t shndx;
};

// Relocation
struct Rel {
    uint32_t offset;
    uint32_t info;
};

struct Rela {
    uint32_t offset;
    uint32_t info;
    int32_t addend;
};

struct SectionHeader {
    enum class S_TYPE : uint32_t {
        SHT_NULL = 0,
        SHT_PROGBITS = 1,
        SHT_SYMTAB = 2,
        SHT_STRTAB = 3,
        SHT_RELA = 4,
        SHT_HASH = 5,
        SHT_DYNAMIC = 6,
        SHT_NOTE = 7,
        SHT_NOBITS = 8,
        SHT_REL = 9,
        SHT_SHLIB = 10,
        SHT_DYNSYM = 11,
    };

    enum class S_FLAGS : uint32_t {
        SHF_WRITE = 1,
        SHF_ALLOC = 2,
        SHF_EXECINSTR = 4,
        SHF_MASKPROC = 0xF0000000
    };
    
    uint32_t name; /* Offset into section header string table */
    S_TYPE type;
    S_TYPE flags; // Might be bitmasked (0b00000111 might be a valid combination of all three)
    uint32_t addr;
    uint32_t offset;
    uint32_t size;
    uint32_t link;
    uint32_t info;
    uint32_t addralign;
    uint32_t entsize;
};

class ELFFile {
    public:
        ELFFile();

    private:
        const ELFHeader m_header;

        SectionHeader m_sectionHeaders[];
};
