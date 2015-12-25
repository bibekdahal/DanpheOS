

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

void SetKeyboardEvent(void (*kbd_event)(uint8_t key, uint8_t down));