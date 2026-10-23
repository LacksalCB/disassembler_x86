#include "../include/disassembler.h"
#include "../include/utils.h"
#include "../include/opcode_tables.h"

// x86_specific mod rm operand encoding rules
void parse_modrm(unsigned char* buff, size_t* offset, Modrm* byte) {
    char modrm = buff[*offset++];
    // Extract Mod RM REG fields from ModR/M bits
    byte->mod = (modrm >> 6) & 0b11;
    byte->reg = (modrm >> 3) & 0b111;
    byte->rm = modrm & 0x7;
     
    // Check SIB
    byte->has_sib = (byte->mod & 0b00 && byte->rm & 0b100);
    if (byte->has_sib) {
        byte->sib = buff[*offset++];
    }

    if (byte->mod == 0b00 && byte->rm == 0b101) {
        byte->disp =  (uint32_t)(buff[*offset++] & 0xffffffff);
        *offset += 4;
    }
    if (byte->mod == 0b01) {
        byte->disp = (uint32_t)(buff[*offset++] & 0xff);
        *offset += 1;
    }
    if (byte->mod == 0b10) {
        byte ->disp = (uint32_t)(buff[*offset++] & 0xffffffff);
        *offset += 4;
    }
}

void parse_rex(unsigned char* buff, size_t* offset, REX* byte) {
    char rex = buff[*offset++];

    byte->W = (rex >> 3) & 0b1;
    byte->R = (rex >> 2) & 0b1;
    byte->X = (rex >> 1) & 0b1;
    byte->B = rex & 0b1;

    if (byte->W == 1) {
         
    }
}

void disassemble(unsigned char* buff, header_t* header) {
    init_operand_tables();
    //int offsets[256];
    //int idx = 0;
    for (int i = 0; i < header->symtab->size; i++) {
        if ((header->symtab[i].symtab_entry->section == header->sections[3].sh_entry->shstrtab_idx) ||
                (header->symtab[i].symtab_entry->section == header->sections[4].sh_entry->shstrtab_idx) ||
                    (header->symtab[i].symtab_entry->section == header->sections[5].sh_entry->shstrtab_idx)) {
            //offsets[idx] = i;
            //idx++;
        }
    }

    //for (int i = 0; i < idx; i++) {
        uint64_t offset = header->symtab[30].symtab_entry->offset;
        printf("\n%016lx: <%s>\n", offset, header->symtab[30].symtab_entry->name);   

        char byte = buff[offset];
        struct opcode* op = &opcode_table_bytes1[byte];
       
        if (op->flags & F_NONE) {
            
        }
        if (op->flags & F_REG_BITS) {
            op->operands[0] = reg_64[buff[offset]&0b111];
            printf("    %lx:    %x\t\t%s %s\n", offset, byte, opcode_table_bytes1[byte].mnemonic, reg_64[0]);
        }

        while (op->sel != SEL_NONE) {
            switch (op->sel) {
                case SEL_NEXT_BYTE:
                    op = &op->next[buff[offset++]];
                    break;
                case SEL_MODRM_REG:
                    op = &op->next[(buff[offset] >> 4) & 0b111];
                    break;
                case SEL_PREFIX:
                    break;
                case SEL_REX_W:
                    break;
                case SEL_NONE:
                    break;

            }
        }
}
