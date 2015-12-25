#include "system.h"
#include "multiboot.h"

void init_phys_memory(multiboot_info_t* mbt);
void kbd_event(uint8_t key, uint8_t down);

void kmain(multiboot_info_t* mbt, uint32_t magic)	
{
	gdt_install();
	idt_install();
	asm volatile("sti");	// without this line irqs are disabled

	init_timer();
	cls();

	int i;
	putch('\n');
	for (i=0; i<36; ++i) putch(' ');
	puts("DANPHE OS\n");
	for (i=0; i<34; ++i) putch(' ');
	for (i=0; i<13; ++i) putch('_');
	puts("\n\n");


	init_phys_memory(mbt);
	vm_initialize();

	uint32_t phys = (uint32_t)pm_alloc_frame();
	if (!vm_map_page(phys, 0x22222222))
		puts("Couldn't map");
	int * a = (int*)0x22222222;
	int b=*a;

	init_kbd();
	SetKeyboardEvent(&kbd_event);

	while(1) ;		
}










/////////////////////////////////////////////////////

uint8_t KEYS[256];
void kbd_event(uint8_t key, uint8_t down)
{
	KEYS[key] = down;

	static const uint8_t capNums[] = ")!@#$%^&*()";	
	if (KEYS[key])
	{
	    char shift=0;
	    if (KEYS[KEY_LSHIFT] || KEYS[KEY_RSHIFT])
            shift=1;
	    if (key==KEY_ESCAPE)
            cls();

        if(key >= 'a' && key <= 'z')
        {
            if (shift)
                putch(key - ('a'-'A'));
            else
                putch(key);
        }
        else if (key >= '0' && key <= '9' && shift)
            putch(capNums[key-'0']);
        else if (shift)
        {
            uint8_t ch;
            switch (key)
            {
                case '-': ch='_'; break;
                case '=': ch='+'; break;
                case '`': ch='~'; break;
                case '[': ch='{'; break;
                case ']': ch='}'; break;
                case ';': ch=':'; break;
                case '\'': ch='"'; break;
                case ',': ch='<'; break;
                case '.': ch='>'; break;
                case '/': ch='?'; break;
            default:
                    ch=key;
            }
            putch(ch);
        }
        else
            putch(key);
	}
}


/////////////////////////////////////////////////////

extern uint32_t kernel_end;
void init_phys_memory(multiboot_info_t* mbt)
{
	uint32_t memsize = mbt->mem_upper + 0x400;				// 1MB + upper memory size (in KB)
	pm_init(memsize, kernel_end + 0xC0000000);

	memory_map_t* mmap = (memory_map_t*)mbt->mmap_addr;
	while((uint32_t)mmap < mbt->mmap_addr + mbt->mmap_length) 
	{
		if (mmap->type == 1)
			pm_init_region(mmap->base_addr_low, mmap->length_low);
		mmap = (memory_map_t*) ((uint32_t)mmap + mmap->size + sizeof(uint32_t));
	}
	pm_deinit_region(0, kernel_end+4096);
}

