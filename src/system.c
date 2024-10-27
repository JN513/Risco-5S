#include "simulator.h"
#include <stdio.h>
#include <stdlib.h>


int execute_system_instruction(InstData * instruction){
    switch (instruction->funct3)
    {
    case 0: // ECALL
        printf("System call\n");
        break;
    
    case 1: // EBREAK
        printf("Breakpoint\n");
        break;
    
    default:
        return -1;
    }
    return 0;
}