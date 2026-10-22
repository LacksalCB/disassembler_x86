#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include "header_reader.h"
#include "opcode_tables.h"

void parse_modrm(unsigned char* buff, size_t* offset, Modrm* byte);
void parse_rex(unsigned char* buff, size_t* offset, REX* byte);

void disassemble(unsigned char* buff, header_t* header);

#endif /* DISASSEMBLER_H */
