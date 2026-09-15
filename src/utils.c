#include "../include/utils.h"

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
    for (uint64_t i = src; i <= dest; i++) { 
        printf("0x%02x ", (unsigned char) buff[i]);
        if ((i+1) % 16 == 0 || i == dest) {
            printf("\n%08lx: ", i+1);
        }   
    }
    puts("");
}


