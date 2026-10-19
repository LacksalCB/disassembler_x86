#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include "header_reader.h"

typedef struct MODRM_STRUCT {
    uint8_t mod;
    uint8_t rm;
    uint8_t reg;
    int has_sib;
    uint8_t sib;
    uint32_t displacement;
}modrm_info;

void parse_modrm(unsigned char* buff, size_t* offset, modrm_info* info);

void disassemble(unsigned char* buff, header_t* header);

#endif /* DISASSEMBLER_H */
