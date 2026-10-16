#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "../include/header_reader.h"
#include "../include/isa_reader.h"
#include "../include/disassembler.h"
#include "../include/utils.h"

void dealloc(header_t* headers, unsigned char* buff) {
   free(headers->e_header);
   for (int i = 0; i < headers->sections->size; i++) {
        free(headers->sections[i].sh_entry);
   }
   free(headers->sections);
   for (int i = 0; i < headers->symtab->size; i++) {
        free(headers->symtab[i].symtab_entry);
   }
   free(headers->symtab);
   free(headers);
   free(buff);
}

void dump(unsigned char* buff, sections_t* code_sections) {
    puts("");
    for (int i = 0; i < code_sections->size; i++) {
        if (code_sections[i].type == CODE) {
            printf("Dissassembly of section: .%s\n\n", code_sections[i].sh_entry->name);
            printf("%016lx <fn: %s>:\n", code_sections[i].sh_entry->offset, code_sections[i].sh_entry->name); // This is wrong, this should be function names
            print_bytes(buff, code_sections[i].sh_entry->offset, code_sections[i].sh_entry->offset+code_sections[i].sh_entry->size);
            puts("\n");
        } 
    }
}

int main(int argc, char** argv) {
    int len = 0;
    unsigned char* buff = read_file(argv[1], "rb", &len);
    
    encode_isa("lib/x86_64.isad");

    header_t* headers = read_headers(buff);
    disassemble(buff, headers);

    puts("");

    dealloc(headers, buff);
    return 0;
}
