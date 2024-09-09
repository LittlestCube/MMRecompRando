RAMBASE    = 0x81000000;

MEMORY {
    extram(ARWX) : ORIGIN = RAMBASE, LENGTH = 64M
}

SECTIONS {
    /* All of the sections needed will get passed through to output sections automatically. */
    /* Therefore, all that's needed in the sections command is the discards. */

    /DISCARD/ : { *(.got) *(.MIPS.abiflags) *(.reginfo) *(.pdr) *(.comment) }
}
