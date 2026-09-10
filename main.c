#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

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
    unsigned char* name;
}symtab_entry_t;

typedef struct symtab {
    symtab_entry_t* symtab_entry;
    int size;
}symtab_t;

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
    puts("");
}

uint64_t uctoull(unsigned char* str, size_t len) {
    uint64_t result = 0;
        for (size_t i = 0; i < len; i++) {
            result |= (uint64_t)str[i] << (i * 8);
    }
    return result;
}

e_header_t* read_elf_headers(unsigned char* buff) {
        unsigned char header_ident[16];
        memcpy(header_ident, buff, sizeof(header_ident));
        unsigned char x86_64_header[16] = {0x7f, 0x45, 0x4c, 0x46, 0x02, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

        if (memcmp(header_ident, x86_64_header, 16) != 0) {
            printf("Not a valid x86_64 ELF binary.\n");
            exit(1);
        }  
  
        e_header_t* e_header = malloc(1 * sizeof(struct elf_header)); 
        // Get file offset of section headers
        memcpy(e_header->e_shoff, buff+0x28, sizeof(e_header->e_shoff)); 
        e_header->e_shoff_val = uctoull(e_header->e_shoff, sizeof(e_header->e_shoff)); 
      
        // Get size of each sh entry 
        memcpy(e_header->e_shentsize, buff+0x3a, sizeof(e_header->e_shentsize)); 
        e_header->e_shentsize_val= uctoull(e_header->e_shentsize, sizeof(e_header->e_shentsize));
 
        // Get number of sh entries
        memcpy(e_header->e_shnum, buff+0x3c, sizeof(e_header->e_shnum)); 
        e_header->e_shnum_val= uctoull(e_header->e_shnum, sizeof(e_header->e_shnum));

        // Gets location of strtab entry in shstr
        memcpy(e_header->e_shstrndx, buff+0x3e, sizeof(e_header->e_shstrndx)); 
        e_header->e_shstrndx_val = uctoull(e_header->e_shstrndx, sizeof(e_header->e_shstrndx));

        return e_header;
}


sh_entry_t* get_section(unsigned char* buff, e_header_t* e_header, const char* sh_name) {
    uint64_t sh_entry_idx = 1; 

    sh_entry_t* section = malloc(1 * sizeof(struct section_header_entry));
    while (sh_entry_idx < e_header->e_shnum_val) {
        unsigned char sh_name_offset[4];
        memcpy(sh_name_offset, buff+e_header->e_shoff_val+(sh_entry_idx*e_header->e_shentsize_val), 0x4);
        uint64_t sh_name_offset_val = uctoull(sh_name_offset, sizeof(sh_name_offset));
        char* name = (char*) buff+e_header->strtab_offset_val+sh_name_offset_val+1;
   
        if (strcmp(name, sh_name) == 0) {
            section->name = malloc(strlen(name)+1);
            memcpy(section->name, name, strlen(name));
            section->name[strlen(name)] = 0;
            section->offset = uctoull(buff+e_header->e_shoff_val+(sh_entry_idx*0x40)+0x18, sizeof(uint64_t));
            section->size = uctoull(buff+e_header->e_shoff_val+(sh_entry_idx*0x40)+0x20, sizeof(uint64_t));
            break;
        }
        sh_entry_idx += 1;
    }
    return section; 
}

sections_t* read_sh_headers(e_header_t* e_header, unsigned char* buff) {
    int size = 8;
    sections_t* code_sections = malloc(size * sizeof(struct sections));

    // Get offset of strtab sh entry
    uint64_t strtab_entry = e_header->e_shoff_val+0x40*e_header->e_shstrndx_val;

    // Retrieve bytes containing file offset of strtab
    memcpy(e_header->strtab_offset, buff+strtab_entry+0x18, sizeof(e_header->strtab_offset)); // 0x18th byte of sh entry has file offset of section
    e_header->strtab_offset_val = uctoull(e_header->strtab_offset, sizeof(e_header->strtab_offset));

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
    code_sections[6].type = METADATA;
    code_sections[7].sh_entry = get_section(buff, e_header, "strtab");
    code_sections[7].type = METADATA;
    code_sections->size = size;
    
    return code_sections;
}

// Read offsets of every symbol
// When printing, if current line = any offset of the symbols (plus pruning for stuff outside of text etc) insert symbol marker line
symtab_t* read_symtab(unsigned char* buff, sections_t* code_sections) {  
    symtab_t* symtab = malloc(1 * sizeof(struct symtab));
    symtab->size = 1;
    for (uint64_t i = 0x18; i < code_sections[6].sh_entry->size; i += 0x18) {
        unsigned char st_type[4];
        memcpy(st_type, buff+code_sections[6].sh_entry->offset+i+0x4, sizeof(st_type));
        uint64_t st_type_val = (uctoull(st_type, sizeof(st_type))&0xf); 

        if (st_type_val != 2) {
            continue;
        }

        unsigned char strtab_entry[4];
        memcpy(strtab_entry, buff+code_sections[6].sh_entry->offset+i, sizeof(strtab_entry));
        uint64_t strtab_entry_val = uctoull(strtab_entry, sizeof(strtab_entry));
        
        char* name = (char*) buff+code_sections[7].sh_entry->offset+strtab_entry_val;

        unsigned char offset[4];
        memcpy(offset, buff+code_sections[6].sh_entry->offset+i+0x8, sizeof(offset));
        uint64_t offset_val = uctoull(offset, sizeof(offset)); 

        symtab_entry_t* entry = malloc(1 * sizeof(struct symtab_entry));
        entry->offset = offset_val;
        entry->name = malloc(strlen(name)+1);
        memcpy(entry->name, name, strlen(name));
        entry->name[strlen(name)] = 0;

        symtab[symtab->size-1].symtab_entry = entry; 
        symtab->size++;
        symtab = realloc(symtab, symtab->size * sizeof(struct symtab));
    }
    return symtab;
}

void dealloc(e_header_t* e_header, sections_t* code_sections, symtab_t* symtab) {
   free(e_header);
   for (int i = 0; i < code_sections->size; i++) {
        free(code_sections[i].sh_entry->name);
        free(code_sections[i].sh_entry);
   }
   free(code_sections);
   fflush(stdout);
   for (int i = 0; i < symtab->size-1; i++) {
        free(symtab[i].symtab_entry->name);
        free(symtab[i].symtab_entry);
   }
   free(symtab);
}

void dump(unsigned char* buff, sections_t* code_sections) {
    puts("");
    for (int i = 0; i < code_sections->size; i++) {
        if (code_sections[i].type == CODE) {
            printf("Dissassembly of section: .%s\n\n", code_sections[i].sh_entry->name);
            printf("%016lx <_%s>:\n", code_sections[i].sh_entry->offset, code_sections[i].sh_entry->name); // This is wrong, this should be function names
            print_bytes(buff, code_sections[i].sh_entry->offset, code_sections[i].sh_entry->offset+code_sections[i].sh_entry->size);
            puts("\n");
        } 
    }
}

void read_headers(unsigned char* buff) {
    e_header_t* e_header = read_elf_headers(buff);
    sections_t* code_sections = read_sh_headers(e_header, buff);
    symtab_t* symtab = read_symtab(buff, code_sections);
    dump(buff, code_sections);
    dealloc(e_header, code_sections, symtab);
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
