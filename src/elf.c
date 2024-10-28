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

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <elf_file>\n", argv[0]);
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    Elf32_Ehdr header;
    read_elf_header(fd, &header);

    // Verifica se é um arquivo ELF
    if (memcmp(header.e_ident, "\x7f""ELF", 4) != 0) {
        fprintf(stderr, "Not an ELF file\n");
        close(fd);
        return 1;
    }

    // Lê todas as seções para encontrar .text, .data, .bss e a tabela de símbolos
    Elf32_Shdr text_section = {0}, data_section = {0}, bss_section = {0}, symtab_section = {0}, strtab_section = {0};
    for (int i = 0; i < header.e_shnum; i++) {
        Elf32_Shdr section;
        read_section_header(fd, &header, &section, i);

        if (header.e_shstrndx != SHN_UNDEF) {
            Elf32_Shdr shstrtab;
            read_section_header(fd, &header, &shstrtab, header.e_shstrndx);
            char *shstrtab_data = (char *) read_section(fd, &shstrtab);
            const char *section_name = &shstrtab_data[section.sh_name];

            if (strcmp(section_name, ".text") == 0) {
                text_section = section;
            } else if (strcmp(section_name, ".data") == 0) {
                data_section = section;
            } else if (strcmp(section_name, ".bss") == 0) {
                bss_section = section;
            } else if (section.sh_type == SHT_SYMTAB) {
                symtab_section = section;
            } else if (i == header.e_shstrndx) {
                strtab_section = section;
            }
            free(shstrtab_data);
        }
    }

    size_t total_size = text_section.sh_size + data_section.sh_size + bss_section.sh_size;
    void *concat_content = malloc(total_size);

    if (text_section.sh_size > 0) {
        void *text_content = read_section(fd, &text_section);
        printf(".text section extracted, size: %u bytes\n", text_section.sh_size);
        memcpy(concat_content, text_content, text_section.sh_size);
        free(text_content);
    }
    if (data_section.sh_size > 0) {
        void *data_content = read_section(fd, &data_section);
        printf(".data section extracted, size: %u bytes\n", data_section.sh_size);
        memcpy(concat_content + text_section.sh_size, data_content, data_section.sh_size);
        free(data_content);
    }
    if (bss_section.sh_size > 0) {
        memset(concat_content + text_section.sh_size + data_section.sh_size, 0, bss_section.sh_size);
        printf(".bss section allocated, size: %u bytes\n", bss_section.sh_size);
    }

    printf("Sections concatenated, total size: %zu bytes\n", total_size);

    printf("\nSymbol Table:\n");
    if (symtab_section.sh_size > 0 && strtab_section.sh_size > 0) {
        extract_symbols(fd, &symtab_section, &strtab_section);
    } else {
        printf("No symbol table found.\n");
    }

    free(concat_content);
    close(fd);
    return 0;
}
