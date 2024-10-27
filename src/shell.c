#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "simulator.h"
#include "log.h"

#define MAX_ARGS 100

void make_step(){
    InstData instruction;
    int ret;

    ret = decode_instruction(memory[pc >> 2], &instruction);
    if(ret == -1){
        print_error("Ilegal Instruction: 0x%.8X, address: 0x%.8X\n", memory[pc >> 2], pc);
        return;
    }

    pc_old = pc;
    pc += 4;

    printf("PC: 0x%.8X, Inst C: %d, Hex Ints: 0x%.8X, Opcode: 0x%.2X, RD: %d, RS1: %d, RS2: %d, type: %d\n", pc_old, inst_count, memory[pc_old >> 2], 
        instruction.opcode, instruction.rd, instruction.rs1, instruction.rs2, instruction.type);

    execute_instruction(&instruction);

    inst_count++;
}

void execute_command(char **args) {
    if (strcmp(args[0], "step") == 0) {
        int steps = 1;
        if (args[1]) {
            steps = atoi(args[1]);
        }
        for (int i = 0; i < steps; i++){
            make_step();
        }
    } else if (strcmp(args[0], "until") == 0) {
        int breakpoint = (int)strtol(args[1], NULL, 16);
        while (1) {
            make_step();
            if (pc == breakpoint){
                break;
            }
        }
        
    } else if (strcmp(args[0], "peek") == 0) {
        if(args[1][0] == 'r'){
            printf("Register: 0x%.8X\n", registers[atoi(args[1] + 1)]);
        } else {
            printf("Memory: 0x%.8X\n", memory[atoi(args[1]) >> 2]);
        }
    } else if (strcmp(args[0], "write-stdin") == 0) {

    } else if (strcmp(args[0], "exit") == 0) {

    } else {
        printf("Comando desconhecido: %s\n", args[0]);
    }  
}


// Função de autocompletar (fornecida para readline)
char *command_generator(const char *text, int state) {
    static int list_index, len;
    char *commands[] = {"step", "until", "peek", "write-stdin", "exit", NULL};

    if (!state) {
        list_index = 0;
        len = strlen(text);
    }

    while (commands[list_index]) {
        char *command = commands[list_index++];
        if (strncmp(command, text, len) == 0) {
            return strdup(command);
        }
    }
    return NULL;
}

// Função de inicialização de autocompletar
char **command_completion(const char *text, int start, int end) {
    rl_attempted_completion_over = 1;
    return rl_completion_matches(text, command_generator);
}

int run_debug(){
    char *input;
    char *args[MAX_ARGS];

    // Configura o autocompletar para comandos internos
    rl_attempted_completion_function = command_completion;

    while (1) {
        input = readline("Risco 5S> ");
        if (!input) break;

        if (*input) add_history(input); // Adiciona ao histórico

        int i = 0;
        args[i] = strtok(input, " ");
        while (args[i] != NULL && i < MAX_ARGS - 1) {
            i++;
            args[i] = strtok(NULL, " ");
        }
        args[i] = NULL;

        if (args[0] && strcmp(args[0], "exit") == 0) {
            free(input);
            break;
        }

        execute_command(args);
        free(input);
    }

    return 0;
}
