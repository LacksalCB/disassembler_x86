#include "../include/header_reader.h"

// This function's existence is pointless and a epistemic of poor design practices.
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
        // x86_64 ELF statically linked
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
            section->name = name;
            section->offset = uctoull(buff+e_header->e_shoff_val+(sh_entry_idx*0x40)+0x18, sizeof(uint64_t));
            section->size = uctoull(buff+e_header->e_shoff_val+(sh_entry_idx*0x40)+0x20, sizeof(uint64_t));
            section->shstrtab_idx = sh_entry_idx;
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

symtab_t* read_symtab(unsigned char* buff, sections_t* code_sections) { 
    symtab_t* symtab = malloc((code_sections[6].sh_entry->size/0x18) * sizeof(struct symtab));
    symtab->size = code_sections[6].sh_entry->size/0x18;
    for (uint64_t i = 0; i < code_sections[6].sh_entry->size/0x18; i += 1) {
        symtab_entry_t* entry = malloc(1 * sizeof(struct symtab_entry));

        unsigned char offset[4];
        memcpy(offset, buff+code_sections[6].sh_entry->offset+i*0x18+0x8, sizeof(offset));
        entry->offset = uctoull(offset, sizeof(offset)); 

        unsigned char strtab_entry[4];
        memcpy(strtab_entry, buff+code_sections[6].sh_entry->offset+i*0x18, sizeof(strtab_entry));
        uint64_t strtab_entry_val = uctoull(strtab_entry, sizeof(strtab_entry));        
        entry->name = (char*) buff+code_sections[7].sh_entry->offset+strtab_entry_val;

        unsigned char st_type[2];
        memcpy(st_type, buff+code_sections[6].sh_entry->offset+i*0x18+0x6, sizeof(st_type));
        uint64_t st_type_val = (uctoull(st_type, sizeof(st_type)));
        entry->section = st_type_val;
            
        symtab[i].symtab_entry = entry; 
    }
    return symtab;
}

header_t* read_headers(unsigned char* buff) {
    header_t* headers = malloc(1*sizeof(struct headers));
    headers->e_header = read_elf_headers(buff);
    headers->sections = read_sh_headers(headers->e_header, buff);
    headers->symtab = read_symtab(buff, headers->sections);

    return headers;
}
