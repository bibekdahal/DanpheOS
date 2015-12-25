#include "common.h"
#include "desc_tables.h"
#include "isr.h"

extern void gdt_flush(uint);
extern void idt_flush(uint);


#define GDT_ENTRIES_NUM 5
gdt_entry_t gdt[GDT_ENTRIES_NUM];
gdt_ptr_t   gdt_ptr;

idt_entry_t idt[256];
idt_ptr_t   idt_ptr;


void init_gdt();
void gdt_set_desc(int, uint, uint, byte, byte);
void init_idt();
void idt_set_ir(byte, uint, ushort, byte);


extern isr_t interrupt_handlers[];

void init_desc_tables()
{
    init_gdt();
    init_idt();
    memset((void*)&interrupt_handlers,0,sizeof(isr_t)*256);
}

void init_gdt()
{
    gdt_ptr.limit = (sizeof(gdt_entry_t) * GDT_ENTRIES_NUM) - 1;
    gdt_ptr.base = (uint)&gdt[0];

    gdt_set_desc(0, 0,0,0,0);                   //NULL DESC
    gdt_set_desc(1, 0,0xFFFFFFFF,0x9A,0xCF);    //DEFAULT CODE DESC
    gdt_set_desc(2, 0,0xFFFFFFFF,0x92,0xCF);    //DEFAULT DATA DESC
    gdt_set_desc(3, 0,0xFFFFFFFF,0xFA,0xCF);    //USER MODE CODE DESC
    gdt_set_desc(4, 0,0xFFFFFFFF,0xF2,0xCF);    //USER MODE DATA DESC
    gdt_flush((uint)&gdt_ptr);
}

void gdt_set_desc(int num, uint base, uint limit, byte access, byte gran)
{
    if (num >= GDT_ENTRIES_NUM) return;

    gdt[num].base_low = base & 0xFFFF;
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;

    gdt[num].limit_low = limit & 0xFFFF;
    gdt[num].granularity = (limit >> 16) & 0x0F;
    gdt[num].granularity |= gran & 0xF0;
    gdt[num].access = access;
}

void init_idt()
{
    idt_ptr.limit = sizeof(idt_entry_t) * 256 - 1;
    idt_ptr.base = (uint)&idt;

    memset((void*)&idt,0,sizeof(idt_entry_t)*256);

    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);
    outb(0xA1, 0x0);

    idt_set_ir(0,(uint)isr0,0x08,0x8E);
    idt_set_ir(1,(uint)isr1,0x08,0x8E);
    idt_set_ir(2,(uint)isr2,0x08,0x8E);
    idt_set_ir(3,(uint)isr3,0x08,0x8E);
    idt_set_ir(4,(uint)isr4,0x08,0x8E);
    idt_set_ir(5,(uint)isr5,0x08,0x8E);
    idt_set_ir(6,(uint)isr6,0x08,0x8E);
    idt_set_ir(7,(uint)isr7,0x08,0x8E);
    idt_set_ir(8,(uint)isr8,0x08,0x8E);
    idt_set_ir(9,(uint)isr9,0x08,0x8E);
    idt_set_ir(10,(uint)isr10,0x08,0x8E);
    idt_set_ir(11,(uint)isr11,0x08,0x8E);
    idt_set_ir(12,(uint)isr12,0x08,0x8E);
    idt_set_ir(13,(uint)isr13,0x08,0x8E);
    idt_set_ir(14,(uint)isr14,0x08,0x8E);
    idt_set_ir(15,(uint)isr15,0x08,0x8E);
    idt_set_ir(16,(uint)isr16,0x08,0x8E);
    idt_set_ir(17,(uint)isr17,0x08,0x8E);
    idt_set_ir(18,(uint)isr18,0x08,0x8E);
    idt_set_ir(19,(uint)isr19,0x08,0x8E);
    idt_set_ir(20,(uint)isr20,0x08,0x8E);
    idt_set_ir(21,(uint)isr21,0x08,0x8E);
    idt_set_ir(22,(uint)isr22,0x08,0x8E);
    idt_set_ir(23,(uint)isr23,0x08,0x8E);
    idt_set_ir(24,(uint)isr24,0x08,0x8E);
    idt_set_ir(25,(uint)isr25,0x08,0x8E);
    idt_set_ir(26,(uint)isr26,0x08,0x8E);
    idt_set_ir(27,(uint)isr27,0x08,0x8E);
    idt_set_ir(28,(uint)isr28,0x08,0x8E);
    idt_set_ir(29,(uint)isr29,0x08,0x8E);
    idt_set_ir(30,(uint)isr30,0x08,0x8E);
    idt_set_ir(31,(uint)isr31,0x08,0x8E);

    idt_set_ir(32,(uint)irq0,0x08,0x8E);
    idt_set_ir(33,(uint)irq1,0x08,0x8E);
    idt_set_ir(34,(uint)irq2,0x08,0x8E);
    idt_set_ir(35,(uint)irq3,0x08,0x8E);
    idt_set_ir(36,(uint)irq4,0x08,0x8E);
    idt_set_ir(37,(uint)irq5,0x08,0x8E);
    idt_set_ir(38,(uint)irq6,0x08,0x8E);
    idt_set_ir(39,(uint)irq7,0x08,0x8E);
    idt_set_ir(40,(uint)irq8,0x08,0x8E);
    idt_set_ir(41,(uint)irq9,0x08,0x8E);
    idt_set_ir(42,(uint)irq10,0x08,0x8E);
    idt_set_ir(43,(uint)irq11,0x08,0x8E);
    idt_set_ir(44,(uint)irq12,0x08,0x8E);
    idt_set_ir(45,(uint)irq13,0x08,0x8E);
    idt_set_ir(46,(uint)irq14,0x08,0x8E);
    idt_set_ir(47,(uint)irq15,0x08,0x8E);

    idt_flush((uint)&idt_ptr);

}

void idt_set_ir(byte num, uint base, ushort sel, byte flags)
{
    idt[num].base_lo = base & 0xFFFF;
    idt[num].base_hi = (base >> 16) & 0xFFFF;

    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
}

