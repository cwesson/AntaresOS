/**
 * @file dev/keyboard.c
 * Driver for keyboard input.
 * @author Conlan Wesson
 */

#include "keyboard.h"

#include <kernel/ioport.h>
#include <stdint.h>
#include <queue.h>
#include "../sys/interrupt/isr.h"

const uint8_t KEYBOARD_DATA_PORT   = 0x60;
const uint8_t KEYBOARD_STATUS_PORT = 0x64;
const uint8_t KEYBOARD_SETLED_COM  = 0xED;    //!< Command to set LED state.
const uint8_t KEYBOARD_CLEAR_FLAG  = 0x02;    //!< Status port flag for ready state.
enum { KEYBOARD_UP_FLAG = 0x80 };    //!< Flag in scancodes for key up.

//! Map of scan codes to characters.
static unsigned char keyboard_map[] = {
	0, KEY_ESC, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', '\t',                             // 00 - 0F
	'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', KEY_LCTRL, 'a', 's',                          // 10 - 1F
	'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', KEY_LSHIFT, '\\', 'z', 'x', 'c', 'v',                        // 20 - 2F
	'b', 'n', 'm', ',', '.', '/', KEY_RSHIFT, '*', KEY_LALT, ' ', KEY_CAPS, KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, // 30 - 3F
	KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_NUM, KEY_SCROLL, '7', '8', '9', '-', '4', '5', '6', '+', '1',      // 40 - 4F
	'2', '3', '0', '.', 0x54, 0x55, 0x56, KEY_F11, KEY_F12, 0x59, 0x5A, KEY_LLOGO_MASK, 0x5C, 0x5D, 0x5E, 0x5F,     // 50 - 5F
	0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,                 // 60 - 6F
	0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F                  // 70 - 7F
};

//! Map of scan codes to characters while shifed.
static unsigned char keyboard_map_shift[] = {
	0, KEY_ESC, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b', '\t',                             // 00 - 0F
	'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', KEY_LCTRL, 'A', 'S',                          // 10 - 1F
	'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '\"', '~', KEY_LSHIFT, '|', 'Z', 'X', 'C', 'V',                         // 20 - 2F
	'B', 'N', 'M', '<', '>', '?', KEY_RSHIFT, '*', KEY_LALT, ' ', KEY_CAPS, KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, // 30 - 3F
	KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_NUM, KEY_SCROLL, '7', '8', '9', '-', '4', '5', '6', '+', '1',      // 40 - 4F
	'2', '3', '0', '.', 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, KEY_LLOGO_MASK, 0x5C, 0x5D, 0x5E, 0x5F,           // 50 - 5F
	0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,                 // 60 - 6F
	0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F                  // 70 - 7F
};

static uint8_t mod_state = 0;    //!< State of modifier keys.
static uint8_t led_state = 0;    //!< State of keyboards LEDs.

static queue buffer;
static int buff_data[KEY_BUFFER_SIZE];

/**
 * Set the keyboard LED state.
 * @param status LED status flags.
 */
void keyboard_set_led(uint8_t status){
	led_state = status;
	while(!(inb(KEYBOARD_STATUS_PORT) & KEYBOARD_CLEAR_FLAG)){}
	outb(KEYBOARD_DATA_PORT, KEYBOARD_SETLED_COM);

	while(!(inb(KEYBOARD_STATUS_PORT) & KEYBOARD_CLEAR_FLAG)){}
	outb(KEYBOARD_DATA_PORT, status);
}

/**
 * Gets the keyboard LED state.
 * @return The LED status flags.
 */
uint8_t keyboard_get_led(){
	return led_state;
}

/**
 * Clears the keyboard input buffer.
 */
void keyboard_clear_buffer(){
	clear_queue(&buffer);
}

/**
 * Gets the first key stroke in the input buffer.
 * @return The key stroke, lower 8 bits are the character, upper 8 bits are modifier flags.
 */
uint16_t keyboard_get_key(){
	return (uint16_t)dequeue(&buffer);
}

/**
 * Keyboard interrupt callback function.
 * @param regs The registers at the time of the interrupt.
 */
static void keyboard_isr(isr_regs regs){
	(void)regs;

	unsigned char new_char = 0;
	unsigned char scan_code = inb(KEYBOARD_DATA_PORT);

	switch(scan_code){
		case 0x2A:    // Left Shift Down
			mod_state = mod_state | KEY_LSHIFT_MASK;
		break;
		case (0x2A | KEYBOARD_UP_FLAG):    // Left Shift Up
			mod_state = mod_state & ~KEY_LSHIFT_MASK;
		break;
		case 0x36:    // Right Shift Down
			mod_state = mod_state | KEY_RSHIFT_MASK;
		break;
		case 0x36 | KEYBOARD_UP_FLAG:    // Right Shift Up
			mod_state = mod_state & ~KEY_RSHIFT_MASK;
		break;
		case 0x3A: break;
		case 0x3A | KEYBOARD_UP_FLAG:    // Caps Lock Up
			led_state = led_state ^ KEY_CAPS_MASK;
			//ksetled(led_state);
		break;
		case 0x45: break;
		case 0x45 | KEYBOARD_UP_FLAG:    // Num Lock Up
			led_state = led_state ^ KEY_NUM_MASK;
			//ksetled(led_state);
		break;
		case 0x46: break;
		case 0x46 | KEYBOARD_UP_FLAG:    // Scroll Lock Up
			led_state = led_state ^ KEY_SCROLL_MASK;
			//ksetled(led_state);
		break;

		default:
			if(scan_code & KEYBOARD_UP_FLAG){
				/* Ignore the break code */
			}else{
				if(!(mod_state & (KEY_LSHIFT_MASK | KEY_RSHIFT_MASK)) != !(led_state & KEY_CAPS_MASK)){
					new_char = keyboard_map_shift[scan_code];
				}else{
					new_char = keyboard_map[scan_code];
				}
			}
		break;
	}

	if(new_char){
		enqueue(&buffer, (((uint16_t)mod_state << 8) | new_char));
	}
}

/**
 * Initializes the keyboard input driver.
 */
void keyboard_init(){
	new_queue(&buffer, (int*)&buff_data, KEY_BUFFER_SIZE);
	isr_register(IRQ1, &keyboard_isr);
}

