@echo off

nasm -f bin Stage2.asm -o Stage2.SYS
copy Stage2.SYS A:\STAGE2.SYS

del Stage2.SYS
pause
