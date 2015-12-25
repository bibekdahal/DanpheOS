
bits	32
;;;;****************************************
;;;; page directory table
%define		PAGE_DIR			0x9C000
;;;;****************************************
;;;; 0th page table. Address must be 4KB aligned
%define		PAGE_TABLE_0		0x9D000
;;;;****************************************
;;;; 768th page table. Address must be 4KB aligned
;;;; This table corresponds to the virtual 4 GB
%define		PAGE_TABLE_768		0x9E000
;;;;****************************************
;;;; each page table has 1024 entries
%define		PAGE_TABLE_ENTRIES	1024
;;;;****************************************
;;;; attributes (page is present;page is writable; supervisor mode)
%define		PRIV				3
;;;;****************************************



;****************************************
;	Enable Paging
;****************************************
EnablePaging:
	pusha

	;;;; idenitity map 1st page table (4MB)
	mov		eax, PAGE_TABLE_0
	mov		ebx, 0x0 | PRIV
	mov		ecx, PAGE_TABLE_ENTRIES
.loop:
	mov		dword [eax], ebx
	add		eax, 4
	add		ebx, 4096
	loop	.loop

	;------------------------------------------
	;	map the 768th table to physical addr 1MB
	;	the 768th table starts the 3gb virtual address
	;------------------------------------------

	mov		eax, PAGE_TABLE_768
	mov		ebx, 0x100000 | PRIV
	mov		ecx, PAGE_TABLE_ENTRIES
.loop2:
	mov		dword [eax], ebx
	add		eax, 4
	add		ebx, 4096
	loop	.loop2



	;;;; set up the entries in the directory table
	mov		eax, PAGE_TABLE_0 | PRIV
	mov		dword [PAGE_DIR], eax

	mov		eax, PAGE_TABLE_768 | PRIV
	mov		dword [PAGE_DIR+(768*4)], eax

	

	;;;; install directory table
	mov		eax, PAGE_DIR
	mov		cr3, eax

	;;;; enable paging
	mov		eax, cr0
	or		eax, 0x80000000
	mov		cr0, eax

	popa
	ret
