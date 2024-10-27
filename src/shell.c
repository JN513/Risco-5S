#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

#define MAX_ARGS 100

void execute_command(char **args) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("Erro ao criar processo");
    } else if (pid == 0) {
        if (execvp(args[0], args) == -1) {
            perror("Erro ao executar comando");
        }
        exit(EXIT_FAILURE);
    } else {
        int status;
        waitpid(pid, &status, 0);
    }
}


// Função de autocompletar (fornecida para readline)
char *command_generator(const char *text, int state) {
    static int list_index, len;
    char *commands[] = {"step", "until", "peek", "write-stdin", "ls", NULL};

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

void run(){
    char *input;
    char *args[MAX_ARGS];

    // Configura o autocompletar para comandos internos
    rl_attempted_completion_function = command_completion;

    while (1) {
        input = readline("meu_shell> ");
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
}
