#include "system.h"

static void (*keyboard_event)(uint8_t key, uint8_t down) = 0;
void SetKeyboardEvent(void (*kbd_event)(uint8_t key, uint8_t down))
{
    keyboard_event = kbd_event;
}

void kbd_handle()
{
    if (!keyboard_event) return;

    uint8_t code, character, down;
	code = inportb(0x60);

    static const uint8_t set1_map[] =
    {
    0,  KEY_ESCAPE, '1',    '2',    '3',    '4',    '5',    '6',
    '7',    '8',    '9',    '0',    '-',    '=',    '\b',   '\t',
    'q',    'w',    'e',    'r',    't',    'y',    'u',    'i',
    'o',    'p',    '[',    ']',    '\n',   KEY_LCTRL,  'a',    's',
    'd',    'f',    'g',    'h',    'j',    'k',    'l',    ';',
    '\'',   '`',    KEY_LSHIFT, '\\',   'z',    'x',    'c',    'v',
    'b',    'n',    'm',    ',',    '.',    '/',    KEY_RSHIFT, '*',
    KEY_RALT,   ' ',    KEY_CAPS_LOCK,  KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5,
    KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_NUM_LOCK,  KEY_SCRL_LOCK,  KEY_HOME,
    KEY_UP, KEY_PGUP,'-',   KEY_LFT,'5',    KEY_RT, '+',    KEY_END,
    KEY_DN, KEY_PGDN,KEY_INS,KEY_DEL,0, 0,  0,  KEY_F11,
    KEY_F12
    };

    if(code >= 0x80)
    {
        code &= 0x7F;
        down = 0;
    }
    else
        down = 1;

    if(code >= sizeof(set1_map) / sizeof(set1_map[0]))
        return;

    keyboard_event(code, down);
}

void init_kbd()
{
    irq_install_handler(1, &kbd_handle);
}
