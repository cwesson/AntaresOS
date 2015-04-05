/**
 * @file dev/keyboard.h
 * Driver for keyboard input.
 * @author Conlan Wesson
 */

#ifndef __DEV_KEYBOARD_H_
#define __DEV_KEYBOARD_H_

#include <stdint.h>

#define KEY_BUFFER_SIZE 128

#define KEY_LSHIFT_MASK 0x80    // 1000 0000
#define KEY_RSHIFT_MASK 0x08    // 0000 1000
#define KEY_LCTRL_MASK  0x40    // 0100 0000
#define KEY_RCTRL_MASK  0x04    // 0000 0100
#define KEY_LLOGO_MASK  0x20    // 0010 0000
#define KEY_RLOGO_MASK  0x02    // 0000 0010
#define KEY_LALT_MASK   0x10    // 0001 0000
#define KEY_RALT_MASK   0x01    // 0000 0001

#define KEY_NUM_MASK    0x01
#define KEY_CAPS_MASK   0x02
#define KEY_SCROLL_MASK 0x04

enum {
	KEY_ESC    = 0x1B,    //!< ASCII escape character
	KEY_DELETE = 0x7F,    //!< ASCII delete character
	
	KEY_NUM_0 = 0x80,
	KEY_NUM_1 = 0x81,
	KEY_NUM_2 = 0x82,
	KEY_NUM_3 = 0x83,
	KEY_NUM_4 = 0x84,
	KEY_NUM_5 = 0x85,
	KEY_NUM_6 = 0x86,
	KEY_NUM_7 = 0x87,
	KEY_NUM_8 = 0x88,
	KEY_NUM_9 = 0x89,
	KEY_NUM_STAR  = 0x8A,
	KEY_NUM_PLUS  = 0x8B,
	KEY_NUM_ENTER = 0x8C,
	KEY_NUM_MINUS = 0x8D,
	KEY_NUM_DOT   = 0x8E,
	KEY_NUM_SLASH = 0x8F,

	KEY_HOME   = 0x90,
	KEY_END    = 0x91,
	KEY_PGUP   = 0x92,
	KEY_PGDOWN = 0x93,
	KEY_INSERT = 0x94,
	KEY_UP     = 0x95,
	KEY_DOWN   = 0x96,
	KEY_LEFT   = 0x97,
	KEY_RIGHT  = 0x98,
	KEY_PRINT  = 0x99,
	KEY_MENU   = 0x9A,

	KEY_NUM    = 0xC1,
	KEY_CAPS   = 0xC2,
	KEY_SCROLL = 0xC4,

	KEY_RALT   = 0xD1,
	KEY_RLOGO  = 0xD2,
	KEY_RCTRL  = 0xD4,
	KEY_RSHIFT = 0xD8,
	KEY_LALT   = 0xE1,
	KEY_LLOGO  = 0xE2,
	KEY_LCTRL  = 0xE4,
	KEY_LSHIFT = 0xE8,

	KEY_F1  = 0xF1,
	KEY_F2  = 0xF2,
	KEY_F3  = 0xF3,
	KEY_F4  = 0xF4,
	KEY_F5  = 0xF5,
	KEY_F6  = 0xF6,
	KEY_F7  = 0xF7,
	KEY_F8  = 0xF8,
	KEY_F9  = 0xF9,
	KEY_F10 = 0xFA,
	KEY_F11 = 0xFB,
	KEY_F12 = 0xFC,

	KEY_BREAK = 0xFF
};

/**
 * Set the keyboard LED state.
 * @param status LED status flags.
 */
void keyboard_set_led(uint8_t);

/**
 * Gets the keyboard LED state.
 * @return The LED status flags.
 */
uint8_t keyboard_get_led();

/**
 * Clears the keyboard input buffer.
 */
void keyboard_clear_buffer();

/**
 * Gets the first key stroke in the input buffer.
 * @return The key stroke, lower 8 bits are the character, upper 8 bits are modifier flags.
 */
uint16_t keyboard_get_key();

/**
 * Initializes the keyboard input driver.
 */
void keyboard_init();

#endif /* __DEV_KEYBOARD_H_ */

