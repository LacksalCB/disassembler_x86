#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

struct elf_header {
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
};

struct section_header {
    unsigned char sh_name[4];
    uint64_t sh_name_val;
    unsigned char sh_type[4];
    unsigned char sh_flags[8];
    unsigned char sh_addr[8];
    unsigned char sh_offset[8];
    unsigned char sh_size[8];
    unsigned char sh_link[4];
    unsigned char sh_info[4];
    unsigned char sh_addralign[8];
    unsigned char sh_entsize[8];
};

typedef struct section_header_entry {
    char* name;
    uint64_t offset;
    uint64_t size;
}sh_entry_t;

typedef struct sections {
    enum SH_SECTION_TYPES {
        CODE,
        DATA
    }type;
   sh_entry_t* sh_entry;
   int size;
}sections_t;

void print_bin(unsigned char* buff, size_t size) {
    printf("%08lx: ", (uint64_t)0x00); 
    for (uint64_t i = 0; i < size; i++) {  
        printf("0x%02x ", (unsigned char) buff[i]);
        if ((i+1) % 16 == 0 || i == size) {
            printf("\n%08lx: ", i+1);
        }   
    }
}

void print_bytes(unsigned char* buff, size_t src, size_t dest) {
    printf("%08lx: ", (uint64_t)src); 
    for (uint64_t i = src; i < dest; i++) { 
        printf("0x%02x ", (unsigned char) buff[i]);
        if ((i+1) % 16 == 0 || i == dest) {
            printf("\n%08lx: ", i+1);
        }   
    } 
}

uint64_t uctoull(unsigned char* str, size_t len) {
    uint64_t result = 0;
        for (size_t i = 0; i < len; i++) {
            result |= (uint64_t)str[i] << (i * 8);
    }
    return result;
}

struct elf_header read_elf_headers(unsigned char* buff) {
        unsigned char header_ident[16];
        memcpy(header_ident, buff, sizeof(header_ident));
        unsigned char x86_64_header[16] = {0x7f, 0x45, 0x4c, 0x46, 0x02, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

        if (memcmp(header_ident, x86_64_header, 16) != 0) {
            printf("Not a valid x86_64 ELF binary.\n");
            exit(1);
        }  
  
        struct elf_header e_header; 
        // Get file offset of section headers
        memcpy(e_header.e_shoff, buff+0x28, sizeof(e_header.e_shoff)); 
        e_header.e_shoff_val = uctoull(e_header.e_shoff, sizeof(e_header.e_shoff)); 
      
        // Get size of each sh entry 
        memcpy(e_header.e_shentsize, buff+0x3a, sizeof(e_header.e_shentsize)); 
        e_header.e_shentsize_val= uctoull(e_header.e_shentsize, sizeof(e_header.e_shentsize));
 
        // Get number of sh entries
        memcpy(e_header.e_shnum, buff+0x3c, sizeof(e_header.e_shnum)); 
        e_header.e_shnum_val= uctoull(e_header.e_shnum, sizeof(e_header.e_shnum));

        // Gets location of strtab entry in shstr
        memcpy(e_header.e_shstrndx, buff+0x3e, sizeof(e_header.e_shstrndx)); 
        e_header.e_shstrndx_val = uctoull(e_header.e_shstrndx, sizeof(e_header.e_shstrndx));

        return e_header;
}


sh_entry_t* get_section(unsigned char* buff, struct elf_header e_header, const char* sh_name) {
    uint64_t sh_entry_idx = 1; // Skip entry that just says .symtab

    sh_entry_t* section = malloc(1 * sizeof(struct section_header_entry));
    while (sh_entry_idx < e_header.e_shnum_val) {
        struct section_header s_header;
   
        memcpy(s_header.sh_name, buff+e_header.e_shoff_val+(sh_entry_idx*e_header.e_shentsize_val), 0x4);
        s_header.sh_name_val = uctoull(s_header.sh_name, sizeof(s_header.sh_name));
 
        char* name = (char*) buff+e_header.strtab_offset_val+s_header.sh_name_val+1;
    
        if (strcmp(name, sh_name) == 0) {

            section->name = malloc(strlen(name)+1);
            memcpy(section->name, name, strlen(name));
            section->name[strlen(name)] = 0;
            section->offset = uctoull(buff+e_header.e_shoff_val+(sh_entry_idx*0x40)+0x18, sizeof(uint64_t));
            section->size = uctoull(buff+e_header.e_shoff_val+(sh_entry_idx*0x40)+0x20, sizeof(uint64_t));
            printf("Section: %s\nOffset: %lx\nSize: %lx\n\n", section->name, section->offset, section->size);
            break;
        }  
        sh_entry_idx += 1;
    }
    return section; 
}

// Read offsets of every symbol
// When printing, if current line = any offset of the symbols (plus pruning for stuff outside of text etc) insert symbol marker line
void read_symtab(unsigned char* buff) {
     
}

sections_t* read_sh_headers(struct elf_header e_header, unsigned char* buff) {
    int size = 7;
    sections_t* code_sections = malloc(size * sizeof(struct sections));

    // Get offset of strtab sh entry
    uint64_t strtab_entry = e_header.e_shoff_val+0x40*e_header.e_shstrndx_val;

    // Retrieve bytes containing file offset of strtab
    memcpy(e_header.strtab_offset, buff+strtab_entry+0x18, sizeof(e_header.strtab_offset)); // 0x18th byte of sh entry has file offset of section
    e_header.strtab_offset_val = uctoull(e_header.strtab_offset, sizeof(e_header.strtab_offset));

    code_sections[0].sh_entry = get_section(buff, e_header, "rodata");
    code_sections[0].type = DATA; 
    code_sections[1].sh_entry = get_section(buff, e_header, "data");
    code_sections[1].type = DATA;
    code_sections[2].sh_entry = get_section(buff, e_header, "bss");
    code_sections[2].type = DATA;
    code_sections[3].sh_entry = get_section(buff, e_header, "init");
    code_sections[3].type = CODE;
    code_sections[4].sh_entry = get_section(buff, e_header, "text");
    code_sections[4].type = CODE;
    code_sections[5].sh_entry = get_section(buff, e_header, "fini");
    code_sections[5].type = CODE;
    code_sections[6].sh_entry = get_section(buff, e_header, "symtab");
    code_sections[6].type = DATA;
    code_sections->size = size;

    return code_sections;
}

void dealloc(sections_t* code_sections) {
   for (int i = 0; i < code_sections->size; i++) {
        free(code_sections[i].sh_entry->name);
        free(code_sections[i].sh_entry);
   } 
}

// Get symtab/strtab function names (symbols)
// Symtab is in shstr, 3rd from last index (doesn't matter where)
// layout is:
// symbol name, symbol info (contains type, e.g. STTi_FUNC) also what section in SHT_SYMTAB_SHNDX

void dump(unsigned char* buff, sections_t* code_sections) {
    for (int i = 0; i < code_sections->size; i++) {
        if (code_sections[i].type == CODE) {
            printf("Dissassembly of section: .%s\n\n", code_sections[i].sh_entry->name);
            printf("%016lx <_%s>:\n", code_sections[i].sh_entry->offset, code_sections[i].sh_entry->name);
            print_bytes(buff, code_sections[i].sh_entry->offset, code_sections[i].sh_entry->offset+code_sections[i].sh_entry->size);
            puts("\n");
        } 
    }
}

void read_headers(unsigned char* buff) {
    struct elf_header e_header = read_elf_headers(buff);
    sections_t* code_sections = read_sh_headers(e_header, buff);
    dump(buff, code_sections);
    dealloc(code_sections);
}

void disas(unsigned char* buff) {
    
}

int main(int argc, char** argv) {
    FILE* bin;
    size_t size = 0;
    bin = fopen(argv[1], "rb");
    fseek(bin, 0, SEEK_END);
    size = ftell(bin);
    fseek(bin, 0, SEEK_SET);
    unsigned char* buff = malloc(size);
    fread(buff, 1, size, bin);
    fclose(bin);
   
    read_headers(buff);
    disas(buff);

    puts("");

    free(buff);
    return 0;
}
