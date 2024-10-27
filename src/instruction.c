#include "simulator.h"
#include "log.h"
#include <stdio.h>
#include <stdlib.h>


void get_instruction_type(InstData * instruction){
    if(instruction->opcode == 0x33){
        instruction->type = R;
    } else if(instruction->opcode == 0x13){
        instruction->type = I;
    } else if(instruction->opcode == 0x3){
        instruction->type = I;
    } else if(instruction->opcode == 0x73){
        instruction->type = I;
    } else if(instruction->opcode == 0x67){
        instruction->type = I;
    } else if(instruction->opcode == 0x63){
        instruction->type = B;
    } else if(instruction->opcode == 0x23){
        instruction->type = S;
    } else if(instruction->opcode == 0x37 || instruction->opcode == 0x17){
        instruction->type = U;
    } else if(instruction->opcode == 0x6F){
        instruction->type = J;
    } else {
        instruction->type = INVALID;
    }
}

int decode_instruction(int hex_instruction, InstData * instruction){
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

            // Extensão de sinal para garantir valores negativos
            if (instruction->imm & 0x800) {
                instruction->imm |= 0xFFFFF000;
            }
            break;

        case S:
            instruction->imm = (hex_instruction & 0xFE000000) >> 20; // Bits [31:25]
            instruction->imm |= (hex_instruction & 0xF80) >> 7;      // Bits [11:7]
            instruction->funct3 = (hex_instruction & 0x7000) >> 12;
            instruction->rs1 = (hex_instruction & 0xF8000) >> 15;
            instruction->rs2 = (hex_instruction & 0x1F00000) >> 20;

            // Extensão de sinal para garantir valores negativos
            if (instruction->imm & 0x1000) {
                instruction->imm |= 0xFFFFE000;
            }
            break;

        case B:
            instruction->imm = (hex_instruction & 0x80000000) >> 19;  // Bit 12 (bit 31 da instrução)
            instruction->imm |= (hex_instruction & 0x80) << 4;         // Bit 11 (bit 7 da instrução)
            instruction->imm |= (hex_instruction & 0x7E000000) >> 20;  // Bits [10:5]
            instruction->imm |= (hex_instruction & 0xF00) >> 7;        // Bits [4:1]
            instruction->funct3 = (hex_instruction & 0x7000) >> 12;
            instruction->rs1 = (hex_instruction & 0xF8000) >> 15;
            instruction->rs2 = (hex_instruction & 0xF80) >> 20;

            // Extensão de sinal: se o bit 12 for 1, aplica extensão para 32 bits
            if (instruction->imm & 0x1000) {
                instruction->imm |= 0xFFFFE000;
            }
            break;


        case U:
            instruction->rd = (hex_instruction & 0xF80) >> 7;
            instruction->imm = (hex_instruction & 0xFFFFF000);

            break;

        case J:
            instruction->rd = (hex_instruction & 0xF80) >> 7;
            instruction->imm = (hex_instruction & 0x80000000) >> 11;
            instruction->imm |= (hex_instruction & 0xFF000);
            instruction->imm |= (hex_instruction & 0x7FE00000) >> 20;
            instruction->imm |= (hex_instruction & 0x100000) >> 9;
            
            if (instruction->imm & 0x100000) { // Expansão de sinal
                instruction->imm |= 0xFFE00000;
            }
            
            break;

    
        default:
            return -1;
            break;
    }

#ifdef DEBUG
    printf("Instruction: 0x%.8X\n", hex_instruction);
    printf("Opcode: %d\n", instruction->opcode);
    printf("Type: %d\n", instruction->type);
    printf("RD: %d\n", instruction->rd);
    printf("RS1: %d\n", instruction->rs1);
    printf("RS2: %d\n", instruction->rs2);
    printf("IMM: 0x%.8X\n", instruction->imm);
    printf("Funct3: %d\n", instruction->funct3);
    printf("Funct7: %d\n", instruction->funct7);
    printf("\n");
#endif

    return 0;
}


// Função que executa as instruções de tipo R para o conjunto de instruções RV32I.
int execute_RType(InstData * instruction){
    // Decodifica o campo funct3 da instrução para identificar a operação.

#ifdef DEBUG
    print_info("Executing R-Type instruction\n");
#endif

    switch (instruction->funct3)
    {
    case 0: // ADD ou SUB, dependendo de funct7
        if(instruction->funct7 == 0) {
            // ADD: Soma o valor dos registradores rs1 e rs2 e armazena o resultado em rd.
            registers[instruction->rd] = registers[instruction->rs1] + 
                registers[instruction->rs2];
        } else {
            // SUB: Subtrai o valor de rs2 de rs1 e armazena o resultado em rd.
            registers[instruction->rd] = registers[instruction->rs1] - 
                registers[instruction->rs2];
        }
        break;
    
    case 1: // SLL (Shift Left Logical)
        // Desloca o valor em rs1 para a esquerda pelo valor em rs2 e armazena em rd.
        registers[instruction->rd] = registers[instruction->rs1] << 
                registers[instruction->rs2];
        break;

    case 2: // SLT (Set Less Than)
        // Define rd como 1 se rs1 for menor que rs2, caso contrário, define como 0.
        registers[instruction->rd] = registers[instruction->rs1] < 
                registers[instruction->rs2];
        break;

    case 3: // SLTU (Set Less Than Unsigned)
        // Compara rs1 e rs2 como valores sem sinal. Define rd como 1 se rs1 < rs2, caso contrário, 0.
        registers[instruction->rd] = (unsigned)registers[instruction->rs1] < 
                (unsigned)registers[instruction->rs2];
        break;

    case 4: // XOR
        // Realiza a operação XOR bit a bit entre rs1 e rs2 e armazena o resultado em rd.
        registers[instruction->rd] = registers[instruction->rs1] ^ 
                registers[instruction->rs2];
        break;

    case 5: // SRL ou SRA (Shift Right Logical ou Shift Right Arithmetic), dependendo de funct7
        if(instruction->funct7 == 0){
            // SRL: Desloca o valor em rs1 para a direita de forma lógica e armazena em rd.
            registers[instruction->rd] = (unsigned)registers[instruction->rs1] >> 
                (unsigned)registers[instruction->rs2];
        } else {
            // SRA: Desloca o valor em rs1 para a direita de forma aritmética e armazena em rd.
            registers[instruction->rd] = registers[instruction->rs1] >> 
                registers[instruction->rs2];
        }
        break;

    case 6: // OR
        // Realiza a operação OR bit a bit entre rs1 e rs2 e armazena o resultado em rd.
        registers[instruction->rd] = registers[instruction->rs1] | 
                registers[instruction->rs2];
        break;

    case 7: // AND
        // Realiza a operação AND bit a bit entre rs1 e rs2 e armazena o resultado em rd.
        registers[instruction->rd] = registers[instruction->rs1] & 
                registers[instruction->rs2];
        break;

    default:
        // Caso a operação não seja reconhecida, retorna erro (-1).
        return -1;
        break;
    }

    // Se a execução da instrução for bem-sucedida, retorna 0.
    return 0;
}

int execute_IType(InstData *instruction) {
    int imm = instruction->imm;

#ifdef DEBUG
    print_info("Executing I-Type instruction\n");
#endif
    
    // Verifica se o opcode é de load ou de instrução aritmética
    if (instruction->opcode == 0x03) { // opcode para instruções de load
        switch (instruction->funct3) {
            case 0: // LB
                registers[instruction->rd] = memory_load_byte(registers[instruction->rs1] + imm);
                break;
            case 1: // LH
                registers[instruction->rd] = memory_load_half(registers[instruction->rs1] + imm);
                break;
            case 2: // LW
                registers[instruction->rd] = memory_load_word(registers[instruction->rs1] + imm);
                break;
            case 4: // LBU
                registers[instruction->rd] = memory_load_unsigned_byte(registers[instruction->rs1] + imm);
                break;
            case 5: // LHU
                registers[instruction->rd] = memory_load_unsigned_half(registers[instruction->rs1] + imm);
                break;
            default:
                return -1;
        }
    } else if (instruction->opcode == 0x13) { // opcode para instruções aritméticas imediatas
        switch (instruction->funct3) {
            case 0: // ADDI
                registers[instruction->rd] = registers[instruction->rs1] + imm;
                break;
            case 4: // XORI
                registers[instruction->rd] = registers[instruction->rs1] ^ imm;
                break;
            case 6: // ORI
                registers[instruction->rd] = registers[instruction->rs1] | imm;
                break;
            case 7: // ANDI
                registers[instruction->rd] = registers[instruction->rs1] & imm;
                break;
            case 1: // SLLI
                registers[instruction->rd] = registers[instruction->rs1] << (imm & 0x1F);
                break;
            case 5: // SRLI ou SRAI
                if (instruction->funct7 == 0) { // SRLI
                    registers[instruction->rd] = (unsigned)registers[instruction->rs1] >> (imm & 0x1F);
                } else { // SRAI
                    registers[instruction->rd] = registers[instruction->rs1] >> (imm & 0x1F);
                }
                break;
            default:
                return -1;
        }
    } else if(instruction->opcode == 0x73) { // opcode para instruções do tipo I de sistema
        return execute_system_instruction(instruction);
    } else if(instruction->opcode == 0x67) { // opcode para instruções do tipo I de salto incondicional
        pc = registers[instruction->rs1] + imm;
        registers[instruction->rd] = pc_old + 4;
    } else {
        return -1; // opcode inválido para instruções do tipo I
    }
    
    return 0;
}


// Execute RISC-V S instruction type
int execute_SType(InstData *instruction) {
    int imm = instruction->imm;

#ifdef DEBUG
    print_info("Executing S-Type instruction\n");
#endif

    switch (instruction->funct3) {
        case 0: // SB (store byte)
            memory_store_byte(registers[instruction->rs1] + imm, registers[instruction->rs2]);
            break;
        case 1: // SH (store halfword)
            memory_store_halfword(registers[instruction->rs1] + imm, registers[instruction->rs2]);
            break;
        case 2: // SW (store word)
            memory_store_word(registers[instruction->rs1] + imm, registers[instruction->rs2]);
            break;
        default:
            return -1;
    }
    return 0;
}

// Execute RISC-V B instruction type
int execute_BType(InstData *instruction) {
    int imm = instruction->imm;

#ifdef DEBUG
    print_info("Executing B-Type instruction\n");
#endif

    switch (instruction->funct3) {
        case 0: // BEQ
            if (registers[instruction->rs1] == registers[instruction->rs2]) {
                pc += imm;
            }
            break;
        case 1: // BNE
            if (registers[instruction->rs1] != registers[instruction->rs2]) {
                pc += imm;
            }
            break;
        case 4: // BLT
            if (registers[instruction->rs1] < registers[instruction->rs2]) {
                pc += imm;
            }
            break;
        case 5: // BGE
            if (registers[instruction->rs1] >= registers[instruction->rs2]) {
                pc += imm;
            }
            break;
        case 6: // BLTU
            if ((unsigned)registers[instruction->rs1] < (unsigned)registers[instruction->rs2]) {
                pc += imm;
            }
            break;
        case 7: // BGEU
            if ((unsigned)registers[instruction->rs1] >= (unsigned)registers[instruction->rs2]) {
                pc += imm;
            }
            break;
        default:
            return -1;
    }
    return 0;
}

// Execute RISC-V U instruction type
int execute_UType(InstData *instruction) {

#ifdef DEBUG
    print_info("Executing U-Type instruction\n");
#endif

    switch (instruction->opcode) {
        case 0x37: // LUI
            registers[instruction->rd] = instruction->imm;
            break;
        case 0x17: // AUIPC
            registers[instruction->rd] = pc_old + instruction->imm;
            break;
        default:
            return -1;
    }
    return 0;
}

// Execute RISC-V J instruction type
int execute_JType(InstData *instruction) {

#ifdef DEBUG
    print_info("Executing J-Type instruction\n");
#endif

    int imm = instruction->imm;
    registers[instruction->rd] = pc_old + 4;
    pc = pc_old + imm;
    return 0;
}

// Main instruction dispatcher
int execute_instruction(InstData *instruction) {
    registers[0] = 0; // Register x0 is always zero

    switch (instruction->type) {
        case R:
            return execute_RType(instruction);
        case I:
            return execute_IType(instruction);
        case S:
            return execute_SType(instruction);
        case B:
            return execute_BType(instruction);
        case U:
            return execute_UType(instruction);
        case J:
            return execute_JType(instruction);
        default:
            return -1;
    }

    registers[0] = 0; // Register x0 is always zero

    return 0;
}