#include "common.h"
#include "isr.h"


isr_t interrupt_handlers[256];

void isr_handler(registers_t regs)
{

    if (interrupt_handlers[regs.int_no] != 0)
    {
        isr_t handler = interrupt_handlers[regs.int_no];
        handler(regs);
    }
    else
    {
        puts("received interrupt:");
        puthex(regs.int_no);
        putch('\n');
    }
}


void set_int_handler(byte n, isr_t handler)
{
    interrupt_handlers[n] = handler;
}


void irq_handler(registers_t regs)
{
    // Send an EOI (end of interrupt) signal to the PICs
    if (regs.int_no >= 40)
        outb(0xA0, 0x20);    // Send reset signal to slave, if necessary
    // Send reset signal to master
    outb(0x20, 0x20);

    if (interrupt_handlers[regs.int_no] != 0)
    {
        isr_t handler = interrupt_handlers[regs.int_no];
        handler(regs);
    }
}

void page_fault(registers_t regs)
{
    clrscr();
    // A page fault has occurred.
    // The faulting address is stored in the CR2 register.
    uint faulting_address;
    asm volatile("mov %%cr2, %0" : "=r" (faulting_address));


    // Output an error message.
    puts("Page fault occured !\n Detailed Info:\n");
    puts("At 0x");
    puthex(faulting_address);
    if (!(regs.err & 0x1))
        puts ("\nCause: Page Not Present");
    if (regs.err & 0x8)
        puts("\nCause: Corrupted page entry; reserved bytes overwritten");
    if (regs.err & 0x2)
        puts ("\nDuring write operation");
    else
        puts ("\nDuring read operation");
    if (regs.err & 0x4)
        puts("\nWhile in user-mode");
    else
        puts("\nWhile in kernel-mode");
    if (regs.err & 0x8)
        puts ("\nDuring instruction fetch");

    puts("\n");
    PANIC("\n**************** PAGE FAULT **************");
}
