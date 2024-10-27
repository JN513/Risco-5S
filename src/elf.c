#include "elf.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void print_hex_data(unsigned char *data, size_t size) {
    for (size_t i = 0; i < size; i += 4) {
        printf("%02x%02x%02x%02x\n", data[i], data[i+1], data[i+2], data[i+3]);
    }
}