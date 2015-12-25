#include "system.h"

typedef struct 
{
    uint16_t base_lo;
    uint16_t sel;
    uint8_t always0;
    uint8_t flags;
    uint16_t base_hi;
} __attribute__((packed)) idt_entry_s;

typedef struct
{
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idt_ptr_s;

idt_entry_s idt_entry[256];
idt_ptr_s idt_ptr;

void idt_load();

void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags)
{
    idt_entry[num].base_lo = base & 0xFFFF;
	idt_entry[num].base_hi = (base >> 16) & 0xFFFF;

	idt_entry[num].sel     = sel;
	idt_entry[num].always0 = 0;
	idt_entry[num].flags   = flags;
}

void idt_install()
{
    idt_ptr.limit = sizeof(idt_entry_s) * 256 - 1;
    idt_ptr.base = (uint32_t)&idt_entry;	
	memset((char*)&idt_entry, 0, sizeof(idt_entry_s) * 256);
	
	isrs_install();
    irqs_install();
	idt_load();
}