#include "common.h"
#include "keyboard.h"

void scroll();
void move_cursor();
void cls();
void putch(unsigned char c);
void puts(unsigned char *text);
void putdec(uint32_t n);
void puthex(uint32_t n);
void settextcolor(uint8_t forecolor, uint8_t backcolor);

void irq_install_handler(int irq, void (*handler)(registers* regs));
void irq_uninstall_handler(int irq);
void isr_install_handler(int isr, void (*handler)(registers* regs));
void isr_uninstall_handler(int isr);

void* pm_alloc_frame();
void pm_free_frame(void* addr);
