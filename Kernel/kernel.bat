@echo off
path D:\OS Proj\MinGW\bin

nasm -f elf -o KRNL.o krnl.asm

gcc -ffreestanding -c krnl.c -o KRNLC.o
gcc -ffreestanding -c stdinc.c -o STDC.o
gcc -ffreestanding -c isr.c -o ISRC.o
gcc -ffreestanding -c desc_tables.c -o DESCT.o
gcc -ffreestanding -c timer.c -o TIMERC.o
gcc -ffreestanding -c pmmngr.c -o PMMNGRC.o
gcc -ffreestanding -c vmmngr.c -o VMMNGRC.o
gcc -ffreestanding -c kbd.c -o KBD.o
gcc -ffreestanding -c display.c -o DISPLAY.o
gcc -ffreestanding -c mmngr.c -o MMNGR.o
 
ld -T link.ld -o KRNL.tmp KRNL.o KRNLC.o STDC.o ISRC.o DESCT.o TIMERC.o PMMNGRC.o VMMNGRC.o KBD.o DISPLAY.o MMNGR.o
objcopy -R .note -R .comment -S -O binary KRNL.tmp KRNL.SYS


copy KRNL.SYS A:\KRNL.SYS

del KRNLC.o
del STDC.o
del ISRC.o
del DESCT.o
del TIMERC.o
del PMMNGRC.o
del VMMNGRC.o
del KBD.o
del DISPLAY.o
del MMNGR.o

del KRNL.o
del KRNL.tmp
del KRNL.SYS

pause
