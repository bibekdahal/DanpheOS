#include "common.h"

ushort * video_memory = (ushort*) 0xB8000;
#define COLS    80
#define ROWS    25

ushort text_color = 0;

byte curX = 0;
byte curY = 0;

#define STARTLINE 7   // should have been 0, but...

static void scroll()
{
    int i;
    ushort blank = ' ' | (text_color << 8);

    for (i=STARTLINE*80;i<24*80;i++)
        video_memory[i] = video_memory[i+80];
    for (i=24*80;i<25*80;i++)
        video_memory[i] = blank;
    curY = 24;
}

void update_cursor(int x, int y)
 {
    ushort cursorLocation = y * 80 + x;
    outb(0x3D4, 14);
    outb(0x3D5, cursorLocation >> 8); // Send the high byte.
    outb(0x3D4, 15);
    outb(0x3D5, cursorLocation);      // Send the low byte.
 }
static void putc(byte ch)
{
    if (ch=='\b' && curX)
    {
        curX--;
        putc(' ');
        curX--;
    }
    else if (ch=='\t')
        curX = (curX + 8) & ~(8-1);
    else if (ch=='\r')
        curX = 0;
    else if (ch=='\n')
    {
        curX=0;
        curY++;
    }

    else if (ch>=' ') // printable characters
    {
        video_memory[curY*COLS + curX] = ch|(text_color<<8);
        curX++;
    }

    if (curX>=80)
    {
        curX=0;
        curY++;
    }

    if (curY>=25)
        scroll();
}

void putch(byte ch)
{
    putc(ch);
    update_cursor(curX, curY);
}


void puts(byte* str)
{
    if (!str) return;
    unsigned i=0;
    while (str[i]!='\0')
    {
        putc(str[i]);
        i++;
    }
    update_cursor(curX, curY);
}


void clrscr()
{
    text_color = 0x71;
    unsigned i;
    for (i=0;i<80*25;i++)
        video_memory[i] = ' ' | (text_color<<8);

    text_color = 0x74;
    curX=10; curY=2;
    puts("DANPHE OS");
    curX=10; curY=4;
    puts("---------");

    curX = 0;
    curY = STARTLINE;
    update_cursor(curX, curY);

    text_color = 0x71;
}



