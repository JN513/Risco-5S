#ifndef __SIMULATOR_H__
#define __SIMULATOR_H__

#include <time.h>

//#define DEBUG 1

typedef enum InstType { R, I, S, B, U, J, INVALID } InstType; // Instruction types
typedef enum Mode { USER, SUPERVISOR, MACHINE } Mode; // Executions modes

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

extern int registers[32];         // Register array
extern float f_registers[32];     // Floating point register array
extern int pc;                    // Program counter
extern int pc_old;                // Program counter old value
extern int memory[MEMORY_SIZE];   // Memory array
extern int mode;                  // Execution mode
extern int inst_count;            // Instruction count
extern clock_t start, end;        // Clocks to measure execution time

/**
 * @brief Decode instruction from hexadecimal format to data structure
 * 
 * @param hex_instruction Instruction in hexadecimal format
 * @param instruction Instruction data structure
 * @return int Execution error code (0 = no error)
 */
int decode_instruction(int hex_instruction, InstData *instruction);

/**
 * @brief Execute instruction
 * 
 * @param instruction Instruction data structure
 * @return int Execution error code (0 = no error)
 */
int execute_instruction(InstData *instruction);

/**
 * @brief Execute especial instruction (system call)
 * 
 * @param instruction Instruction data structure
 * @return int Execution error code (0 = no error)
 */
int execute_system_instruction(InstData *instruction);

// Memory manipulation functions

/**
 * @brief Load a byte from memory
 * 
 * @param address Memory address
 * @return unsigned int Byte value
 */
unsigned int memory_load_byte(unsigned int address);

/**
 * @brief Load a halfword from memory
 * 
 * @param address Memory address
 * @return unsigned int Halfword value
 */
unsigned int memory_load_half(unsigned int address);

/**
 * @brief Load a word from memory
 * 
 * @param address Memory address
 * @return unsigned int Word value
 */
unsigned int memory_load_word(unsigned int address);

/**
 * @brief Load an unsigned byte from memory
 * 
 * @param address Memory address
 * @return unsigned int Unsigned byte value
 */
unsigned int memory_load_unsigned_byte(unsigned int address);

/**
 * @brief Load an unsigned halfword from memory
 * 
 * @param address Memory address
 * @return unsigned int Unsigned halfword value
 */
unsigned int memory_load_unsigned_half(unsigned int address);

/**
 * @brief Store a byte in memory
 * 
 * @param address Memory address
 * @param data Byte value
 */
void memory_store_byte(unsigned int address, unsigned int data);

/**
 * @brief Store a halfword in memory
 * 
 * @param address Memory address
 * @param data Halfword value
 */
void memory_store_halfword(unsigned int address, unsigned int data);

/**
 * @brief Store a word in memory
 * 
 * @param address Memory address
 * @param data Word value
 */
void memory_store_word(unsigned int address, unsigned int data);

// Simulator functions

/**
 * @brief Open memory file
 * 
 * @param filename Memory file name
 * @param memory Memory array
 * @return int Num of words read
 */
int open_memory_file(char *filename, int memory[]);

/**
 * @brief Initialize simulator, setting stack pointer and program counter
 * 
 * @return void
 */
void init_simulator();

/**
 * @brief Run simulator in debbug mode
 * 
 * @return int Execution error code (0 = no error)
 */
int run_debug();

/**
 * @brief Run simulator
 * 
 * @return int Execution error code (0 = no error)
 */
int run_simulator();

#endif // __SIMULATOR_H__
