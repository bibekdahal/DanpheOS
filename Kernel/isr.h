
typedef struct registers
{
    uint ds;                //  "mov ax, ds"   -   "push eax"
    uint edi, esi, ebp, esp, ebx, edx, ecx, eax;         //  "pusha"
    uint int_no, err;       //  err: either pushed by default or "push byte 0", int_no: "push %1" in macro
    uint eip, cs, eflags, useresp, ss;      // pushed by processor automatically
}   registers_t;


#define IRQ0 32
#define IRQ1 33
#define IRQ2 34
#define IRQ3 35
#define IRQ4 36
#define IRQ5 37
#define IRQ6 38
#define IRQ7 39
#define IRQ8 40
#define IRQ9 41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47

typedef void (*isr_t)(registers_t);
void set_int_handler(byte n, isr_t handler);
