#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include "header_reader.h"

// Flags each instruction can have:
// NONE: no flags 
// MOD_RM: has ModR/M bits
// IMM8: has 8 bit immediate value
// IMM16: Has 16 bit immediate value
typedef enum{
    NONE = 0,
    MOD_RM = 1 << 0,
    IMM8 = 1 << 1,
    IMM16 = 1 << 2,
}flags;

typedef struct {
    const char* mnemonic;
    uint8_t flags;
}instr;

// x86_64 (AMD64) ISA:
// Opcodes to Name (mnemonic) and flags required
// Should be dynamically populated for modularity tbh
const instr opcode_table[256] = {};

void disassemble(unsigned char* buff, header_t* header);

#endif /* DISASSEMBLER_H */
