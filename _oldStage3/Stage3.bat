@echo off
path E:\CodeBlocks\MinGW\bin


nasm -f bin Stage3.asm -o KRNL.SYS
copy KRNL.SYS A:\KRNL.SYS

del KRNL.sys

pause