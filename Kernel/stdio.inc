%ifndef __STDIO_INC_67343546FDCC56AAB872_INCLUDED__
    %define __STDIO_INC_67343546FDCC56AAB872_INCLUDED__

    bits 32

    %define		VIDMEM	0xB8000
    %define		COLS	80
    %define		LINES	25
    ;%define		CHAR_ATTRIB 0x1F

    _CurX db 0
    _CurY db 0

;;;;;;;;;;;;;;;;;;;;;;
; bh = CHAR_ATTRIB
; bl = character
;;;;;;;;;;;;;;;;;;;;;;
    Putch32:

        pusha				; save registers
        mov	edi, VIDMEM		; get pointer to video memory
        xor	eax, eax		; clear eax

            mov	ecx, COLS*2
            mov	al, byte [_CurY]
            mul	ecx
            push eax

            mov	al, byte [_CurX]
            mov	cl, 2
            mul	cl
            pop	ecx
            add	eax, ecx

            xor	ecx, ecx
            add	edi, eax

        cmp	bl, 0x0A		; is it a newline character?
        je	.Row

        cmp bl, 0x08        ; backspace
        jne .bs
        cmp byte[_CurX], 0  ; not if at the start of line
        je .done
        dec byte[_CurX]     ; decrement CurX and print a space (clear out)
        mov bl, ' '
        call Putch32
        dec byte[_CurX]     ; again decrement
        jmp .done
    .bs:

        cmp bl, 0x09        ; tab
        jne .tb
        add byte[_CurX], 8  ; CurX = (CurX+8) & ~(8-1)  ;; anywhere divisible by 8
        mov eax, 7
        not eax
        and [_CurX], eax
        jmp .chkANDdone
    .tb:

        cmp bl, 0x0D        ; \r return carriage : \n enters new line for \r do CurX = 0 only
        jne .rn
        mov byte[_CurX], 0
        jmp .done
    .rn:

        mov	dl, bl
        mov	dh, bh          ;CHAR_ATTRIB
        mov	word [edi], dx
        inc	byte [_CurX]

    .chkANDdone:
    	cmp	byte [_CurX], COLS
        je	.Row
        jmp	.done
    .Row:
        mov	byte [_CurX], 0
        inc	byte [_CurY]
    .done:
        popa
        ret


;;;;;;;;;;;;;;;;;;;;;;
; ah = CHAR_ATTRIB
; ebx = pointer to string
;;;;;;;;;;;;;;;;;;;;;;
    Puts32:
        pusha
        push	ebx
        pop	edi
    .loop:
        mov	bl, byte [edi]
        cmp	bl, 0
        je	.done
        mov bh, ah
        call	Putch32
        inc	edi
        jmp	.loop
    .done:
        popa
        ret

;;;;;;;;;;;;;;;;;;;;;;
; bh = Y_OFFSET
; bl = X_OFFSET
;;;;;;;;;;;;;;;;;;;;;;
    MovCur:

        pusha
        xor	eax, eax
        mov	ecx, COLS
        mov	al, bh
        mul	ecx
        add	al, bl
        mov	ebx, eax
        mov	al, 0x0f
        mov	dx, 0x03D4
        out	dx, al
        mov	al, bl
        mov	dx, 0x03D5
        out	dx, al
        xor	eax, eax
        mov	al, 0x0e
        mov	dx, 0x03D4
        out	dx, al
        mov	al, bh
        mov	dx, 0x03D5
        out	dx, al
        popa
        ret

;;;;;;;;;;;;;;;;;;;;;;
; ah = CHAR_ATTRIB
;;;;;;;;;;;;;;;;;;;;;;
    ClrScr32:

        pusha
        cld
        mov	edi, VIDMEM
        mov	cx, 2000
        mov	al, ' '
        rep	stosw
        mov	byte [_CurX], 0
        mov	byte [_CurY], 0
        popa
        ret

;;;;;;;;;;;;;;;;;;;;;;
; ah = Y_OFFSET
; al = X_OFFSET
;;;;;;;;;;;;;;;;;;;;;;
    GotoXY:
        pusha
        mov	[_CurX], al
        mov	[_CurY], ah
        popa
        ret



%endif ;__STDIO_INC_67343546FDCC56AAB872_INCLUDED__
