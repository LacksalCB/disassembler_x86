#include "../include/disassembler.h"
#include "../include/utils.h"
#include "../include/opcode_tables.h"

// x86_specific mod rm operand encoding rules
void parse_modrm(unsigned char* buff, size_t* offset, modrm_info* info) {
    char modrm = buff[*offset++];
    // Extract Mod RM REG fields from ModR/M bits
    info->mod = (modrm >> 6) & 0b11;
    info->reg = (modrm >> 3) & 0b111;
    info->rm = modrm & 0x7;
     
    // Check SIB
    info->has_sib = (info->mod & 0b00 && info->rm & 0b100);
    if (info->has_sib) {
        info->sib = buff[*offset++];
    }

    if (info->mod == 0b00 && info->rm == 0b101) {
        info->displacement =  (uint32_t)(buff[*offset++] & 0xffffffff);
        *offset += 4;
    }
    if (info->mod == 0b01) {
        info ->displacement = (uint32_t)(buff[*offset++] & 0xff);
        *offset += 1;
    }
    if (info->mod == 0b10) {
        info ->displacement = (uint32_t)(buff[*offset++] & 0xffffffff);
        *offset += 4;
    }
}


void disassemble(unsigned char* buff, header_t* header) {
    init_operand_tables();
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

    //for (int i = 0; i < idx; i++) {
        uint64_t j = header->symtab[30].symtab_entry->offset;
        printf("\n%016lx: <%s>\n", j, header->symtab[30].symtab_entry->name);   

        while (buff[j] != 0xc3) {
            unsigned char opcode = buff[j];
           
            puts(opcode_table[opcode].mnemonic);
            if (opcode_table[opcode].flags & F_MODRM) {
                puts("MODRM");
                modrm_info* info = malloc(1 * sizeof(struct MODRM_STRUCT));
                parse_modrm(buff, &j, info);
            }
            if (opcode_table[opcode].flags & F_PREFIX) {
                puts("PREFIX");
            }
            printf("    %lx:    %x\t\t%s\n", j, opcode, opcode_table[opcode].mnemonic);
            j++;
           
        }
    //}
    
}
