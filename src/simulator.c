#include "simulator.h"
#include "log.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int registers[32];                // Definição da variável global
float f_registers[32];            // Definição da variável global
int pc, pc_old;                           // Definição da variável global
int memory[MEMORY_SIZE];          // Definição da variável global
int mode;                         // Definição da variável global
int inst_count = 0;
clock_t start, end;

int open_memory_file(char * filename, int memory[]){
    FILE * file = fopen(filename, "r");
    if(file == NULL){
        print_error("Error opening file\n");
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
            print_error("Memory file too large\n");
            exit(1);
        }
    }

    fclose(file);

    return i;
}

int run_simulator(){
    int ret;

    while(1){
        InstData instruction;
#ifdef DEBUG
        printf("PC: 0x%.8X\nPC old: 0x%.8X\n", pc, pc_old);
#endif
        ret = decode_instruction(memory[pc >> 2], &instruction);
        
        if(ret == -1){
            print_error("Ilegal Instruction: 0x%.8X, address: 0x%.8X\n", memory[pc >> 2], pc);
            return 1;
        }

        pc_old = pc;
        pc += 4;

        execute_instruction(&instruction);

        inst_count++;
    }

    return 0;
}

void init_simulator(){
    pc = 0;
    pc_old = 0;
    mode = MACHINE;
    registers[0] = 0;
    inst_count = 0;

    // Inicializa SP
    registers[2] = (MEMORY_SIZE-1) * 4;

    // Inicializa GP
    registers[3] = MEMORY_SIZE * 4 - 1;

    // Inicializa FP
    registers[8] = (MEMORY_SIZE-1) * 4;

    start = clock();
}
