extern _isr_handler

%macro ISR_NOERR 1      ;; param_no. = 1
[GLOBAL _isr%1]
_isr%1:
    cli
    push byte 0
    push byte %1
    jmp isr_common
%endmacro

%macro ISR_ERR 1      ;; param_no. = 1
[GLOBAL _isr%1]
_isr%1:
    cli
    push byte %1
    jmp isr_common
%endmacro

ISR_NOERR 0
ISR_NOERR 1
ISR_NOERR 2
ISR_NOERR 3
ISR_NOERR 4
ISR_NOERR 5
ISR_NOERR 6
ISR_NOERR 7
ISR_ERR   8
ISR_NOERR 9
ISR_ERR   10
ISR_ERR   11
ISR_ERR   12
ISR_ERR   13
ISR_ERR   14
ISR_NOERR 15
ISR_NOERR 16
ISR_NOERR 17
ISR_NOERR 18
ISR_NOERR 19
ISR_NOERR 20
ISR_NOERR 21
ISR_NOERR 22
ISR_NOERR 23
ISR_NOERR 24
ISR_NOERR 25
ISR_NOERR 26
ISR_NOERR 27
ISR_NOERR 28
ISR_NOERR 29
ISR_NOERR 30
ISR_NOERR 31


isr_common:
    pusha

    mov ax, ds
    push eax
    mov ax, 0x10        ;; kernel data segment, useful when in user mode
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call _isr_handler

    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popa
    add esp, 8
    sti
    iret        ;; IRET for returning from interrupt


%macro IRQ 2      ;; param_no. = 2
[GLOBAL _irq%1]
_irq%1:
    cli
    push byte 0
    push byte %2
    jmp irq_common
%endmacro

IRQ   0,    32
IRQ   1,    33
IRQ   2,    34
IRQ   3,    35
IRQ   4,    36
IRQ   5,    37
IRQ   6,    38
IRQ   7,    39
IRQ   8,    40
IRQ   9,    41
IRQ  10,    42
IRQ  11,    43
IRQ  12,    44
IRQ  13,    45
IRQ  14,    46
IRQ  15,    47

extern _irq_handler
irq_common:
    pusha

    mov ax, ds
    push eax
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call _irq_handler

    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popa
    add esp, 8
    sti
    iret        ;; IRET for returning from interrupt
