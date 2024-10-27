#include "simulator.h"
#include "log.h"
#include <stdio.h>
#include <stdlib.h>


unsigned int memory_load_byte(unsigned int address) {
    if ((address >> 2) >= MEMORY_SIZE) {
        print_error("Ilegal memory access at address 0x%.8X\n", address);
        exit(1);
    }

    // Carrega o byte a partir da memória
    unsigned temp = memory[address >> 2] & (0xFF << ((address & 0x3) << 3));

    if(temp & 0x80) {
        temp |= 0xFFFFFF00;
    }

    return temp; // Ajusta o byte correto
}

unsigned int memory_load_half(unsigned int address) {
    if ((address >> 2) >= MEMORY_SIZE) {
        print_error("Ilegal memory access at address 0x%.8X\n", address);
        exit(1);
    }

    if (address & 0x3) {
        print_error("Unaligned memory access at address 0x%.8X\n", address);
        exit(1);
    }

    // Carrega a halfword a partir da memória

    int endbits = address & 0x3;
    unsigned temp;

    if(endbits == 0) {
        temp = memory[address >> 2] & 0xFFFF;
    } else {
        temp = (memory[address >> 2] >> (endbits << 3)) & 0xFFFF;
        temp |= (memory[(address >> 2) + 1] << ((4 - endbits) << 3)) & 0xFF00;
    }
    
    if(temp & 0x8000) {
        temp |= 0xFFFF0000;
    }

    return temp; // Ajusta a halfword correta
}

unsigned int memory_load_unsigned_byte(unsigned int address) {
    if ((address >> 2) >= MEMORY_SIZE) {
        print_error("Ilegal memory access at address 0x%.8X\n", address);
        exit(1);
    }

    // Carrega o byte sem sinal
    return memory[address >> 2] & (0xFF << ((address & 0x3) << 3));
}

unsigned int memory_load_unsigned_half(unsigned int address) {
    if ((address >> 2) >= MEMORY_SIZE) {
        print_error("Ilegal memory access at address 0x%.8X\n", address);
        exit(1);
    }

    int endbits = address & 0x3;
    unsigned temp;

    if(endbits == 0) {
        temp = memory[address >> 2] & 0xFFFF;
    } else {
        temp = (memory[address >> 2] >> (endbits << 3)) & 0xFFFF;
        temp |= (memory[(address >> 2) + 1] << ((4 - endbits) << 3)) & 0xFF00;
    }

    // Carrega a halfword sem sinal
    return temp;
}

unsigned int memory_load_word(unsigned int address) {
    if ((address >> 2) >= MEMORY_SIZE) {
        print_error("Ilegal memory access at address 0x%.8X\n", address);
        exit(1);
    }

    // Carrega a word a partir da memória
    if(address & 0x3) {
        return memory[address >> 2]; // Carrega diretamente a word
    } else {
        unsigned temp = memory[address >> 2] >> ((address & 0x3) << 3);
        temp |= (memory[(address >> 2) + 1]) << ((4 - (address & 0x3)) << 3);
        return temp;
    }
}


void memory_store_byte(unsigned int address, unsigned int data){
    if((address >> 2) >= MEMORY_SIZE) {
        printf("Ilegal memory access at address 0x%.8X\n", address);
        exit(1);
    }

    int endbits = address & 0x3;
    unsigned temp = memory[address >> 2];

    if(endbits == 0) {
        temp = (temp & 0xFFFFFF00) | (data & 0xFF);
    } else {
        temp = (temp & ~(0xFF << (endbits << 3))) | ((data & 0xFF) << (endbits << 3));
    }

    memory[address >> 2] = temp;

}
 
void memory_store_halfword(unsigned int address, unsigned int data){
    if((address >> 2) >= MEMORY_SIZE) {
        printf("Ilegal memory access at address 0x%.8X\n", address);
        exit(1);
    }

    int endbits = address & 0x3;
    unsigned temp = memory[address >> 2];

    if(endbits == 0) {
        temp = (temp & 0xFFFF0000) | (data & 0xFFFF);
        memory[address >> 2] = temp;
    } else if (endbits == 1) {
        temp = (temp & 0xFF0000FF) | ((data & 0xFFFF) << 8);
        memory[address >> 2] = temp;
    } else if (endbits == 2) {
        temp = (temp & 0x0000FFFF) | ((data & 0xFFFF) << 16);
        memory[address >> 2] = temp;
    } else {
        temp = (temp & 0x00FFFFFF) | ((data & 0xFFFF) << 24);
        memory[address >> 2] = temp;
        temp = memory[(address >> 2) + 1];
        temp = (temp & 0xFFFFFF00) | ((data & 0xFFFF) >> 8);
        memory[(address >> 2) + 1] = temp;
    }
}

void memory_store_word(unsigned int address, unsigned int data){
    if((address >> 2) >= MEMORY_SIZE) {
        printf("Ilegal memory access at address 0x%.8X\n", address);
        exit(1);
    }

    int endbits = address & 0x3;

    if(endbits == 0) {
        memory[address >> 2] = data;
    } else {
        memory[address >> 2] = (memory[address >> 2] & ((1 << (endbits << 3)) - 1)) | (data << (endbits << 3));
        memory[(address >> 2) + 1] = (memory[(address >> 2) + 1] & (0xFFFFFFFF << ((4 - endbits) << 3))) | (data >> ((4 - endbits) << 3));
    }
}