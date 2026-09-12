#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "../include/header_reader.h"
#include "../include/disassembler.h"


void print_bin(unsigned char* buff, size_t size) {
    printf("%08lx: ", (uint64_t)0x00); 
    for (uint64_t i = 0; i < size; i++) {  
        printf("0x%02x ", (unsigned char) buff[i]);
        if ((i+1) % 16 == 0 || i == size) {
            printf("\n%08lx: ", i+1);
        }   
    }
}

void print_bytes(unsigned char* buff, size_t src, size_t dest) {
    printf("%08lx: ", (uint64_t)src); 
    for (uint64_t i = src; i < dest; i++) { 
        printf("0x%02x ", (unsigned char) buff[i]);
        if ((i+1) % 16 == 0 || i == dest) {
            printf("\n%08lx: ", i+1);
        }   
    }
    puts("");
}

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
    FILE* bin;
    size_t size = 0;
    bin = fopen(argv[1], "rb");
    fseek(bin, 0, SEEK_END);
    size = ftell(bin);
    fseek(bin, 0, SEEK_SET);
    unsigned char* buff = malloc(size);
    fread(buff, 1, size, bin);
    fclose(bin);

    header_t* headers = read_headers(buff);
    disassemble(buff, headers);

    puts("");

    dealloc(headers, buff);
    return 0;
}
