#include "../include/disassembler.h"
#include "../include/utils.h"

/* Rough Design:
 * Stolen from objdump, linear sweep through each symbol is probably the best balance of speed and complexity
 * Can run a clean-up pass to ensure all code is reached and that nothing major is missed
 */

typedef enum {
    F_NONE = 0,
    F_MODRM = 1 << 0,
    F_IMM8 = 1 << 1,
    F_IMM16 = 1 << 2,
    F_GROUP = 1 << 3
}flags;

typedef struct {
    char* mnemonic;
    uint8_t flags;
}isa_entry_t;

const char mod_table[4][16] = {"INDIRECT", "DISP_1B", "DISP_4B", "REG_MODE"};

const char reg[3][4] = {"eax", "ecx", "edx"};

const isa_entry_t isa_table[256] = {
    [0x01] = {"add", F_NONE},
    [0xc3] = {"ret", F_NONE}
};

void disassemble(unsigned char* buff, header_t* header) {
    int offsets[128];
    int idx = 0;
    for (int i = 0; i < header->symtab->size; i++) {
        if ((header->symtab[i].symtab_entry->section == header->sections[3].sh_entry->shstrtab_idx) ||
                (header->symtab[i].symtab_entry->section == header->sections[4].sh_entry->shstrtab_idx) ||
                    (header->symtab[i].symtab_entry->section == header->sections[5].sh_entry->shstrtab_idx)) {
            //printf("0x%02lx: %s\n", header->symtab[i].symtab_entry->offset, header->symtab[i].symtab_entry->name);
            offsets[idx] = i;
            printf("%d, %d\n", idx, i);
            idx++;
        }
    }
  
    for (int i = 0; i < idx; i++) {
        printf("%ld\n", header->sections[offsets[idx]].sh_entry->offset);
        for (int j = header->sections[offsets[idx]].sh_entry->offset; j < header->sections[offsets[idx]].sh_entry->size; j++) {
           printf("0x%01x", buff[j]); 
        } 
    }

    // Print out the add instruction in main:
    print_bytes(buff, 0x1131, 0x1132);

    //0x1131 0x1132
    unsigned char opcode = buff[0x1131];
    unsigned char modrm = buff[0x1132];
    unsigned char mod = (modrm & 0b11000000) >> 6;
    unsigned char s = (modrm & 0b00111000) >> 3;
    unsigned char d = modrm & 0b00000111;

    printf("0x1131: 0x%01x 0x%01x:\t %s \t %s, %s", opcode, modrm, isa_table[opcode].mnemonic, reg[s], reg[d]);
}
