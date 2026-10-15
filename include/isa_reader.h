#ifndef ISA_READER_H
#define ISA_READER_H

#include <stdint.h>

typedef struct instruction {
    char prefix;
    char opcode;
    char multi_bit_opcode[8];       // Won't ever be over 8 bytes
    char flags[32];                 // For various opcode flags 
}instr_t;

struct ISA {
    int count;
    char opcode_length;
    instr_t* instructions;
};

void encode_isa(char* file);

#endif /* ISA_READER_H */
