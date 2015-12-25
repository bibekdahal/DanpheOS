

bits	16
org		0


start:          jmp main

bpbOEM			        DB "DANPHEOS"
bpbBytesPerSector:  	DW 512      ; <--- should be at 11 bytes from first
bpbSectorsPerCluster: 	DB 1
bpbReservedSectors: 	DW 1
bpbNumberOfFATs: 	    DB 2
bpbRootEntries: 	    DW 224
bpbTotalSectors: 	    DW 2880
bpbMedia: 		        DB 0xf0
bpbSectorsPerFAT: 	    DW 9
bpbSectorsPerTrack: 	DW 18
bpbHeadsPerCylinder: 	DW 2
bpbHiddenSectors: 	    DD 0
bpbTotalSectorsBig:     DD 0
bsDriveNumber: 	        DB 0
bsUnused: 		        DB 0
bsExtBootSignature: 	DB 0x29
bsSerialNumber:	        DD 0xa0a1a2a3
bsVolumeLabel: 	        DB "MOS FLOPPY "
bsFileSystem: 	        DB "FAT12   "



Print:
			lodsb
			or			al, al
			jz			PrintDone
			mov			ah,	0eh
			int			10h
			jmp			Print
PrintDone:
			ret


; LBA = (cluster - 2) * sectors per cluster
ClusterLBA:
          sub     ax, 0x0002
          xor     cx, cx
          mov     cl, BYTE [bpbSectorsPerCluster]
          mul     cx
          add     ax, WORD [datasector]
          ret
LBACHS:
          xor     dx, dx
          div     WORD [bpbSectorsPerTrack]
          inc     dl
          mov     BYTE [absoluteSector], dl
          xor     dx, dx
          div     WORD [bpbHeadsPerCylinder]
          mov     BYTE [absoluteHead], dl
          mov     BYTE [absoluteTrack], al
          ret

ReadSectors:
     .MAIN:
          mov     di, 0x0005                          ; five retries for error
     .SECTORLOOP:
          push    ax
          push    bx
          push    cx
          call    LBACHS
          mov     ah, 0x02                            ; BIOS read sector
          mov     al, 0x01                            ; read one sector
          mov     ch, BYTE [absoluteTrack]            ; track
          mov     cl, BYTE [absoluteSector]           ; sector
          mov     dh, BYTE [absoluteHead]             ; head
          mov     dl, BYTE [bsDriveNumber]            ; drive
          int     0x13
          jnc     .SUCCESS
          xor     ax, ax                              ; BIOS reset disk
          int     0x13
          dec     di
          pop     cx
          pop     bx
          pop     ax
          jnz     .SECTORLOOP                         ; attempt to read again
          int     0x18
     .SUCCESS:
          pop     cx
          pop     bx
          pop     ax
          add     bx, WORD [bpbBytesPerSector]        ; queue next buffer
          inc     ax                                  ; queue next sector
          loop    .MAIN                               ; read next sector
          ret
main:
    cli
    mov ax, 0x07C0
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ax, 0x0000
    mov ss, ax
    mov sp, 0xFFFF
    sti

	mov	si, msg
	call Print

; LOAD ROOT DIRECTORY
    xor     cx, cx
    xor     dx, dx
    mov     ax, 0x0020
    mul     WORD [bpbRootEntries]
    div     WORD [bpbBytesPerSector]
    xchg    ax, cx
    mov     al, BYTE [bpbNumberOfFATs]
    mul     WORD [bpbSectorsPerFAT]
    add     ax, WORD [bpbReservedSectors]
    mov     WORD [datasector], ax
    add     WORD [datasector], cx
    mov     bx, 0x0200                      ; read root directory into memory (7C00:0200)
    call    ReadSectors
    ; browse root directory for binary image
    mov     cx, WORD [bpbRootEntries]
    mov     di, 0x0200
.LOOP:
    push    cx
    mov     cx, 0x000B
    mov     si, ImageName
    push    di
rep cmpsb
    pop     di
    je      LOAD_FAT
    pop     cx
    add     di, 0x0020
    loop    .LOOP
    jmp     FAILURE

LOAD_FAT:
    mov     dx, WORD [di + 0x001A]
    mov     WORD [cluster], dx
    xor     ax, ax
    mov     al, BYTE [bpbNumberOfFATs]
    mul     WORD [bpbSectorsPerFAT]
    mov     cx, ax
    mov     ax, WORD [bpbReservedSectors]
    mov     bx, 0x0200                          ; read FAT into memory (7C00:0200)
    call    ReadSectors

    ; read image file into memory (0050:0000)
    mov     ax, 0x0050
    mov     es, ax
    mov     bx, 0x0000
    push    bx

LOAD_IMAGE:
    mov     ax, WORD [cluster]
    pop     bx
    call    ClusterLBA
    xor     cx, cx
    mov     cl, BYTE [bpbSectorsPerCluster]
    call    ReadSectors
    push    bx

        mov     ax, WORD [cluster]
        mov     cx, ax
        mov     dx, ax
        shr     dx, 0x0001
        add     cx, dx
        mov     bx, 0x0200
        add     bx, cx
        mov     dx, WORD [bx]
        test    ax, 0x0001
        jnz     .ODD_CLUSTER
    .EVEN_CLUSTER:
        and     dx, 0000111111111111b
        jmp     .DONE
    .ODD_CLUSTER:
        shr     dx, 0x0004
    .DONE:
        mov     WORD [cluster], dx
        cmp     dx, 0x0FF0
        jb      LOAD_IMAGE

DONE:
    push    WORD 0x0050     ; CS
    push    WORD 0x0000     ; IP
    retf                    ;pop IP, pop CS
FAILURE:
    mov     si, msgFailure
    call    Print
    mov     ah, 0x00
    int     0x16                                ; await keypress
    int     0x19                                ; warm boot computer


absoluteSector db 0x00
absoluteHead   db 0x00
absoluteTrack  db 0x00
datasector  dw 0x0000
cluster     dw 0x0000

ImageName db "STAGE2  SYS"              ;exactly 11 bytes, no NULL

msg	db	"Loading OS...", 0x0D, 0x0A, 0x00
msgFailure db "OS loading failed...", 0x0D, 0x0A, 0x00

times 510 - ($-$$) db 0

dw 0xAA55
