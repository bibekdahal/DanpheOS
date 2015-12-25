[BITS 32]				; 32 bit code
jmp	start				; jump to entry point

extern _kmain

;%include "stdio.inc"
%include "clib.inc"
%include "isr.inc"


;msg db  "-DANPHE OS-",0
;sgn db  "**********************************************************",0
bootinfo dd 0
[GLOBAL _KernelSize]
_KernelSize dd 0


[GLOBAL start]
start:
    pop dword[bootinfo]
    mov dword[_KernelSize],edx

	;;-------------------------------;
	;;-------------------------------;
	;;  disable interrupts,          ;
    ;;  call kernelMain() and        ;
    ;;  stop the processor           ;
    ;;-------------------------------;
	;;-------------------------------;

    cli
    push dword[bootinfo]
    call _kmain
    add esp, 4
	jmp $


[GLOBAL _gdt_flush]
_gdt_flush:
    mov eax, [esp+4]
    lgdt[eax]
    mov ax, 0x10        ;; data segment offset in GDT
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    jmp 0x08:.flush     ;; far jump to set cs to code segment offset in GDT
.flush:
    ret

[GLOBAL _idt_flush]
_idt_flush:
    mov eax, [esp+4]
    lidt[eax]
    ret

[GLOBAL _enablepaging]
_enablepaging:
    mov eax, cr0
    cmp dword[esp+4], 1
    je enable
    jmp disable
enable:
    or eax, 0x80000000
    mov	cr0, eax
    jmp done
disable:
    and eax, 0x7FFFFFFF
    mov	cr0, eax
done:
    ret



