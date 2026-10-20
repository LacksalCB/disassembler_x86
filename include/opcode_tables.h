#ifndef OPCODE_TABLES_H
#define OPCODE_TABLES_H
#include <string.h>

struct opcode {
    const char* mnemonic; 
    int flags; 
};

#define COUNT 256  
#define OPCODE_LENGTH 0  
#define DEFAULT_BIT_WIDTH 32 

// FLAGS:
#define F_NONE  0
#define F_MODRM  1<<0
#define F_SIB  1<<1
#define F_IMM8  1<<2
#define F_IMM32  1<<3
#define F_GROUP  1<<4
#define F_REX  1<<5
#define F_PREFIX  1<<6
#define F_REG_BITS  1<<7

// OPERANDS:
void init_operand_tables(void);

//OPCODES:
extern struct opcode opcode_table[256];

#endif
