phys = 0xC0000000;
SECTIONS
{
    .text phys :
    {
    code = .;
    *(.text)
    *(.rodata)
    . = ALIGN(4096);
    }
    .data :
    {
    data = .;
    *(.data)
    . = ALIGN(4096);
    }
    .bss :
    {
    bss = .;
    *(.bss)
    . = ALIGN(4096);
    }
    end = .;
}
