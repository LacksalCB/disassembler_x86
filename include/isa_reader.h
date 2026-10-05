#ifndef ISA_READER_H
#define ISA_READER_H

#include <stdint.h>

typedef struct instruction {
    char prefix;
    char opcode;
    char multi_bit_opcode[8];       // Won't ever be over 8 bytes
    char flags[32];                 // For various opcode flags 
}instr_t;

#endif /* ISA_READER_H */
