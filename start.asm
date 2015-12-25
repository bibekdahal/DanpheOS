
BITS 32
    
KERNEL_VIRTUAL_BASE equ 0xC0000000
KERNEL_PDE_INDEX equ (KERNEL_VIRTUAL_BASE >> 22)

SECTION .multiboot
ALIGN 4
    MULTIBOOT_PAGE_ALIGN    equ 1<<0
    MULTIBOOT_MEMORY_INFO   equ 1<<1
    MULTIBOOT_AOUT_KLUDGE   equ 1<<16
    MULTIBOOT_HEADER_MAGIC  equ 0x1BADB002
    MULTIBOOT_HEADER_FLAGS  equ MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO | MULTIBOOT_AOUT_KLUDGE
    MULTIBOOT_CHECKSUM  equ -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)

    dd MULTIBOOT_HEADER_MAGIC
    dd MULTIBOOT_HEADER_FLAGS
    dd MULTIBOOT_CHECKSUM
    
     EXTERN code, bss, end, starter, mboot
     dd mboot
     dd code
     dd bss
     dd end
     dd starter

 
SECTION .text
EXTERN _kmain
GLOBAL start
start:

    push ebx
    
    mov ebx, (PageTable - KERNEL_VIRTUAL_BASE)          ; must use physical address before paging
    mov ecx, 1024
    mov edx, 0
    mov edi, 0
L1: mov dword[ebx+edi], edx
    or dword[ebx+edi], 3
    add edi, 4
    add edx, 4096
    loop L1

    mov ebx, (PageDirectory - KERNEL_VIRTUAL_BASE)
    mov dword[ebx+0], (PageTable - KERNEL_VIRTUAL_BASE)
    or dword[ebx+0], 3
    mov dword[ebx+KERNEL_PDE_INDEX*4], (PageTable - KERNEL_VIRTUAL_BASE)
    or dword[ebx+KERNEL_PDE_INDEX*4], 3

    mov cr3, ebx
    mov ecx, cr0
    or ecx, 0x80000000
    mov cr0, ecx

    mov ecx, HIGHER
    jmp ecx                ; absolute jump to change eip to higher address
HIGHER:
    pop ebx

    mov esp, _sys_stack
    push ebx                ; pass the multiboot info
    call _kmain
    cli
hang:
    hlt
    jmp hang        ; in case hlt doesn't work

GLOBAL _kernel_end
_kernel_end dd end

GLOBAL _gdt_flush
EXTERN _gdt_ptr
_gdt_flush:
    lgdt [_gdt_ptr]
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:flush2
flush2:
    ret

GLOBAL _idt_load
EXTERN _idt_ptr
_idt_load:
    lidt [_idt_ptr]
    ret

%include "isr.asm"
%include "irq.asm"

SECTION .data
ALIGN 4096
PageDirectory:
    times 1024 dd (0 | 2)

ALIGN 4096
PageTable:
    times 1024 dd 0

SECTION .bss
    resb 8192               ; Reserve 8KBytes for stack
_sys_stack: