#include "system.h"

typedef struct 
{
	uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed)) gdt_entry_s;

typedef struct
{
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) gdt_ptr_s;

gdt_ptr_s gdt_ptr;
gdt_entry_s gdt_entry[5];
void gdt_flush();

static void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
    gdt_entry[num].base_low = (base & 0xFFFF);
    gdt_entry[num].base_middle = (base >> 16) & 0xFF;
    gdt_entry[num].base_high = (base >> 24) & 0xFF;

    gdt_entry[num].limit_low = (limit & 0xFFFF);
    gdt_entry[num].granularity = ((limit >> 16) & 0x0F);

    gdt_entry[num].granularity |= (gran & 0xF0);
    gdt_entry[num].access = access;
}

void gdt_install()
{
	gdt_ptr.limit = (sizeof(gdt_entry_s) * 5) - 1;
	gdt_ptr.base  = (uint32_t)&gdt_entry;

	gdt_set_gate(0, 0, 0, 0, 0);                // Null segment
	gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
	gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment
	gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment
	gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment

	gdt_flush();
}

