#ifndef __SIMULATOR_H__
#define __SIMULATOR_H__

#define MEMORY_SIZE 4096

static int registers[32];
static float f_registers[32]; // 32 floating point registers
static int pc;

int memory[MEMORY_SIZE];

#endif