#include "simulator.h"
#include <stdio.h>
#include <stdlib.h>


unsigned int memory_load_byte(unsigned int address) {
    if ((address >> 2) >= MEMORY_SIZE) {
        printf("Ilegal memory access at address 0x%.8X\n", address);
        exit(1);
    }

    if (address & 0x3) {
        printf("Unaligned memory access at address 0x%.8X\n", address);
        exit(1);
    }

    // Carrega o byte a partir da memória
    int temp = memory[address >> 2] & 0xFF;

    if(temp & 0x80) {
        temp |= 0xFFFFFF00;
    }

    return temp; // Ajusta o byte correto
}

unsigned int memory_load_half(unsigned int address) {
    if ((address >> 2) >= MEMORY_SIZE) {
        printf("Ilegal memory access at address 0x%.8X\n", address);
        exit(1);
    }

    if (address & 0x3) {
        printf("Unaligned memory access at address 0x%.8X\n", address);
        exit(1);
    }

    // Carrega a halfword a partir da memória
    int temp = memory[address >> 2] & 0xFFFF;
    
    if(temp & 0x8000) {
        temp |= 0xFFFF0000;
    }

    return temp; // Ajusta a halfword correta
}

unsigned int memory_load_unsigned_byte(unsigned int address) {
    if ((address >> 2) >= MEMORY_SIZE) {
        printf("Ilegal memory access at address 0x%.8X\n", address);
        exit(1);
    }

    if (address & 0x3) {
        printf("Unaligned memory access at address 0x%.8X\n", address);
        exit(1);
    }

    // Carrega o byte sem sinal
    return memory[address >> 2] & 0xFF;
}

unsigned int memory_load_unsigned_half(unsigned int address) {
    if ((address >> 2) >= MEMORY_SIZE) {
        printf("Ilegal memory access at address 0x%.8X\n", address);
        exit(1);
    }

    if (address & 0x3) {
        printf("Unaligned memory access at address 0x%.8X\n", address);
        exit(1);
    }

    // Carrega a halfword sem sinal
    return memory[address >> 2] & 0xFFFF;
}

unsigned int memory_load_word(unsigned int address) {
    if ((address >> 2) >= MEMORY_SIZE) {
        printf("Ilegal memory access at address 0x%.8X\n", address);
        exit(1);
    }

    if (address & 0x3) {
        printf("Unaligned memory access at address 0x%.8X\n", address);
        exit(1);
    }

    // Carrega a word a partir da memória
    return memory[address >> 2]; // Carrega diretamente a word
}


void memory_store_byte(unsigned int address, unsigned int data){
    if((address >> 2) >= MEMORY_SIZE) {
        printf("Ilegal memory access at address 0x%.8X\n", address);
        exit(1);
    }

    if(address & 0x3) {
        printf("Unaligned memory access at address 0x%.8X\n", address);
        exit(1);
    }

    int temp = memory[address >> 2];

    temp = (temp & 0xFFFFFF00) | (data & 0xFF);

    memory[address >> 2] = temp;

}
 
void memory_store_halfword(unsigned int address, unsigned int data){
    if((address >> 2) >= MEMORY_SIZE) {
        printf("Ilegal memory access at address 0x%.8X\n", address);
        exit(1);
    }

    if(address & 0x3) {
        printf("Unaligned memory access at address 0x%.8X\n", address);
        exit(1);
    }

    int temp = memory[address >> 2];

    temp = (temp & 0xFFFF0000) | (data & 0xFFFF);

    memory[address >> 2] = temp;
}

void memory_store_word(unsigned int address, unsigned int data){
    if((address >> 2) >= MEMORY_SIZE) {
        printf("Ilegal memory access at address 0x%.8X\n", address);
        exit(1);
    }

    if(address & 0x3) {
        printf("Unaligned memory access at address 0x%.8X\n", address);
        exit(1);
    }

    memory[address >> 2] = data;
}