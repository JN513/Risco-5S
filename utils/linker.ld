MEMORY
{
    MEM (rwx) : ORIGIN = 0x00000000, LENGTH = 16K
}

SECTIONS
{
    . = 0x00000000;  /* Endereço inicial da memória */

    .text : {
        *(.text)       /* Código */
    } > MEM

    .data : {
        *(.data)       /* Dados */
    } > MEM

    .bss : {
        *(.bss)        /* Dados não inicializados */
    } > MEM
}
