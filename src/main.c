#include <stdio.h>
#include <string.h>
#include "simulator.h"

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
        return run_simulator();
    }

    return 0;
}