#include "elf.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>


void read_elf_header(int fd, Elf32_Ehdr *header) {
    lseek(fd, 0, SEEK_SET);
    read(fd, header, sizeof(header));
}


void *read_section(int fd, Elf32_Shdr *section_header) {
    void *section_data = malloc(section_header->sh_size);
    lseek(fd, section_header->sh_offset, SEEK_SET);
    read(fd, section_data, section_header->sh_size);
    return section_data;
}


void read_section_header(int fd, Elf32_Ehdr *header, Elf32_Shdr *section, int index) {
    lseek(fd, header->e_shoff + index * header->e_shentsize, SEEK_SET);
    read(fd, &section, sizeof(section));
}


void extract_symbols(int fd, Elf32_Shdr *symtab_section, Elf32_Shdr *strtab_section) {
    Elf32_Sym *symbols = (Elf32_Sym *) read_section(fd, symtab_section);
    char *strtab = (char *) read_section(fd, strtab_section);
    int num_symbols = symtab_section->sh_size / symtab_section->sh_entsize;

    for (int i = 0; i < num_symbols; i++) {
        const char *name = &strtab[symbols[i].st_name];
        
        // Adicionando verificação para endereços e tamanhos
        if (symbols[i].st_shndx != SHN_UNDEF) {
            printf("Symbol: %s, Address: 0x%x, Size: %u\n", name, symbols[i].st_value, symbols[i].st_size);
        } else {
            printf("Symbol: %s (undefined), Address: 0x%x, Size: %u\n", name, symbols[i].st_value, symbols[i].st_size);
        }
    }
    free(symbols);
    free(strtab);
}
