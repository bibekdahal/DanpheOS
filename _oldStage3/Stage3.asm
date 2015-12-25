

org	0x100000			; Kernel starts at 1 MB

bits	32				; 32 bit code

jmp	Stage3				; jump to entry point

%include "stdio.inc"

msg1 db  "-BIBEK's OS-",0
msg2 db  "KERNEL EXECUTING", 0x0A, 0

global start
start:
Stage3:
	;-------------------------------;
	;   Set registers		;
	;-------------------------------;

	mov	ax, 0x10		; set data segments to data selector (0x10)
	mov	ds, ax
	mov	ss, ax
	mov	es, ax
	mov	esp, 90000h		; stack begins from 90000h

	;---------------------------------------;
	;   Clear screen and print success	;
	;---------------------------------------;
    mov ah, 0x70
	call	ClrScr32
	;mov	ebx, msg
	;call	Puts32

    mov ah,2
    mov al,34
    call GotoXY
    mov ah, 0x7C
    mov ebx, msg1
    call Puts32

    mov ah,4
    mov al,32
    call GotoXY
    mov ah, 0x7C
    mov ebx, msg2
    call Puts32

	;---------------------------------------;
	;   Stop execution			;
	;---------------------------------------;

	cli
	hlt



