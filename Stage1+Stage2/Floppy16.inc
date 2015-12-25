

%ifndef __FLOPPY16_INC_67343546FDCC56AAB872_INCLUDED__
    %define __FLOPPY16_INC_67343546FDCC56AAB872_INCLUDED__

    bits	16

    bpbOEM			        DB "DANPHEOS"
    bpbBytesPerSector:  	DW 512
    bpbSectorsPerCluster: 	DB 1
    bpbReservedSectors: 	DW 1
    bpbNumberOfFATs: 	    DB 2
    bpbRootEntries: 	    DW 224
    bpbTotalSectors: 	    DW 2880
    bpbMedia: 		        DB 0xf0  ;; 0xF1
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

    datasector  dw 0x0000
    cluster     dw 0x0000

    absoluteSector db 0x00
    absoluteHead   db 0x00
    absoluteTrack  db 0x00

    ;************************************************;
    ; Convert CHS to LBA
    ; LBA = (cluster - 2) * sectors per cluster
    ;************************************************;
    ClusterLBA:
              sub     ax, 0x0002
              xor     cx, cx
              mov     cl, BYTE [bpbSectorsPerCluster]
              mul     cx
              add     ax, WORD [datasector]
              ret

    ;************************************************;
    ; Convert LBA to CHS
    ; AX=>LBA Address to convert
    ;
    ; absolute sector = (logical sector / sectors per track) + 1
    ; absolute head   = (logical sector / sectors per track) MOD number of heads
    ; absolute track  = logical sector / (sectors per track * number of heads)
    ;
    ;************************************************;

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


    ;************************************************;
    ; Reads a series of sectors
    ; CX=>Number of sectors to read
    ; AX=>Starting sector
    ; ES:EBX=>Buffer to read to
    ;************************************************;

    ReadSectors:
         .MAIN:
              mov     di, 0x0005
         .SECTORLOOP:
              push    ax
              push    bx
              push    cx
              call    LBACHS
              mov     ah, 0x02
              mov     al, 0x01
              mov     ch, BYTE [absoluteTrack]
              mov     cl, BYTE [absoluteSector]
              mov     dh, BYTE [absoluteHead]
              mov     dl, BYTE [bsDriveNumber]
              int     0x13
              jnc     .SUCCESS
              xor     ax, ax
              int     0x13
              dec     di
              pop     cx
              pop     bx
              pop     ax
              jnz     .SECTORLOOP
              int     0x18
         .SUCCESS:
              pop     cx
              pop     bx
              pop     ax
              add     bx, WORD [bpbBytesPerSector]
              inc     ax
              loop    .MAIN
              ret

%endif		;__FLOPPY16_INC_67343546FDCC56AAB872_INCLUDED__
