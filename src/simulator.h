#ifndef __SIMULATOR_H__
#define __SIMULATOR_H__

#include <time.h>

//#define DEBUG 1

typedef enum InstType { R, I, S, B, U, J, INVALID } InstType;
typedef enum Mode { USER, SUPERVISOR, MACHINE } Mode;

typedef struct InstData {
    int opcode;
    int rd;
    int rs1;
    int rs2;
    int imm;
    int funct3;
    int funct7;
    InstType type;
} InstData;

#define MEMORY_SIZE 4096

extern int registers[32];         // Declare a variável global como extern
extern float f_registers[32];     // Declare a variável global como extern
extern int pc;
extern int pc_old;
extern int memory[MEMORY_SIZE];   // Declare a variável global como extern
extern int mode;                  // Declare a variável global como extern
extern int inst_count;
extern clock_t start, end;

// Funções para manipulação de memória
int decode_instruction(int hex_instruction, InstData *instruction);
unsigned int memory_load_byte(unsigned int address);
unsigned int memory_load_half(unsigned int address);
unsigned int memory_load_word(unsigned int address);
unsigned int memory_load_unsigned_byte(unsigned int address);
unsigned int memory_load_unsigned_half(unsigned int address);
void memory_store_byte(unsigned int address, unsigned int data);
void memory_store_halfword(unsigned int address, unsigned int data);
void memory_store_word(unsigned int address, unsigned int data);
int open_memory_file(char *filename, int memory[]);
int execute_instruction(InstData *instruction);
int execute_system_instruction(InstData *instruction);
void init_simulator();
int run_debug();

#endif // __SIMULATOR_H__
