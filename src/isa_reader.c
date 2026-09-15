#include "../include/isa_reader.h"

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

// Entry layout:
// 

void read(char* file) {
    
}
