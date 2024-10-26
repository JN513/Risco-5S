#ifndef __INSTRUCTION_H__
#define __INSTRUCTION_H__

typedef enum InstType { R, I, S, B, U, J} InstType;

typedef struct InstData {
    int opcode,
        rd,
        rs1,
        rs2,
        imm,
        funct3,
        funct7;
    InstType type;
} InstData;


void decode_instruction(int hex_instruction, InstData * instruction);


#endif