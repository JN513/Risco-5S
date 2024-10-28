#!/usr/bin/bash

mkdir -p build

# Verifica se um argumento foi passado, caso contrário executa read
if [ -z "$1" ]; then
    echo "Digite o nome do teste:"
    read input_name
else
    input_name=$1
fi

# Extrai o último nome do caminho e remove a extensão
nome_do_teste=$(basename "$input_name" .s)

riscv32-unknown-elf-as -march=rv32izicsr "$input_name" -o "build/$nome_do_teste.o"
riscv32-unknown-elf-ld -T utils/linker.ld "build/$nome_do_teste.o" -o "build/$nome_do_teste.elf"
riscv32-unknown-elf-objcopy -O binary "build/$nome_do_teste.elf" "build/$nome_do_teste.bin"
hexdump -v -e '1/4 "%08x" "\n"' "build/$nome_do_teste.bin" > "build/$nome_do_teste.hex"

echo "Teste $nome_do_teste, compilado"
