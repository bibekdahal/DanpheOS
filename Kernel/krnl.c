#include "common.h"
#include "desc_tables.h"
#include "timer.h"
#include "isr.h"

#include "mmngr.h"

typedef struct multiboot_info_struct {

	uint	m_flags;
	ulong	m_memorySize;

	uint	m_bootDevice;
	uint	m_cmdLine;
	uint	m_modsCount;
	uint	m_modsAddr;
	uint	m_syms0;
	uint	m_syms1;
	uint	m_syms2;
	uint	m_mmap_length;
	uint	m_mmap_addr;
	uint	m_drives_length;
	uint	m_drives_addr;
	uint	m_config_table;
	uint	m_bootloader_name;
	uint	m_apm_table;
	uint	m_vbe_control_info;
	uint	m_vbe_mode_info;
	ushort	m_vbe_mode;
	uint	m_vbe_interface_addr;
	ushort	m_vbe_interface_len;
} multiboot_info;

typedef struct memory_region_struct
{
	uint	startLo;	//base address
	uint	startHi;
	uint	sizeLo;		//length (in bytes)
	uint	sizeHi;
	uint	type;
	uint	acpi_3_0;
} memory_region;

char* strMemoryTypes[] =
{
	"Available",				//type 1
	"Reserved",				    //type 2
	"ACPI Reclaim",	            //type 3
	"ACPI NVS Memory"			//type 4
};

uint KernelSize;

void reboot()
{
	unsigned temp;
    asm volatile ("cli");
	do
	{
		temp = inb(0x64);
		if((temp & 0x01) != 0)
		{
			(void)inb(0x60);
			continue;
		}
	} while((temp & 0x02) != 0);
	outb(0x64, 0xFE);
	while(1)
        ; // halt if that didn't work
}


void page_fault(registers_t regs);
int kmain(multiboot_info* bootinfo)
{
    init_desc_tables();


    pm_init (bootinfo->m_memorySize, 0xC0000000 + KernelSize);

	memory_region*	region = (memory_region*)0x1000;
	int i;
	for (i=0; i<10; ++i) {
		if (region[i].type>4)
			break;
		if (i>0 && region[i].startLo==0)
			break;

        if (region[i].type==1)
            pm_init_region (region[i].startLo, region[i].sizeLo);
	}
	puts("\n\n");
	pm_deinit_region (0,KernelSize+0x10000); // from start to 4k after kernel

    set_int_handler(14, &page_fault);

    heap* kheap = (heap*) pm_alloc();

    vm_initialize();

    clrscr();

    mm_createheap(0xC0000000+0x100000, 4096, kheap);

    mm_alloc(4,kheap);
    mm_alloc(8,kheap);
    mm_alloc(20,kheap);
    displayHeap(kheap);

    asm volatile("sti");
    init_timer(100);

    init_kbd();



}



