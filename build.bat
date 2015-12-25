@echo off
path D:\OS Proj\MinGW\bin

nasm -f elf -o start.o start.asm
gcc -ffreestanding -c kernel.c -o kernel.o
gcc -ffreestanding -c common.c -o common.o
gcc -ffreestanding -c system.c -o system.o
gcc -ffreestanding -c gdt.c -o gdt.o
gcc -ffreestanding -c idt.c -o idt.o
gcc -ffreestanding -c isr.c -o isr.o
gcc -ffreestanding -c irq.c -o irq.o
gcc -ffreestanding -c timer.c -o timer.o
gcc -ffreestanding -c keyboard.c -o keyboard.o
gcc -ffreestanding -c physmem.c -o physmem.o
gcc -ffreestanding -c virtmem.c -o virtmem.o

ld -T link.ld -o kernel.tmp start.o kernel.o common.o system.o gdt.o idt.o isr.o irq.o timer.o keyboard.o physmem.o virtmem.o
objcopy -R .note -R .comment -S -O binary kernel.tmp kernel.bin

REM copy kernel.bin A:\kernel.bin

del start.o
del kernel.o
del common.o
del system.o
del gdt.o
del idt.o
del isr.o
del irq.o
del timer.o
del keyboard.o
del physmem.o
del virtmem.o

del kernel.tmp
del kernel.bin
pause