#include "simulator.h"
#include <stdio.h>
#include <stdlib.h>

int registers[32];                // Definição da variável global
float f_registers[32];            // Definição da variável global
int pc;                           // Definição da variável global
int memory[MEMORY_SIZE];          // Definição da variável global


int open_memory_file(char * filename, int memory[]){
    FILE * file = fopen(filename, "r");
    if(file == NULL){
        printf("Error opening file\n");
        exit(1);
    }

#ifdef DEBUG
    printf("DEBUG: File opened\n");
#endif

    int i = 0;
    while(fscanf(file, "%x", &memory[i]) != EOF){
#ifdef DEBUG
    printf("DEBUG: Memory[%d] = 0x%.8X\n", i, memory[i]);
#endif
        i++;
        if(i >= MEMORY_SIZE){
            printf("Memory file too large\n");
            exit(1);
        }
    }

    fclose(file);

    return i;
}


void init_simulator(){
    pc = 0;

    // Inicializa SP
    registers[2] = (MEMORY_SIZE-1) * 4;

    // Inicializa GP
    registers[3] = MEMORY_SIZE * 4 - 1;

    // Inicializa FP
    registers[8] = (MEMORY_SIZE-1) * 4;
}