
bits 16
org 0x500

jmp main


;*******************************************
; Global Descriptor Table (GDT)
;*******************************************

gdt_data:
	dd 0                ; null descriptor
	dd 0

; gdt code:	            ; code descriptor
	dw 0FFFFh
	dw 0
	db 0
	db 10011010b
	db 11001111b
	db 0

; gdt data:	            ; data descriptor
	dw 0FFFFh
	dw 0
	db 0
	db 10010010b
	db 11001111b
	db 0
end_of_gdt:
;********************************************
toc:
	dw end_of_gdt - gdt_data - 1
	dd gdt_data
;********************************************

;************************************************
; wait for input buffer to be clear
wait_input:
    in      al,0x64
    test    al,2
    jnz     wait_input
    ret

; wait for output buffer to be clear
wait_output:
    in      al,0x64
    test    al,1
    jz      wait_output
    ret
;************************************************


;************************************************
%define IMAGE_PMODE_BASE  0xC0000000		;0x100000       ;For PMode: Virtual Memory 3GB instead of 1MB
%define IMAGE_RMODE_BASE 0x3000         ;For Real Mode

ImageName     db "KRNL    SYS"
ImageSize     db 0

    %define ROOT_OFFSET 0x2e00  ; root directory to 0x7e00  = 0x5000 + ROOT_OFFSET
    %define FAT_SEG 0x2c0       ; FAT overwrites bootloader
    %define ROOT_SEG 0x2e0

%define NULL_DESC 0
%define CODE_DESC 0x8
%define DATA_DESC 0x10
;************************************************


;************************************************
; Multi-Boot Information Structure
;       (for passing memory info & for future
;        implementaion)
;************************************************
%include "bootinfo.inc"
%include "Memory.inc"

%include "Fat12.inc"

msgFailure db "FAILED!!!",0

msg db "DONE!!!",0

Print:
			lodsb
			or			al, al
			jz			PrintDone
			mov			ah,	0eh
			int			10h
			jmp			Print
PrintDone:
			ret


main:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax

    mov ax, 0x0
    mov ss, ax
    mov sp, 0xFFFF
    sti

    ; Install GDT
    cli
    pusha
	lgdt 	[toc]
	sti
	popa


    ; Enable A20
    cli
    pusha
    call    wait_input
    mov     al,0xAD
    out     0x64,al
    call    wait_input
    mov     al,0xD0
    out     0x64,al
    call    wait_output
    in      al,0x60
    push    eax
    call    wait_input
    mov     al,0xD1
    out     0x64,al
    call    wait_input
    pop     eax
    or      al,2
    out     0x60,al
    call    wait_input
    mov     al,0xAE
    out     0x64,al
    call    wait_input
    sti
    popa


    ; Get Memory Map
    xor		eax, eax
	xor		ebx, ebx
	call	BiosGetMemorySize64MB
	push	eax
	mov		eax, 64
	mul		ebx
	mov		ecx, eax
	pop		eax
	add		eax, ecx
	add		eax, 1024		; the routine doesnt add the KB between 0-1MB; add it
	mov		dword [boot_info+multiboot_info.memoryHi], 0
	mov		dword [boot_info+multiboot_info.memoryLo], eax
	mov		eax, 0x0
	mov		ds, ax
	mov		di, 0x1000
	call	BiosGetMemoryMap


    ; Load Root Directory Table
    call LoadRoot

    ;Load Kernel
    mov	ebx, 0
    mov	bp, IMAGE_RMODE_BASE
	mov	si, ImageName
	call	LoadFile
	mov	dword [ImageSize], ecx
	cmp	ax, 0
	je	EnterStage3
	mov	si, msgFailure
	call	Print
	mov	ah, 0
	int     0x16
	int     0x19
	cli
	hlt

EnterStage3:

    cli
    mov eax, cr0
    or eax, 1       ;Protected Mode
    mov cr0, eax

    jmp CODE_DESC:Stage3
;---------------------------------------------------------------------------------

bits 32
%include "Paging.inc"
Stage3:

	mov	ax, DATA_DESC
	mov	ds, ax
	mov	ss, ax
	mov	es, ax
	mov	esp, 90000h

	;;;;;;;;; PAGING ENABLED: 1MB MAPPED TO 4GB
	;;;;;;;;; FIRST 1MB IDENTITY MAPPED
	call EnablePaging

         mov	eax, dword [ImageSize]
         movzx	ebx, word [bpbBytesPerSector]
         mul	ebx
         mov	ebx, 4
         div	ebx
         cld
         mov    esi, IMAGE_RMODE_BASE
         mov	edi, IMAGE_PMODE_BASE
         mov	ecx, eax
         rep	movsd
	
	mov 	eax, dword[ImageSize]
	mul 	word [bpbBytesPerSector]
	mov 	edx, eax

	push 	dword boot_info
	jmp	IMAGE_PMODE_BASE

	cli
	hlt
