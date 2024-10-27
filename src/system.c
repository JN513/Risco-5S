#include "simulator.h"
#include "log.h"
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>


#define STDIN  0
#define STDOUT 1
#define STDERR 2


void execute_read_syscall(){
    int fd = registers[10];
    int size = registers[12];
    char *ptr = (char *)(memory + (registers[11] >> 2));
    ptr += registers[11] & 0x3;

    registers[10] = read(fd, ptr, size);
}

void execute_write_syscall(){
    int fd = registers[10];
    int size = registers[12];

    char *ptr = (char *)(memory + (registers[11] >> 2));
    ptr += registers[11] & 0x3;

    registers[10] = write(fd, ptr, size);
}

void execute_ecall(){
    switch(registers[17]){ // Get ecall code
        case 57: //close
            close(registers[10]);
            break;
        case 63: //read
            execute_read_syscall();
            break;
        case 64: //write
            execute_write_syscall();
            break;
        case 93: //exit
            end = clock();
            print_info("Program exited with code %d\n", registers[10]);
            print_info("Instructions executed: %d\n", inst_count);
    
            unsigned int cycles = end - start;
            double elapsed = (double)(end - start)/CLOCKS_PER_SEC;

            print_info("Time measured: %.9f seconds.\n", elapsed);
            print_info("Cycles measured: %d\n", cycles);
            exit(registers[10]);
            break;
        case 1024: //open
            registers[10] = open((char *)(memory + registers[10]), 
                registers[11], registers[12]);
            break;

        default:
            print_error("Invalid syscall code %d\n", registers[17]);
            exit(1);
            break;
    }
}


int execute_system_instruction(InstData * instruction){
    mode = MACHINE;

    switch (instruction->imm)
    {
    case 0: // ECALL
        execute_ecall();
        break;
    
    case 1: // EBREAK
        printf("Breakpoint\n");
        break;
    
    default:
        return -1;
    }
    return 0;
}