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
    F_IMM32 = 1 << 2,
    F_GROUP = 1 << 3
}flags;

typedef struct {
    char* mnemonic;
    uint8_t flags;
}isa_entry_t;

const char mod_table[4][16] = {"INDIRECT", "DISP_1B", "DISP_4B", "REG_MODE"};

const char reg[8][6] = {"%eax", "%ecx", "%edx", "%ebx", "%esp", "%ebp", "%esi", "%edi"};

const isa_entry_t isa_table[256] = {
    [0x01] = {"add", F_MODRM},
    [0x55] = {"push\t%rbp", F_NONE},
    [0x48] = {"", F_GROUP},
    [0x89] = {"mov", F_MODRM}, 
    [0x8d] = {"lea", F_MODRM},
    [0xc3] = {"ret", F_NONE},
    [0xc7] = {"movl", F_IMM32|F_MODRM}
};

void disassemble(unsigned char* buff, header_t* header) {
    int offsets[256];
    int idx = 0;
    for (int i = 0; i < header->symtab->size; i++) {
        if ((header->symtab[i].symtab_entry->section == header->sections[3].sh_entry->shstrtab_idx) ||
                (header->symtab[i].symtab_entry->section == header->sections[4].sh_entry->shstrtab_idx) ||
                    (header->symtab[i].symtab_entry->section == header->sections[5].sh_entry->shstrtab_idx)) {
            offsets[idx] = i;
            idx++;
        }
    }

    for (int i = 0; i < idx; i++) {
        uint64_t j = header->symtab[offsets[i]].symtab_entry->offset;
        printf("\n%016lx: <%s>\n", j, header->symtab[offsets[i]].symtab_entry->name);
        while (buff[j] != 0x00) {
            unsigned char opcode = buff[j];
            
            // Ret kinda has to be handled separately just to end linear search through symbol
            if (opcode == 0xc3) { // ret                 
                printf("%01x\t, %s", opcode, isa_table[opcode].mnemonic);
                break;
            }
            
            // Single byte instructions
            if (isa_table[opcode].flags == F_NONE) { 
                printf("    %lx:    %x\t\t%s\n", j, opcode, isa_table[opcode].mnemonic);
                j++;
                continue;
            }

            unsigned char instr[8] = {0, 0, 0, 0, 0, 0, 0};

            // For Prefixes
            if (isa_table[opcode].flags == F_GROUP) {
                instr[0] = buff[j];
                if (isa_table[buff[j+1]].flags == F_MODRM) {
                    instr[1] = buff[j+1];
                    instr[2] = buff[j+2];
                    unsigned char mod = (instr[2] & 0b11000000) >> 6;
                    switch(mod) {
                        case 0b00:
                            break;
                        case 0b01:
                            break;
                        case 0b10:
                            break;
                        case 0b11:    
                            unsigned char s = (instr[2] & 0b00111000) >> 3;
                            unsigned char d = instr[2] & 0b00000111;
                            printf("    %lx:    ", j);
                            for (int k = 0; k < 3; k++) {
                                printf("%x ", instr[k]);
                            } 
                            printf("\t%s\t%s,%s\n", isa_table[instr[1]].mnemonic, reg[s], reg[d]);
                            j += 2;
                            break;
                    }
                    j++;
                    continue;
                }
            } 

            // c7 45 f4 ef be 00 00
            // c7 -> mov with 32 bit imm
            // 45 -> 0b01 000101
            // f4 -> 0b11 110 100
            // Multi byte instructions
            if (isa_table[opcode].flags == (F_IMM32|F_MODRM)) {
                instr[0] = buff[j];
                instr[1] = buff[j+1];
                unsigned char mod = (instr[1] & 0b11000000) >> 6;
                switch(mod) {
                    case 0b00:
                        break;
                    case 0b01:
                        unsigned char rm = instr[1] & 0b00000111;
                        char disp = buff[j+2];
                        instr[2] = disp;
                        unsigned char val_bytes[4];
                        memcpy(val_bytes, buff+j+3, sizeof(val_bytes));
                        for (int k = 3; k < 7; k++) {
                            instr[k] = buff[j+k];
                        }

                        uint64_t imm = uctoull(val_bytes, sizeof(val_bytes));
                        printf("    %lx:    ", j);
                        for (int k = 0; k < 7; k++) {
                            printf("%02x ", instr[k]);
                        }
                        printf("\t%s\t$0x%lx,%s0x%02x(%s)\n", isa_table[instr[0]].mnemonic, imm, (disp < 0) ? "-" : "", abs(disp), reg[rm]);
                        j += 6;
                        break;
                    case 0b10:
                        break;
                    case 0b11:    
                        unsigned char s = (instr[2] & 0b00111000) >> 3;
                        unsigned char d = instr[2] & 0b00000111;
                        printf("    %lx:    ", j);
                        for (int k = 0; k < 3; k++) {
                            printf("%x ", instr[k]);
                        } 
                        printf("\t%s\t%s,%s\n", isa_table[instr[1]].mnemonic, reg[s], reg[d]);
                        j += 2;
                        break;
                }
                j++;
                continue;              
            }
            j++;
        }    
    }
}
