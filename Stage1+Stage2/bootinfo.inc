
struc multiboot_info
	.flags				resd	1
	.memoryLo			resd	1
	.memoryHi			resd	1
	.bootDevice			resd	1
	.cmdLine			resd	1
	.mods_count			resd	1
	.mods_addr			resd	1
	.syms0				resd	1
	.syms1				resd	1
	.syms2				resd	1
	.mmap_length		resd	1
	.mmap_addr			resd	1
	.drives_length		resd	1
	.drives_addr		resd	1
	.config_table		resd	1
	.bootloader_name	resd	1
	.apm_table			resd	1
	.vbe_control_info	resd	1
	.vbe_mode_info		resd	1
	.vbe_mode			resw	1
	.vbe_interface_seg	resw	1
	.vbe_interface_off	resw	1
	.vbe_interface_len	resw	1
endstruc


boot_info:
istruc multiboot_info
	at multiboot_info.flags,			    dd 0
	at multiboot_info.memoryLo,			    dd 0
	at multiboot_info.memoryHi,			    dd 0
	at multiboot_info.bootDevice,		    dd 0
	at multiboot_info.cmdLine,			    dd 0
	at multiboot_info.mods_count,	    	dd 0
	at multiboot_info.mods_addr,	    	dd 0
	at multiboot_info.syms0,		    	dd 0
	at multiboot_info.syms1,		    	dd 0
	at multiboot_info.syms2,		    	dd 0
	at multiboot_info.mmap_length,			dd 0
	at multiboot_info.mmap_addr,			dd 0
	at multiboot_info.drives_length,		dd 0
	at multiboot_info.drives_addr,			dd 0
	at multiboot_info.config_table,			dd 0
	at multiboot_info.bootloader_name,		dd 0
	at multiboot_info.apm_table,			dd 0
	at multiboot_info.vbe_control_info,		dd 0
	at multiboot_info.vbe_mode_info,		dw 0
	at multiboot_info.vbe_interface_seg,	dw 0
	at multiboot_info.vbe_interface_off,	dw 0
	at multiboot_info.vbe_interface_len,	dw 0
iend
