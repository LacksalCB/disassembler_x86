#ifndef HEADER_READER_C
#define HEADER_READER_C

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

typedef struct elf_header {
    unsigned char e_ident[4];
    unsigned char e_format;
    unsigned char e_endianness;
    unsigned char e_elf_version;
    unsigned char e_target_os;
    unsigned char e_abi_version;
    unsigned char e_padding[7];
    unsigned char e_type[2];
    unsigned char e_machine[2];
    unsigned char e_version[4];
    unsigned char e_entry[8];
    unsigned char e_phoff[8]; 
    unsigned char e_shoff[8];
    uint64_t e_shoff_val;
    unsigned char e_flags[4];
    unsigned char e_ehsize[2];
    unsigned char e_phentsize[2];
    unsigned char e_phnum[2];
    unsigned char e_shentsize[2];
    uint64_t e_shentsize_val;
    unsigned char e_shnum[2];
    uint64_t e_shnum_val;
    unsigned char e_shstrndx[2];
    uint64_t e_shstrndx_val;
    // Custom values for better record keeping:
    unsigned char strtab_offset[8];
    uint64_t strtab_offset_val;
}e_header_t;

typedef struct section_header_entry {
    char* name;
    uint64_t offset;
    uint64_t size;
    uint64_t shstrtab_idx;
}sh_entry_t;

typedef struct sections {
    enum SH_SECTION_TYPES {
        CODE,
        DATA,
        METADATA
    }type;
   sh_entry_t* sh_entry;
   int size;
}sections_t;

// Should move to hashtable for efficiency but alas
typedef struct symtab_entry {
    uint64_t offset;
    char* name;
    uint64_t section;
}symtab_entry_t;

typedef struct symtab {
    symtab_entry_t* symtab_entry;
    int size;
}symtab_t;

typedef struct headers {
    e_header_t* e_header;
    sections_t* sections;
    symtab_t* symtab;
}header_t;

uint64_t uctoull(unsigned char* str, size_t len);
e_header_t* read_elf_headers(unsigned char* buff);
sh_entry_t* get_section(unsigned char* buff, e_header_t* e_header, const char* sh_name);
sections_t* read_sh_headers(e_header_t* e_header, unsigned char* buff);
symtab_t* read_symtab(unsigned char* buff, sections_t* code_sections); 
header_t* read_headers(unsigned char* buff);

#endif /* HEADER_READER_C */
