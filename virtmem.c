#include "system.h"

uint32_t* kernel_pagedir = (uint32_t*)0xFFFFF000;	// virtual address of page directory

// for when memory is allocated for page dir/table later but physical address is needed
static void * get_physaddr(void * virtualaddr)
{
    uint32_t pdindex = (uint32_t)virtualaddr >> 22;
    uint32_t ptindex = (uint32_t)virtualaddr >> 12 & 0x03FF;
 	
 	if (!(kernel_pagedir[pdindex] & 1)) return 0;
    uint32_t * pt = ((uint32_t *)0xFFC00000) + (0x400 * pdindex);
    if (!(pt[ptindex] & 1)) return 0;
 
    return (void *)((pt[ptindex] & ~0xFFF) + ((uint32_t)virtualaddr & 0xFFF));
}

static void create_kernel_directory()
{
	uint32_t * pdir = (uint32_t*) pm_alloc_frame();
	int i;
	for (i=0; i<1023; i++)
		pdir[i] = 0|2;

	uint32_t * ptable = (uint32_t*) pm_alloc_frame();
	uint32_t address = 0; 
	for(i = 0; i < 1024; i++)
	{
	    ptable[i] = address | 3;
	    address = address + 4096;
	}

	//pdir[0] = (uint32_t)ptable | 3;					// identity map first 4 megabytes
	pdir[(0xC0000000>>22)] = (uint32_t)ptable | 3;		// Higher Half Kernel
	pdir[1023] = (uint32_t)pdir | 3;					// last entry points to page-directory itself

	asm volatile("mov %0, %%cr3":: "b"(pdir));
}

static inline void enable_paging()
{
	uint32_t cr0;
	asm volatile("mov %%cr0, %0": "=b"(cr0));
	cr0 |= 0x80000000;
	asm volatile("mov %0, %%cr0":: "b"(cr0));
}

static inline void flush_tlb(uint32_t addr)
{
    asm volatile("invlpg (%0)" ::"r" (addr) : "memory");
}

void page_fault_handler(registers* regs)
{
    puts("Page Fault\n");
    while(1)
        ;
}

void vm_initialize()
{
	create_kernel_directory();
	enable_paging();
    isr_install_handler(14, &page_fault_handler);
}

BOOL vm_map_page(uint32_t phys, uint32_t virt)
{
    int i;
    uint32_t pdindex = (uint32_t)virt >> 22;
    uint32_t ptindex = (uint32_t)virt >> 12 & 0x03FF;

    uint32_t* ptable = (uint32_t *)(0xFFC00000 + pdindex * 0x1000);
    if (kernel_pagedir[pdindex] & 1)
    {
    	if (ptable[ptindex] & 1)
    		return FALSE;
    	ptable[ptindex] = phys | 3;
        flush_tlb(virt);
    	return TRUE;
    }

    uint32_t ptable_phys = (uint32_t) pm_alloc_frame();
    if (!ptable_phys) return FALSE;
    kernel_pagedir[pdindex] = ptable_phys | 3;
	for (i=0; i<1024; i++)
		ptable[i] = 0|2;
	ptable[ptindex] = phys | 3;
    flush_tlb(virt);
	return TRUE;
}

void vm_unmap_page(uint32_t virt)
{
	int i;
	uint32_t pdindex = (uint32_t)virt >> 22;
    uint32_t ptindex = (uint32_t)virt >> 12 & 0x03FF;

    uint32_t* ptable = (uint32_t *)(0xFFC00000 + pdindex * 0x1000);
    if (kernel_pagedir[pdindex] & 1)
    {
    	if (ptable[ptindex] & 1)
    		ptable[ptindex] = 0 | 2;
    	
    	// remove the table if empty
    	for (i=0; i<1024; i++)
    		if (ptable[ptindex] & 1) break;
    	if (i == 1024)
    	{
    		pm_free_frame((void*)(kernel_pagedir[pdindex] & 0xFFFFF000));
    		kernel_pagedir[pdindex] = 0 | 2;
    	}

        flush_tlb(virt);
    }
}
