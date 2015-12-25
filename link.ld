phys = 0x00100000;
virt_base = 0xC0000000;
SECTIONS
{ 
  . = phys + virt_base;
  .text :
  {
    mboot = (. - virt_base);
    code = (. - virt_base);
    *(.multiboot) 
    starter = (. - virt_base);
    *(.text)
    . = ALIGN(4096);
  }
  .data :
  {
    data = (. - virt_base);
    *(.data)
    . = ALIGN(4096);
  }
  .bss :
  {
    bss = (. - virt_base);
    *(.bss)
    . = ALIGN(4096);
  }
  end = (. - virt_base);
}