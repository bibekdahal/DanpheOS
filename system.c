#include "system.h"


uint16_t *textmemptr = (uint16_t *)(0xB8000 + 0xC0000000);
int attrib = 0x0F;
int csr_x = 0, csr_y = 0;

void scroll()
{
    unsigned blank, temp;
    blank = 0x20 | (attrib << 8);
    if(csr_y >= 25)
    {
        temp = csr_y - 25 + 1;
        memcpy((void*)textmemptr, (void*)(textmemptr + temp * 80), (25 - temp) * 80 * 2);
        memsetw(textmemptr + (25 - temp) * 80, blank, 80);
        csr_y = 25 - 1;
    }
}

void move_cursor()
{
    unsigned temp;
    temp = csr_y * 80 + csr_x;
    outportb(0x3D4, 14);
    outportb(0x3D5, temp >> 8);
    outportb(0x3D4, 15);
    outportb(0x3D5, temp);
}

void cls()
{
    unsigned blank;
    int i;
    blank = 0x20 | (attrib << 8);

    for(i = 0; i < 25; i++)
        memsetw(textmemptr + i * 80, blank, 80);
    
    csr_x = 0;
    csr_y = 0;
    move_cursor();
}

void putch(unsigned char c)
{
    uint16_t *where;
    unsigned att = attrib << 8;

    if(c == 0x08)
    {   
        if (csr_x == 0)
        {
            if (csr_y == 0) return;
            csr_x = 79;
            csr_y--;
        }
    	else
            csr_x--;
        where = textmemptr + (csr_y * 80 + csr_x);
        *where = ' ' | att;
    }
    else if(c == '\t')
        csr_x = (csr_x + 8) & ~(8 - 1);
    else if(c == '\r')
        csr_x = 0;
    else if(c == '\n')
    {
        csr_x = 0;
        csr_y++;
    }
    else if(c >= ' ')
    {
        where = textmemptr + (csr_y * 80 + csr_x);
        *where = c | att;	
        csr_x++;
    }

    if(csr_x >= 80)
    {
        csr_x = 0;
        csr_y++;
    }

    scroll();
    move_cursor();
}

void puts(unsigned char *text)
{
    int i;
    for (i = 0; i < strlen(text); i++)
        putch(text[i]);
}

void putdec(uint32_t n)
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

void puthex(uint32_t n)
{
    int tmp;
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

void settextcolor(uint8_t forecolor, uint8_t backcolor)
{
    attrib = (backcolor << 4) | (forecolor & 0x0F);
}
