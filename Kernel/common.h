#ifndef COMMON_H
#define COMMON_H


typedef unsigned long int   ulong;
typedef unsigned int        uint;
typedef unsigned short      ushort;
typedef unsigned char       byte;



void pause(ulong ms);
void memset(void *dest, byte val, uint len);
void memcpy(void *dest, const byte *src, uint len);

void puthex(uint n);
void putdec(uint n);
void putbin(uint n);
void putbinall(uint n);
void PANIC(char* str);

void outb(ushort port, byte value);
byte inb(ushort port);
ushort inw(ushort port);


#endif
