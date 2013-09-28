/**
 * @file dev/keyboard.h
 * Driver for keyboard input.
 * @author Conlan Wesson
 */

#ifndef DEV_KEYBOARD_H
#define DEV_KEYBOARD_H

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

#define KEY_ESC    0x1B    //!< ASCII escape character
#define KEY_DELETE 0x7F    //!< ASCII delete character

#define KEY_NUM_0  0x80
#define KEY_NUM_1  0x81
#define KEY_NUM_2  0x82
#define KEY_NUM_3  0x83
#define KEY_NUM_4  0x84
#define KEY_NUM_5  0x85
#define KEY_NUM_6  0x86
#define KEY_NUM_7  0x87
#define KEY_NUM_8  0x88
#define KEY_NUM_9  0x89
#define KEY_NUM_STAR  0x8A
#define KEY_NUM_PLUS  0x8B
#define KEY_NUM_ENTER 0x8C
#define KEY_NUM_MINUS 0x8D
#define KEY_NUM_DOT   0x8E
#define KEY_NUM_SLASH 0x8F

#define KEY_HOME   0x90
#define KEY_END    0x91
#define KEY_PGUP   0x92
#define KEY_PGDOWN 0x93
#define KEY_INSERT 0x94
#define KEY_UP     0x95
#define KEY_DOWN   0x96
#define KEY_LEFT   0x97
#define KEY_RIGHT  0x98
#define KEY_PRINT  0x99
#define KEY_MENU   0x9A

#define KEY_NUM    0xC1
#define KEY_CAPS   0xC2
#define KEY_SCROLL 0xC4

#define KEY_RALT   0xD1
#define KEY_RLOGO  0xD2
#define KEY_RCTRL  0xD4
#define KEY_RSHIFT 0xD8
#define KEY_LALT   0xE1
#define KEY_LLOGO  0xE2
#define KEY_LCTRL  0xE4
#define KEY_LSHIFT 0xE8

#define KEY_F1     0xF1
#define KEY_F2     0xF2
#define KEY_F3     0xF3
#define KEY_F4     0xF4
#define KEY_F5     0xF5
#define KEY_F6     0xF6
#define KEY_F7     0xF7
#define KEY_F8     0xF8
#define KEY_F9     0xF9
#define KEY_F10    0xFA
#define KEY_F11    0xFB
#define KEY_F12    0xFC

#define KEY_BREAK  0xFF

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

#endif
