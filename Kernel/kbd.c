#include "common.h"
#include "isr.h"


/* function keys: */
#define	KEY_F1		0x80
#define	KEY_F2		(KEY_F1 + 1)
#define	KEY_F3		(KEY_F2 + 1)
#define	KEY_F4		(KEY_F3 + 1)
#define	KEY_F5		(KEY_F4 + 1)
#define	KEY_F6		(KEY_F5 + 1)
#define	KEY_F7		(KEY_F6 + 1)
#define	KEY_F8		(KEY_F7 + 1)
#define	KEY_F9		(KEY_F8 + 1)
#define	KEY_F10		(KEY_F9 + 1)
#define	KEY_F11		(KEY_F10 + 1)
#define	KEY_F12		(KEY_F11 + 1)
/* cursor keys */
#define	KEY_INS		(KEY_F12 + 1)
#define	KEY_DEL		(KEY_INS + 1)
#define	KEY_HOME	(KEY_DEL + 1)
#define	KEY_END		(KEY_HOME + 1)
#define	KEY_PGUP	(KEY_END + 1)
#define	KEY_PGDN	(KEY_PGUP + 1)
#define	KEY_LFT		(KEY_PGDN + 1)
#define	KEY_UP		(KEY_LFT + 1)
#define	KEY_DN		(KEY_UP + 1)
#define	KEY_RT		(KEY_DN + 1)

#define KEY_ESCAPE  (KEY_RT + 1)

#define KEY_SCRL_LOCK   (KEY_ESCAPE + 1)
#define KEY_NUM_LOCK    (KEY_SCRL_LOCK + 1)
#define KEY_CAPS_LOCK   (KEY_NUM_LOCK + 1)

#define KEY_LSHIFT  (KEY_CAPS_LOCK + 1)
#define KEY_RSHIFT  (KEY_LSHIFT + 1)
#define KEY_LALT    (KEY_RSHIFT + 1)
#define KEY_RALT    KEY_LALT
#define KEY_LCTRL   (KEY_RALT + 1)
#define KEY_RCTRL   KEY_LCTRL


#define KEY_BREAK   0x0
#define KEY_MAKE    0x1

byte KEYS[256];
static byte convert(uint code)
{
	static const byte set1_map[] =
	{
    0,	KEY_ESCAPE,	'1',	'2',	'3',	'4',	'5',	'6',
    '7',	'8',	'9',	'0',	'-',	'=',	'\b',	'\t',
    'q',	'w',	'e',	'r',	't',	'y',	'u',	'i',
    'o',	'p',	'[',	']',	'\n',	KEY_LCTRL,	'a',	's',
    'd',	'f',	'g',	'h',	'j',	'k',	'l',	';',
    '\'',	'`',	KEY_LSHIFT,	'\\',	'z',	'x',	'c',	'v',
    'b',	'n',	'm',	',',	'.',	'/',	KEY_RSHIFT,	'*',
    KEY_RALT,	' ',	KEY_CAPS_LOCK,	KEY_F1,	KEY_F2,	KEY_F3,	KEY_F4,	KEY_F5,
    KEY_F6,	KEY_F7,	KEY_F8,	KEY_F9,	KEY_F10, KEY_NUM_LOCK,	KEY_SCRL_LOCK,	KEY_HOME,
    KEY_UP,	KEY_PGUP,'-',	KEY_LFT,'5',	KEY_RT,	'+',	KEY_END,
    KEY_DN,	KEY_PGDN,KEY_INS,KEY_DEL,0,	0,	0,	KEY_F11,
    KEY_F12
	};

    byte key = code, type;
	if(code >= 0x80)
    {
        key &= 0x7F;
        type = KEY_BREAK;
    }
    else
        type = KEY_MAKE;

	if(key >= sizeof(set1_map) / sizeof(set1_map[0]))
		return 0;

    KEYS[set1_map[key]] = type;
	return set1_map[key];
}
void kbd_handle()
{
    byte scan_code, character;
	scan_code = inb(0x60);
	character = convert(scan_code);

	static const byte capNums[] = ")!@#$%^&*()";

	if (KEYS[character])
	{
	    unsigned shift=0;
	    if (KEYS[KEY_LSHIFT] || KEYS[KEY_RSHIFT])
            shift=1;
	    if (character==KEY_ESCAPE)
            clrscr();//reboot();

        if(character >= 'a' && character <= 'z')
        {
            if (shift)
                putch(character - ('a'-'A'));
            else
                putch(character);
        }
        else if (character >= '0' && character <= '9' && shift)
            putch(capNums[character-'0']);
        else if (shift)
        {
            byte ch;
            switch (character)
            {
                case '-': ch='_'; break;
                case '=': ch='+'; break;
                case '`': ch='~'; break;
                case '[': ch='{'; break;
                case ']': ch='}'; break;
                case ';': ch=':'; break;
                case '\'': ch='"'; break;
                case ',': ch='<'; break;
                case '.': ch='>'; break;
                case '/': ch='?'; break;
            default:
                    ch=character;
            }
            if (character < 0x80)
                putch(ch);
        }
        else if (character < 0x80)
            putch(character);
	}
}

void init_kbd()
{
    set_int_handler(IRQ1, &kbd_handle);
    unsigned i;
    for (i=0;i<256;i++)
        KEYS[i]=KEY_BREAK;
}
