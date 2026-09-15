#ifndef ISA_READER_H
#define ISA_READER_H

#include <stdint.h>

// Formatted .isad text file (ISA Description)
/*
 * All I got for now, is that we need a list of every opcode, and some way to parse how they are arranged
 * Groups (endbr64 e.g.) are gonna be interesting
 * Idioms and random bs idk man
 * How does one make a system that handles ModR/M and other stuff? Idk man
 * 
 */ 

/* Specific ISA/OpCode decoding
 * isa description files held in lib/[architecture]_instrdesc.isa with following format:
 * [Opcode]: [Mnemonic] [flags (ModR/M): Addressing Method, Operand Size]
 * Addressing Method:
 *  E: r/m
 *  G: reg
 *  I: Immediate
 * Operand Size:
 *  b: 8 bits
 *  v: dynamic word size (16, 32, 64)
 *  z: dynamic word size (16, 32, 64) based on bit length mode
 */


// x86_64 (AMD64) ISA:
// Opcodes to Name (mnemonic) and flags required
// Should be dynamically populated for modularity tbh

#endif /* ISA_READER_H */
