#ifndef OPCODE_TABLES_H
#define OPCODE_TABLES_H
#include <string.h>
#include <stdint.h>
#define OPCODE_LENGTH 0  
#define DEFAULT_BIT_WIDTH 32 

// FLAGS:
#define F_NONE  0
#define F_BITS_EIGHT  1<<0
#define F_MODRM  1<<1
#define F_SIB  1<<2
#define F_IMM8  1<<3
#define F_IMM32  1<<4
#define F_GROUP  1<<5
#define F_REX  1<<6
#define F_PREFIX  1<<7
#define F_REG_BITS  1<<8
#define F_ESCAPE  1<<9

// STRUCTURES:
enum selector {
    SEL_NONE,
    SEL_NEXT_BYTE,
    SEL_MODRM_REG,
    SEL_PREFIX,
    SEL_REX_W
};

struct opcode {
    const char* mnemonic;
    uint32_t flags;
    uint8_t operands[4];
    enum selector sel;
    struct opcode* next;
};

typedef struct MODRM_STRUCT {
    uint8_t mod;
    uint8_t rm;
    uint8_t reg;
    int has_sib;
    uint8_t sib;
    uint32_t disp;
}Modrm;

typedef struct REX_STRUCT {
    uint8_t W;
    uint8_t R;
    uint8_t X;
    uint8_t B;
}REX;

struct Instruction {
    uint8_t prefixes[4];
    REX rex;
    struct opcode* op;
    Modrm modrm;
    uint8_t has_modrm;
    int64_t disp;
    int64_t imm;
    size_t length;
};

// OPERANDS:
void init_operand_tables();
extern char reg_8_short[8][4];
extern char reg_8_rex[8][4];
extern char reg_16[8][4];
extern char reg_32[8][4];
extern char reg_64[16][4];

//OPCODES:
void init_opcode_tables();
extern struct opcode opcode_table_bytes1[256];
extern struct opcode opcode_table_bytes2[256];
extern struct opcode opcode_table_bytes3[256];
extern struct opcode grp1[8];
extern struct opcode grp2[8];
extern struct opcode grp3[8];
extern struct opcode grp4[8];
extern struct opcode grp5[8];
extern struct opcode grp6[8];
extern struct opcode grp7[8];
extern struct opcode grp8[8];
extern struct opcode grp9[8];
extern struct opcode grp10[8];
extern struct opcode grp11[8];
extern struct opcode grp12[8];
extern struct opcode grp13[8];
extern struct opcode grp14[8];
extern struct opcode grp15[8];
extern struct opcode grp16[8];
extern struct opcode grp17[8];
extern struct opcode grp18[8];
extern struct opcode grp19[8];
extern struct opcode grp20[8];
extern struct opcode grp21[8];

#endif
