
typedef struct gdt_entry_struct
{
    ushort  limit_low;
    ushort  base_low;
    byte    base_middle;
    byte    access;
    byte    granularity;
    byte    base_high;
} __attribute__((packed)) gdt_entry_t;

typedef struct gdt_ptr_struct
{
    ushort  limit;
    uint    base;
} __attribute__((packed)) gdt_ptr_t;


typedef struct idt_entry_struct
{
    ushort  base_lo;
    ushort  sel;
    byte    always0;
    byte    flags;
    ushort  base_hi;
} __attribute__((packed)) idt_entry_t;

typedef struct idt_ptr_struct
{
    ushort  limit;
    uint    base;
} __attribute__((packed)) idt_ptr_t;

void init_desc_tables();



/* ********************************************
 inetrupt service routines from ASM
 ******************************************** */
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

extern void irq0 ();
extern void irq1 ();
extern void irq2 ();
extern void irq3 ();
extern void irq4 ();
extern void irq5 ();
extern void irq6 ();
extern void irq7 ();
extern void irq8 ();
extern void irq9 ();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();
/* ******************************************** */


