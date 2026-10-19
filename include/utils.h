#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

void print_bin(unsigned char* buff, size_t size);
void print_bytes(unsigned char* buff, size_t src, size_t dest);
unsigned char* read_file(char* filename, const char* type, size_t* len);

#endif /* UTILS_H  */
