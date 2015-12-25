#include "common.h"
#include "isr.h"
#include "timer.h"



uint tick = 0;
void timer_callback(registers_t regs)
{
    tick++;


}

void init_timer(uint freq)
{

    set_int_handler(IRQ0, &timer_callback);
    uint divisor = 1193180 / freq;
    outb(0x43, 0x36);
    byte l = (byte)(divisor & 0xFF);
    byte h = (byte)((divisor>>8) & 0xFF);

    outb(0x40, l);
    outb(0x40, h);
}
