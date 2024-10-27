#ifndef __ELF_H__
#define __ELF_H__

#include <stdint.h>

#define EI_NIDENT 16
#define SHT_SYMTAB 2
#define SHN_UNDEF 0

// Estruturas de 32 bits para um ELF de 32 bits
typedef struct {
    unsigned char e_ident[EI_NIDENT];
    uint16_t e_type;
    uint16_t e_machine;
    uint32_t e_version;
    uint32_t e_entry;
    uint32_t e_phoff;
    uint32_t e_shoff;
    uint32_t e_flags;
    uint16_t e_ehsize;
    uint16_t e_phentsize;
    uint16_t e_phnum;
    uint16_t e_shentsize;
    uint16_t e_shnum;
    uint16_t e_shstrndx;
} Elf32_Ehdr;

typedef struct {
    uint32_t sh_name;
    uint32_t sh_type;
    uint32_t sh_flags;
    uint32_t sh_addr;
    uint32_t sh_offset;
    uint32_t sh_size;
    uint32_t sh_link;
    uint32_t sh_info;
    uint32_t sh_addralign;
    uint32_t sh_entsize;
} Elf32_Shdr;

typedef struct {
    uint32_t st_name;
    uint8_t st_info;
    uint8_t st_other;
    uint16_t st_shndx;
    uint32_t st_value;
    uint32_t st_size;
} Elf32_Sym;


/**
 * @brief Reads the ELF header from a file descriptor
 * 
 * @param fd File descriptor
 * @param header ELF header structure pointer
 * @return void
 */
void read_elf_header(int fd, Elf32_Ehdr *header);


/**
 * @brief Reads a section from a file descriptor
 * 
 * @param fd File descriptor
 * @param section_header Section header structure pointer
 * @return void* Pointer to the section data
 */
void *read_section(int fd, Elf32_Shdr *section_header);


/**
 * @brief Reads a section header from a file descriptor
 * 
 * @param fd File descriptor
 * @param header ELF header structure pointer
 * @param section Section header structure pointer
 * @param index Section index
 * @return void
 */
void read_section_header(int fd, Elf32_Ehdr *header, Elf32_Shdr *section, int index);


/**
 * @brief Extracts symbols from a symbol table section
 * 
 * @param fd File descriptor
 * @param symtab_section Symbol table section header
 * @param strtab_section String table section header
 * @return void
 */
void extract_symbols(int fd, Elf32_Shdr *symtab_section, Elf32_Shdr *strtab_section);

#endif // __ELF_H__