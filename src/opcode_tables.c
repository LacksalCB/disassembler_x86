#include <string.h>
#include "../include/opcode_tables.h"
    struct opcode opcode_table[256];
// OPERANDS:
void init_operand_tables(void) {

// REG_8_SHORT:
    char reg_8_short[8][4];
    strcpy(reg_8_short[0b0], "al");
    strcpy(reg_8_short[0b1], "cl");
    strcpy(reg_8_short[0b10], "bl");
    strcpy(reg_8_short[0b11], "dl");
    strcpy(reg_8_short[0b100], "ah");
    strcpy(reg_8_short[0b101], "ch");
    strcpy(reg_8_short[0b110], "dh");
    strcpy(reg_8_short[0b111], "bh");

// REG_8_REX:
    char reg_8_rex[8][4];
    strcpy(reg_8_rex[0b100], "spl");
    strcpy(reg_8_rex[0b101], "bpl");
    strcpy(reg_8_rex[0b110], "sil");
    strcpy(reg_8_rex[0b111], "dil");

// REG_16:
    char reg_16[8][4];
    strcpy(reg_16[0b0], "ax");
    strcpy(reg_16[0b1], "cx");
    strcpy(reg_16[0b10], "dx");
    strcpy(reg_16[0b11], "bx");
    strcpy(reg_16[0b100], "sp");
    strcpy(reg_16[0b101], "bp");
    strcpy(reg_16[0b110], "si");
    strcpy(reg_16[0b111], "di");

// REG_32:
    char reg_32[8][4];
    strcpy(reg_32[0b0], "eax");
    strcpy(reg_32[0b1], "ecx");
    strcpy(reg_32[0b10], "edx");
    strcpy(reg_32[0b11], "ebx");
    strcpy(reg_32[0b100], "esp");
    strcpy(reg_32[0b101], "ebp");
    strcpy(reg_32[0b110], "esi");
    strcpy(reg_32[0b111], "edi");

// REG_64:
    char reg_64[16][4];
    strcpy(reg_64[0b0], "rax");
    strcpy(reg_64[0b1], "rcx");
    strcpy(reg_64[0b10], "rdx");
    strcpy(reg_64[0b11], "rbx");
    strcpy(reg_64[0b100], "rsp");
    strcpy(reg_64[0b101], "rbp");
    strcpy(reg_64[0b110], "rsi");
    strcpy(reg_64[0b111], "rdi");
    strcpy(reg_64[0b1000], "r8");
    strcpy(reg_64[0b1001], "r9");
    strcpy(reg_64[0b1010], "r10");
    strcpy(reg_64[0b1011], "r11");
    strcpy(reg_64[0b1100], "r12");
    strcpy(reg_64[0b1101], "r13");
    strcpy(reg_64[0b1110], "r14");
    strcpy(reg_64[0b1111], "r15");

// OPCODE_TABLE:
        opcode_table[0x01].mnemonic =  "add";
opcode_table[0x01].flags = F_MODRM;
        opcode_table[0x48].mnemonic =  NULL;
opcode_table[0x48].flags = F_PREFIX|F_REX;
        opcode_table[0x55].mnemonic =  "push %rbp";
opcode_table[0x55].flags = F_NONE;
        opcode_table[0x5d].mnemonic =  "pop %rbp";
opcode_table[0x5d].flags = F_NONE;
        opcode_table[0x83].mnemonic =  "addl";
opcode_table[0x83].flags = F_MODRM;
        opcode_table[0x89].mnemonic =  "mov";
opcode_table[0x89].flags = F_MODRM;
        opcode_table[0x8b].mnemonic =  "mov";
opcode_table[0x8b].flags = F_MODRM;
        opcode_table[0xb8].mnemonic =  "mov";
opcode_table[0xb8].flags = F_IMM32|F_REG_BITS ;
        opcode_table[0xc3].mnemonic =  "ret";
opcode_table[0xc3].flags = F_NONE;
        opcode_table[0xc7].mnemonic =  "movl";
opcode_table[0xc7].flags = F_MODRM;
}
