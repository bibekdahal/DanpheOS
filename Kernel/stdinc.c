#include "common.h"

void outb(ushort port, byte value)
{
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

byte inb(ushort port)
{
    byte ret;
    asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

ushort inw(ushort port)
{
    ushort ret;
    asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}


void putbinall(uint n)
{
    int i;
    int dec=0;
    for (i=31;i>=0;i--)
    {
        if ((n>>i) & 0x1 == 0x1) putch('1');
        else putch('0');
    }
}
void putbin(uint n)
{
    int i;
    int dec=0;
    for (i=31;i>=0;i--)
    {
        if ((n>>i) & 0x1 == 0x1) dec=dec*10 + 1;
        else dec=dec*10;
    }
    putdec(dec);
}
void puthex(uint n)
{
    int tmp;
    //puts("0x");
    char noZeroes = 1;
    int i;
    for (i = 28; i > 0; i -= 4)
    {
        tmp = (n >> i) & 0xF;
        if (tmp == 0 && noZeroes != 0)
            continue;

        if (tmp >= 0xA)
        {
            noZeroes = 0;
            putch(tmp-0xA+'a' );
        }
        else
        {
            noZeroes = 0;
            putch( tmp+'0' );
        }
    }

    tmp = n & 0xF;
    if (tmp >= 0xA)
        putch(tmp-0xA+'a');
    else
        putch(tmp+'0');

}

void putdec(uint n)
{
    if (n == 0)
    {
        putch('0');
        return;
    }

    int acc = n;
    char c[32];
    int i = 0;
    while (acc > 0)
    {
        c[i] = '0' + acc%10;
        acc /= 10;
        i++;
    }
    c[i] = 0;

    char c2[32];
    c2[i--] = 0;
    int j = 0;
    while(i >= 0)
    {
        c2[i--] = c[j++];
    }
    puts(c2);
}

void pause(ulong ms)
{
    ulong i;
    for (i=0;i<ms*1000;i++);
}

void memset(void *dest, byte val, uint len)
{
    byte *temp = (byte *)dest;
    for ( ; len != 0; len--) *temp++ = val;
}

void memcpy(void *dest, const byte *src, uint len)
{
    const byte *sp = (const byte *)src;
    byte *dp = (byte *)dest;
    for(; len != 0; len--) *dp++ = *sp++;
}

void PANIC(char* str)
{
    asm volatile("cli");
    puts(str);
    for (;;);
}
