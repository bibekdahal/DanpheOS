#include <stdint.h>

unsigned char *memcpy(unsigned char *dest, const unsigned char *src, unsigned int count);
unsigned char *memset(unsigned char *dest, unsigned char val, unsigned int count);
unsigned short *memsetw(unsigned short *dest, unsigned short val, unsigned int count);
unsigned int strlen(const char *str);
unsigned char inportb (unsigned short _port);
void outportb (unsigned short _port, unsigned char _data);


typedef struct
{
	unsigned int gs, fs, es, ds;
	unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
	unsigned int int_no, err_code;
	unsigned int eip, cs, eflags, useresp, ss;
} __attribute__((packed)) registers;

#define TRUE	1
#define FALSE	0
typedef int 	BOOL;
	