#include "log.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>


void print_error(const char *format, ...){
    // Iniciar a lista de argumentos
    va_list args;
    va_start(args, format);

    // Imprimir a mensagem de erro com a cor vermelha
    printf("\033[0;31mError: ");
    vprintf(format, args); // Usar vprintf para lidar com argumentos variáveis
    printf("\033[0m");

    // Finalizar a lista de argumentos
    va_end(args);
}


void print_info(const char *format, ...){
    // Iniciar a lista de argumentos
    va_list args;
    va_start(args, format);

    // Imprimir a mensagem de erro com a cor vermelha
    printf("\033[0;32mInfo: ");
    vprintf(format, args); // Usar vprintf para lidar com argumentos variáveis
    printf("\033[0m");

    // Finalizar a lista de argumentos
    va_end(args);
}


void print_warning(const char *format, ...){
    // Iniciar a lista de argumentos
    va_list args;
    va_start(args, format);

    // Imprimir a mensagem de erro com a cor vermelha
    printf("\033[0;33mWarning: ");
    vprintf(format, args); // Usar vprintf para lidar com argumentos variáveis
    printf("\033[0m");

    // Finalizar a lista de argumentos
    va_end(args);
}