#include "log.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>


/**
 * @brief Print error message in red color
 * 
 * @param format 
 * @param ... 
 * @return void 
 */
void print_error(const char *format, ...){
   // Initiate the argument list
    va_list args;
    va_start(args, format);

    // Print the message with red color
    printf("\033[0;31mError: ");
    vprintf(format, args); // Use vprintf to handle variable arguments
    printf("\033[0m");

    // End the argument list
    va_end(args);
}


/**
 * @brief Print info message in green color
 * 
 * @param format 
 * @param ... 
 * @return void 
 */
void print_info(const char *format, ...){
    // Initiate the argument list
    va_list args;
    va_start(args, format);

    // Print the message with green color
    printf("\033[0;32mInfo: ");
    vprintf(format, args); // Use vprintf to handle variable arguments
    printf("\033[0m");

    // End the argument list
    va_end(args);
}


/**
 * @brief Print warning message in yellow color
 * 
 * @param format 
 * @param ... 
 * @return void 
 */
void print_warning(const char *format, ...){
   // Initiate the argument list
    va_list args;
    va_start(args, format);

    // Print the message with yellow color
    printf("\033[0;33mWarning: ");
    vprintf(format, args); // Use vprintf to handle variable arguments
    printf("\033[0m");

    // End the argument list
    va_end(args);
}