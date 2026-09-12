#include "../include/disassembler.h"

/* Rough Design:
 * Stolen from objdump, linear sweep through each symbol is probably the best balance of speed and complexity
 * Can run a clean-up pass to ensure all code is reached and that nothing major is missed
 */

void disassemble(unsigned char* buff, header_t* header) {
    for (int i = 0; i < header->symtab->size; i++) {
        if ((header->symtab[i].symtab_entry->section == header->sections[3].sh_entry->shstrtab_idx) ||
                (header->symtab[i].symtab_entry->section == header->sections[4].sh_entry->shstrtab_idx) ||
                    (header->symtab[i].symtab_entry->section == header->sections[5].sh_entry->shstrtab_idx)) {
            printf("0x%02lx: %s\n", header->symtab[i].symtab_entry->offset, header->symtab[i].symtab_entry->name);
        }
    }
}
