#include "../include/isa_reader.h"
#include "../include/utils.h"
#include <string.h>


// Declare file.isad COUNT many instr structs, fill out table 
void encode_isa(char* file) {
    int len = 0;
    unsigned char* buff = read_file(file, "r", &len);
    buff[len-1]=0;
    //    printf("%s\n", buff);

    

    free(buff);
}
