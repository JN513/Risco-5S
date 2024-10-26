#include "instruction.h"

void get_instruction_type(InstData * instruction){
    if(instruction->opcode == 0x33){
        instruction->type = R;
    } else if(instruction->opcode == 0x13){
        instruction->type = I;
    } else if(instruction->opcode == 0x3){
        instruction->type = I;
    } else if(instruction->opcode == 0x63){
        instruction->type = B;
    } else if(instruction->opcode == 0x23){
        instruction->type = S;
    } else if(instruction->opcode == 0x37 || instruction->opcode == 0x17){
        instruction->type = U;
    } else if(instruction->opcode == 0x6F){
        instruction->type = J;
    }
}

void decode_instruction(int hex_instruction, InstData * instruction){
    instruction->opcode = hex_instruction & 0x7F;

    get_instruction_type(instruction);

    switch (instruction->type)
    {
        case R:
            instruction->rd = (hex_instruction & 0xF80) >> 7;
            instruction->funct3 = (hex_instruction & 0x7000) >> 12;
            instruction->rs1 = (hex_instruction & 0xF8000) >> 15;
            instruction->rs2 = (hex_instruction & 0x1F00000) >> 20;
            instruction->funct7 = (hex_instruction & 0xFE000000) >> 25;

            break;

        case I:
            instruction->rd = (hex_instruction & 0xF80) >> 7;
            instruction->funct3 = (hex_instruction & 0x7000) >> 12;
            instruction->rs1 = (hex_instruction & 0xF8000) >> 15;
            instruction->imm = (hex_instruction & 0xFFF00000) >> 20;

            break;

        case S:
            instruction->imm = (hex_instruction & 0xFE000000) >> 25;
            instruction->funct3 = (hex_instruction & 0x7000) >> 12;
            instruction->rs1 = (hex_instruction & 0xF8000) >> 15;
            instruction->rs2 = (hex_instruction & 0xF80) >> 7;

            break;

        case B:
            instruction->imm = (hex_instruction & 0x80000000) >> 31;
            instruction->imm |= (hex_instruction & 0x80) >> 7;
            instruction->imm |= (hex_instruction & 0xF00) >> 20;
            instruction->imm |= (hex_instruction & 0x7E000000) >> 21;
            instruction->funct3 = (hex_instruction & 0x7000) >> 12;
            instruction->rs1 = (hex_instruction & 0xF8000) >> 15;
            instruction->rs2 = (hex_instruction & 0xF80) >> 7;

            break;

        case U:
            instruction->rd = (hex_instruction & 0xF80) >> 7;
            instruction->imm = (hex_instruction & 0xFFFFF000);

            break;

        case J:
            instruction->rd = (hex_instruction & 0xF80) >> 7;
            instruction->imm = (hex_instruction & 0x80000000) >> 31;
            instruction->imm |= (hex_instruction & 0xFF000) >> 20;
            instruction->imm |= (hex_instruction & 0x7FE00000) >> 21;
            instruction->imm |= (hex_instruction & 0x100000) >> 10;

            break;
    
        default:
            break;
    }
}