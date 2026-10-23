#include <string.h>
#include "../include/opcode_tables.h"


// OPERANDS:
char reg_8_short[8][4];
char reg_8_rex[8][4];
char reg_16[8][4];
char reg_32[8][4];
char reg_64[16][4];

void init_operand_tables() {

// REG_8_SHORT:
    strcpy(reg_8_short[0b0], "al");
    strcpy(reg_8_short[0b1], "cl");
    strcpy(reg_8_short[0b10], "bl");
    strcpy(reg_8_short[0b11], "dl");
    strcpy(reg_8_short[0b100], "ah");
    strcpy(reg_8_short[0b101], "ch");
    strcpy(reg_8_short[0b110], "dh");
    strcpy(reg_8_short[0b111], "bh");

// REG_8_REX:
    strcpy(reg_8_rex[0b100], "spl");
    strcpy(reg_8_rex[0b101], "bpl");
    strcpy(reg_8_rex[0b110], "sil");
    strcpy(reg_8_rex[0b111], "dil");

// REG_16:
    strcpy(reg_16[0b0], "ax");
    strcpy(reg_16[0b1], "cx");
    strcpy(reg_16[0b10], "dx");
    strcpy(reg_16[0b11], "bx");
    strcpy(reg_16[0b100], "sp");
    strcpy(reg_16[0b101], "bp");
    strcpy(reg_16[0b110], "si");
    strcpy(reg_16[0b111], "di");

// REG_32:
    strcpy(reg_32[0b0], "eax");
    strcpy(reg_32[0b1], "ecx");
    strcpy(reg_32[0b10], "edx");
    strcpy(reg_32[0b11], "ebx");
    strcpy(reg_32[0b100], "esp");
    strcpy(reg_32[0b101], "ebp");
    strcpy(reg_32[0b110], "esi");
    strcpy(reg_32[0b111], "edi");

// REG_64:
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
}


//OPCODES:
struct opcode opcode_table_bytes1[256];
struct opcode opcode_table_bytes2[256];
struct opcode opcode_table_bytes3[256];
struct opcode grp1[8];
struct opcode grp2[8];
struct opcode grp3[8];
struct opcode grp4[8];
struct opcode grp5[8];
struct opcode grp6[8];
struct opcode grp7[8];
struct opcode grp8[8];
struct opcode grp9[8];
struct opcode grp10[8];
struct opcode grp11[8];
struct opcode grp12[8];
struct opcode grp13[8];
struct opcode grp14[8];
struct opcode grp15[8];
struct opcode grp16[8];
struct opcode grp17[8];
struct opcode grp18[8];
struct opcode grp19[8];
struct opcode grp20[8];
struct opcode grp21[8];

void init_opcode_tables() {

// OPCODE_TABLE_BYTES1:
    opcode_table_bytes1[0x00].mnemonic = "add";
    opcode_table_bytes1[0x00].flags =  F_MODRM|F_BITS_EIGHT;
    memcpy(opcode_table_bytes1[0x00].operands, (int[]) {0}, sizeof(opcode_table_bytes1[0x00].operands));
    opcode_table_bytes1[0x00].sel =  SEL_NONE;
    opcode_table_bytes1[0x00].next = NULL;
    opcode_table_bytes1[0x01].mnemonic = "add";
    opcode_table_bytes1[0x01].flags =  F_MODRM;
    memcpy(opcode_table_bytes1[0x01].operands, (int[]) {0}, sizeof(opcode_table_bytes1[0x01].operands));
    opcode_table_bytes1[0x01].sel =  SEL_NONE;
    opcode_table_bytes1[0x01].next = NULL;
    opcode_table_bytes1[0x0f].mnemonic = NULL;
    opcode_table_bytes1[0x0f].flags =  F_ESCAPE;
    memcpy(opcode_table_bytes1[0x0f].operands, (int[]) {0}, sizeof(opcode_table_bytes1[0x0f].operands));
    opcode_table_bytes1[0x0f].sel =  SEL_NEXT_BYTE;
    opcode_table_bytes1[0x0f].next = opcode_table_bytes2;
    opcode_table_bytes1[0x40].mnemonic = NULL;
    opcode_table_bytes1[0x40].flags =  F_PREFIX|F_REX;
    memcpy(opcode_table_bytes1[0x40].operands, (int[]) {0}, sizeof(opcode_table_bytes1[0x40].operands));
    opcode_table_bytes1[0x40].sel =  SEL_NEXT_BYTE;
    opcode_table_bytes1[0x40].next = opcode_table_bytes1;
    opcode_table_bytes1[0x41].mnemonic = NULL;
    opcode_table_bytes1[0x41].flags =  F_PREFIX|F_REX;
    memcpy(opcode_table_bytes1[0x41].operands, (int[]) {0}, sizeof(opcode_table_bytes1[0x41].operands));
    opcode_table_bytes1[0x41].sel =  SEL_NEXT_BYTE;
    opcode_table_bytes1[0x41].next = opcode_table_bytes1;
    opcode_table_bytes1[0x42].mnemonic = NULL;
    opcode_table_bytes1[0x42].flags =  F_PREFIX|F_REX;
    memcpy(opcode_table_bytes1[0x42].operands, (int[]) {0}, sizeof(opcode_table_bytes1[0x42].operands));
    opcode_table_bytes1[0x42].sel =  SEL_NEXT_BYTE;
    opcode_table_bytes1[0x42].next = opcode_table_bytes1;
    opcode_table_bytes1[0x43].mnemonic = NULL;
    opcode_table_bytes1[0x43].flags =  F_PREFIX|F_REX;
    memcpy(opcode_table_bytes1[0x43].operands, (int[]) {0}, sizeof(opcode_table_bytes1[0x43].operands));
    opcode_table_bytes1[0x43].sel =  SEL_NEXT_BYTE;
    opcode_table_bytes1[0x43].next = opcode_table_bytes1;
    opcode_table_bytes1[0x44].mnemonic = NULL;
    opcode_table_bytes1[0x44].flags =  F_PREFIX|F_REX;
    memcpy(opcode_table_bytes1[0x44].operands, (int[]) {0}, sizeof(opcode_table_bytes1[0x44].operands));
    opcode_table_bytes1[0x44].sel =  SEL_NEXT_BYTE;
    opcode_table_bytes1[0x44].next = opcode_table_bytes1;
    opcode_table_bytes1[0x45].mnemonic = NULL;
    opcode_table_bytes1[0x45].flags =  F_PREFIX|F_REX;
    memcpy(opcode_table_bytes1[0x45].operands, (int[]) {0}, sizeof(opcode_table_bytes1[0x45].operands));
    opcode_table_bytes1[0x45].sel =  SEL_NEXT_BYTE;
    opcode_table_bytes1[0x45].next = opcode_table_bytes1;
    opcode_table_bytes1[0x46].mnemonic = NULL;
    opcode_table_bytes1[0x46].flags =  F_PREFIX|F_REX;
    memcpy(opcode_table_bytes1[0x46].operands, (int[]) {0}, sizeof(opcode_table_bytes1[0x46].operands));
    opcode_table_bytes1[0x46].sel =  SEL_NEXT_BYTE;
    opcode_table_bytes1[0x46].next = opcode_table_bytes1;
    opcode_table_bytes1[0x47].mnemonic = NULL;
    opcode_table_bytes1[0x47].flags =  F_PREFIX|F_REX;
    memcpy(opcode_table_bytes1[0x47].operands, (int[]) {0}, sizeof(opcode_table_bytes1[0x47].operands));
    opcode_table_bytes1[0x47].sel =  SEL_NEXT_BYTE;
    opcode_table_bytes1[0x47].next = opcode_table_bytes1;
    opcode_table_bytes1[0x48].mnemonic = NULL;
    opcode_table_bytes1[0x48].flags =  F_PREFIX|F_REX;
    memcpy(opcode_table_bytes1[0x48].operands, (int[]) {0}, sizeof(opcode_table_bytes1[0x48].operands));
    opcode_table_bytes1[0x48].sel =  SEL_REX_W;
    opcode_table_bytes1[0x48].next = opcode_table_bytes1;
    opcode_table_bytes1[0x49].mnemonic = NULL;
    opcode_table_bytes1[0x49].flags =  F_PREFIX|F_REX;
    memcpy(opcode_table_bytes1[0x49].operands, (int[]) {0}, sizeof(opcode_table_bytes1[0x49].operands));
    opcode_table_bytes1[0x49].sel =  SEL_NEXT_BYTE;
    opcode_table_bytes1[0x49].next = opcode_table_bytes1;
    opcode_table_bytes1[0x4a].mnemonic = NULL;
    opcode_table_bytes1[0x4a].flags =  F_PREFIX|F_REX;
    memcpy(opcode_table_bytes1[0x4a].operands, (int[]) {0}, sizeof(opcode_table_bytes1[0x4a].operands));
    opcode_table_bytes1[0x4a].sel =  SEL_NEXT_BYTE;
    opcode_table_bytes1[0x4a].next = opcode_table_bytes1;
    opcode_table_bytes1[0x4b].mnemonic = NULL;
    opcode_table_bytes1[0x4b].flags =  F_PREFIX|F_REX;
    memcpy(opcode_table_bytes1[0x4b].operands, (int[]) {0}, sizeof(opcode_table_bytes1[0x4b].operands));
    opcode_table_bytes1[0x4b].sel =  SEL_NEXT_BYTE;
    opcode_table_bytes1[0x4b].next = opcode_table_bytes1;
    opcode_table_bytes1[0x4c].mnemonic = NULL;
    opcode_table_bytes1[0x4c].flags =  F_PREFIX|F_REX;
    memcpy(opcode_table_bytes1[0x4c].operands, (int[]) {0}, sizeof(opcode_table_bytes1[0x4c].operands));
    opcode_table_bytes1[0x4c].sel =  SEL_NEXT_BYTE;
    opcode_table_bytes1[0x4c].next = opcode_table_bytes1;
    opcode_table_bytes1[0x4d].mnemonic = NULL;
    opcode_table_bytes1[0x4d].flags =  F_PREFIX|F_REX;
    memcpy(opcode_table_bytes1[0x4d].operands, (int[]) {0}, sizeof(opcode_table_bytes1[0x4d].operands));
    opcode_table_bytes1[0x4d].sel =  SEL_NEXT_BYTE;
    opcode_table_bytes1[0x4d].next = opcode_table_bytes1;
    opcode_table_bytes1[0x4e].mnemonic = NULL;
    opcode_table_bytes1[0x4e].flags =  F_PREFIX|F_REX;
    memcpy(opcode_table_bytes1[0x4e].operands, (int[]) {0}, sizeof(opcode_table_bytes1[0x4e].operands));
    opcode_table_bytes1[0x4e].sel =  SEL_NEXT_BYTE;
    opcode_table_bytes1[0x4e].next = opcode_table_bytes1;
    opcode_table_bytes1[0x4f].mnemonic = NULL;
    opcode_table_bytes1[0x4f].flags =  F_PREFIX|F_REX;
    memcpy(opcode_table_bytes1[0x4f].operands, (int[]) {0}, sizeof(opcode_table_bytes1[0x4f].operands));
    opcode_table_bytes1[0x4f].sel =  SEL_NEXT_BYTE;
    opcode_table_bytes1[0x4f].next = opcode_table_bytes1;
    opcode_table_bytes1[0x55].mnemonic = "push";
    opcode_table_bytes1[0x55].flags =  F_REG_BITS;
    memcpy(opcode_table_bytes1[0x55].operands, (int[]) {0}, sizeof(opcode_table_bytes1[0x55].operands));
    opcode_table_bytes1[0x55].sel =  SEL_NONE;
    opcode_table_bytes1[0x55].next = NULL;
    opcode_table_bytes1[0x5d].mnemonic = "pop";
    opcode_table_bytes1[0x5d].flags =  F_REG_BITS;
    memcpy(opcode_table_bytes1[0x5d].operands, (int[]) {0}, sizeof(opcode_table_bytes1[0x5d].operands));
    opcode_table_bytes1[0x5d].sel =  SEL_NONE;
    opcode_table_bytes1[0x5d].next = NULL;
    opcode_table_bytes1[0x83].mnemonic = NULL;
    opcode_table_bytes1[0x83].flags =  F_MODRM;
    memcpy(opcode_table_bytes1[0x83].operands, (int[]) {0}, sizeof(opcode_table_bytes1[0x83].operands));
    opcode_table_bytes1[0x83].sel =  SEL_NEXT_BYTE;
    opcode_table_bytes1[0x83].next = grp1;
    opcode_table_bytes1[0x89].mnemonic = "mov";
    opcode_table_bytes1[0x89].flags =  F_MODRM;
    memcpy(opcode_table_bytes1[0x89].operands, (int[]) {0}, sizeof(opcode_table_bytes1[0x89].operands));
    opcode_table_bytes1[0x89].sel =  SEL_NONE;
    opcode_table_bytes1[0x89].next = NULL;
    opcode_table_bytes1[0x8b].mnemonic = "mov";
    opcode_table_bytes1[0x8b].flags =  F_MODRM;
    memcpy(opcode_table_bytes1[0x8b].operands, (int[]) {0}, sizeof(opcode_table_bytes1[0x8b].operands));
    opcode_table_bytes1[0x8b].sel =  SEL_NONE;
    opcode_table_bytes1[0x8b].next = NULL;
    opcode_table_bytes1[0xb8].mnemonic = "mov";
    opcode_table_bytes1[0xb8].flags =  F_IMM32;
    memcpy(opcode_table_bytes1[0xb8].operands, (int[]) {0}, sizeof(opcode_table_bytes1[0xb8].operands));
    opcode_table_bytes1[0xb8].sel =  SEL_NONE;
    opcode_table_bytes1[0xb8].next = NULL;
    opcode_table_bytes1[0xc3].mnemonic = "ret";
    opcode_table_bytes1[0xc3].flags =  F_NONE;
    memcpy(opcode_table_bytes1[0xc3].operands, (int[]) {0}, sizeof(opcode_table_bytes1[0xc3].operands));
    opcode_table_bytes1[0xc3].sel =  SEL_NONE;
    opcode_table_bytes1[0xc3].next = NULL;
    opcode_table_bytes1[0xc7].mnemonic = "movl";
    opcode_table_bytes1[0xc7].flags =  F_MODRM;
    memcpy(opcode_table_bytes1[0xc7].operands, (int[]) {0}, sizeof(opcode_table_bytes1[0xc7].operands));
    opcode_table_bytes1[0xc7].sel =  SEL_NONE;
    opcode_table_bytes1[0xc7].next = NULL;

// OPCODE_TABLE_BYTES2:
    opcode_table_bytes2[0x1e].mnemonic = NULL;
    opcode_table_bytes2[0x1e].flags =  F_NONE;
    memcpy(opcode_table_bytes2[0x1e].operands, (int[]) {0}, sizeof(opcode_table_bytes2[0x1e].operands));
    opcode_table_bytes2[0x1e].sel =  SEL_PREFIX;
    opcode_table_bytes2[0x1e].next = grp21;

// OPCODE_TABLE_BYTES3:

// GRP1:
    grp1[0x00].mnemonic = "add";
    grp1[0x00].flags = F_MODRM;
    memcpy(grp1[0x00].operands, (int[]) {0}, sizeof(grp1[0x00].operands));
    grp1[0x00].sel =  SEL_NONE;
    grp1[0x00].next = NULL;
    grp1[0x01].mnemonic = "or";
    grp1[0x01].flags = F_MODRM;
    memcpy(grp1[0x01].operands, (int[]) {0}, sizeof(grp1[0x01].operands));
    grp1[0x01].sel =  SEL_NONE;
    grp1[0x01].next = NULL;
    grp1[0x02].mnemonic = "adc";
    grp1[0x02].flags = F_MODRM;
    memcpy(grp1[0x02].operands, (int[]) {0}, sizeof(grp1[0x02].operands));
    grp1[0x02].sel =  SEL_NONE;
    grp1[0x02].next = NULL;
    grp1[0x03].mnemonic = "sbb";
    grp1[0x03].flags = F_MODRM;
    memcpy(grp1[0x03].operands, (int[]) {0}, sizeof(grp1[0x03].operands));
    grp1[0x03].sel =  SEL_NONE;
    grp1[0x03].next = NULL;
    grp1[0x04].mnemonic = "and";
    grp1[0x04].flags = F_MODRM;
    memcpy(grp1[0x04].operands, (int[]) {0}, sizeof(grp1[0x04].operands));
    grp1[0x04].sel =  SEL_NONE;
    grp1[0x04].next = NULL;
    grp1[0x05].mnemonic = "sub";
    grp1[0x05].flags = F_MODRM;
    memcpy(grp1[0x05].operands, (int[]) {0}, sizeof(grp1[0x05].operands));
    grp1[0x05].sel =  SEL_NONE;
    grp1[0x05].next = NULL;
    grp1[0x06].mnemonic = "xor";
    grp1[0x06].flags = F_MODRM;
    memcpy(grp1[0x06].operands, (int[]) {0}, sizeof(grp1[0x06].operands));
    grp1[0x06].sel =  SEL_NONE;
    grp1[0x06].next = NULL;
    grp1[0x07].mnemonic = "cmp";
    grp1[0x07].flags = F_MODRM;
    memcpy(grp1[0x07].operands, (int[]) {0}, sizeof(grp1[0x07].operands));
    grp1[0x07].sel =  SEL_NONE;
    grp1[0x07].next = NULL;

// GRP21:
    grp21[0x07].mnemonic = "endbr64";
    grp21[0x07].flags =  F_NONE;
    memcpy(grp21[0x07].operands, (int[]) {0}, sizeof(grp21[0x07].operands));
    grp21[0x07].sel =  SEL_NONE;
    grp21[0x07].next = NULL;
}
