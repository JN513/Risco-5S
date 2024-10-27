#include <stdio.h>
#include <string.h>
#include "simulator.h"
#include "log.h"


int run_program(){
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

int main(int argc, char * argv[]){
    if(argc < 2){
        printf("Usage: %s <memory file>\n", argv[0]);
        printf("Usage: %s <memory file> --debug\n", argv[0]);
        return 1;
    }

    open_memory_file(argv[1], memory);

    init_simulator();

    if(argc == 3 && strcmp(argv[2], "--debug") == 0){
        return run_debug();
    } else {
        return run_program();
    }

    return 0;
}