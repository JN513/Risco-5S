#include <stdio.h>
#include "simulator.h"
#include "log.h"

//#define DEBUG 1

int main(int argc, char * argv[]){
    if(argc < 2){
        printf("Usage: %s <memory file>\n", argv[0]);
        return 1;
    }

    int size = open_memory_file(argv[1], memory);
    int ret;

    printf("Memory size: %d\n", size);

    init_simulator();

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

        //printf("Opcode: %d\n", instruction.opcode);
    }

    for(int i = 0; i < 32; i++){
        printf("x%d: %d\n", i, registers[i]);
    }

    return 0;
}