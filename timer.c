#include "system.h"

int timer_ticks;

void timer_handler(registers* regs)
{
    timer_ticks++;
}

void init_timer()
{
    timer_ticks = 0;
    irq_install_handler(0, timer_handler);
}