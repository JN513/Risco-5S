#include <stdio.h>
#include "simulator.h"

#define DEBUG 1

int main(int argc, char * argv[]){
    int size = open_memory_file(argv[1], memory);
    int ret;

    printf("Memory size: %d\n", size);

    init_simulator();

    while(pc < size * 4){
        InstData instruction;
        ret = decode_instruction(memory[pc >> 2], &instruction);
        pc += 4;
        
        if(ret == -1){
            printf("Ilegal Instruction: 0x%.8X\n", memory[pc >> 2]);
            return 1;
        }

        execute_instruction(&instruction);

        printf("Opcode: %d\n", instruction.opcode);
    }

    for(int i = 0; i < 32; i++){
        printf("x%d: %d\n", i, registers[i]);
    }

    return 0;
}