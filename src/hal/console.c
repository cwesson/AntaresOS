/**
 * @file hal/console.c
 * Console I/O device driver.
 * @author Conlan Wesson
 */

#include "console.h"

#include <stdio.h>
#include <ctype.h>
#include "device.h"
#include "../dev/keyboard.h"
#include "../dev/vga.h"

#define VROWS 25
#define VROW_MAX (VROWS-1)
#define VCOLS 80
#define VCOL_MAX (VCOLS-1)

#define TAB_WIDTH 4

/**
 * Calculates the offset of a text cell.
 * @param X The column coordinate.
 * @param Y The row coordinate.
 * @return The VRAM offset of the cell.
 */
#define VPOS(X, Y)  (((Y)*(VCOLS))+(X))
/* (Y*VCOLS)+X = P
 * Y*VCOLS = P-X
 * Y = (P-X)/VCOLS
 * 
 * (Y*VCOLS)+X = P
 * X = P-(Y*VCOLS)
 * X = P%VCOLS
 */

#define BLACK    0x0
#define BLUE     0x1
#define GREEN    0x2
#define CYAN     0x3
#define RED      0x4
#define MAGENTA  0x5
#define BROWN    0x6
#define LGRAY    0x7
#define DGRAY    0x8
#define LGREY    0x7
#define DGREY    0x8
#define LBLUE    0x9
#define LGREEN   0xA
#define LCYAN    0xB
#define LRED     0xC
#define LMAGENTA 0xD
#define YELLOW   0xE
#define WHITE    0xF

/**
 * Creates a video color code.
 * @param X The background color.
 * @param Y The foreground color.
 * @return The video color code.
 */
#define COLOR(X, Y) ((uint8_t)(((X)<<4)|(Y)))

#define VTEXT  COLOR(BLACK, WHITE)
#define VERROR COLOR(BLACK, RED)
#define VDEBUG COLOR(YELLOW, BLACK)
#define VBLANK (((VTEXT)<<8) | ' ')

#define MAX_LENGTH 255    //!< Maximum string length.

//! Current column of the cursor.
static int8_t vcol = 0;
//! Current row of the cursor.
static int8_t vrow = 0;
//! Current background color.
static uint8_t vbg = (VTEXT & 0xF0) >> 4;
//! Current foreground color.
static uint8_t vfg = (VTEXT & 0x0F);
//! Swap background and foreground.
static bool reverse = false;
//! Use bright colors.
static bool bright = false;
//! Do not show output.
static bool conceal = false;

//! Enum to indicate the state of an escape code.
enum escaped_level{
	ESC_NONE = 0,    //!< No escape character has been read.
	ESC_START,       //!< Escape character has been read, looking for CSI.
	ESC_READ         //!< CSI has been read, reading command data.
};

//! Enum to inidicate what to clear.
enum clear_amount{
	CLEAR_AFTER,     //!< Clear to the right of the cursor.
	CLEAR_BEFORE,    //!< Clear to the left of the cursor.
	CLEAR_ALL        //!< Clear everything.
};

//! Current state of the output escape code.
static enum escaped_level escaped = ESC_NONE;

/**
 * Initiliaze the console I/O device.
 */
void console_init(){
	
}

/**
 * Moves the cursor on the screen.
 * @param col The column to move the cursor to, negative for current column.
 * @param row The row to move the cursor to, negative for current row.
 */
static void console_setcursor(int8_t col, int8_t row){
	if(row >= 0){
		vrow = row;
	}
	if(col >= 0){
		vcol = col;
	}
	if(vrow > VROW_MAX){
		vrow = VROW_MAX;
	}
	if(vcol > VCOL_MAX){
		vcol = VCOL_MAX;
	}
	vga_desc.bwrite(VGA_ADDR_CURSOR, VPOS(vcol, vrow));
}

/**
 * Moves the cursor by the amount specified.
 * @param amount The amount to move the cursor.
 */
void console_mvcursor(int16_t amount){
	int16_t pos = (vrow * VCOLS) + vcol;
	pos += amount;
	if(pos < 0){
		pos = 0;
	}
	vcol = pos % VCOLS;
	vrow = pos / VCOLS;
	console_setcursor(-1, -1);
}

/**
 * Clears the screen.
 * @param amount What to clear.
 */
static void console_clear(enum clear_amount amount){
	if(amount == CLEAR_ALL){
		for(int i = 0; i < VROWS*VCOLS; i++){
			vga_desc.bwrite(i, VBLANK);
			console_setcursor(0, 0);
		}
		console_setcursor(0, 0);
	}else if(amount == CLEAR_BEFORE){
		for(int i = 0; i < vrow*vcol; i++){
			vga_desc.bwrite(i, VBLANK);
		}
	}else{
		for(int i = vrow*vcol; i < 0; i++){
			vga_desc.bwrite(i, VBLANK);
		}
	}
}

/**
 * Clears the line.
 * @param amount What to clear.
 */
static void console_clearln(enum clear_amount amount){
	if(amount == CLEAR_ALL){
		for(int i = 0; i < VCOLS; i++){
			vga_desc.bwrite(VPOS(i, vrow), VBLANK);
			console_setcursor(0, vrow);
		}
	}else if(amount == CLEAR_BEFORE){
		for(int i = 0; i < vcol; i++){
			vga_desc.bwrite(VPOS(i, vrow), VBLANK);
		}
	}else{
		for(int i = vcol; i < VCOLS; i++){
			vga_desc.bwrite(VPOS(i, vrow), VBLANK);
		}
	}
}

/**
 * Scrolls the screen up one line.
 * @param amount Number or lines to scroll.
 */
static void console_scroll(int8_t amount){
	if(amount > 0){
		int b = 0;
		int a = 0;
		for(int i = 0; i <= VPOS(VCOL_MAX, VROW_MAX); i++){
			if(i <= VPOS(VCOL_MAX, VROW_MAX-amount)){
				vga_desc.bwrite(i, vga_desc.bread(i + (VCOLS*amount)));
				b++;
			}else{
				vga_desc.bwrite(i, VBLANK);
				a++;
			}
		}
		console_setcursor(0, VROW_MAX);
	}else if(amount < 0){
		amount = -amount;
		for(int i = VPOS(VCOL_MAX, VROW_MAX); i >= 0; i--){
			if(i > VPOS(VCOL_MAX, amount-1)){
				vga_desc.bwrite(i, vga_desc.bread(i - (VCOLS*amount)));
			}else{
				vga_desc.bwrite(i, VBLANK);
			}
		}
		console_setcursor(0, 0);
	}
}

/**
 * Processes ANSI escape codes character-by-character.
 * @param ch The next character to process.
 */
static void proc_escape(unsigned char ch){
	static char buffer[16] = {-1};
	static int pos = 0;
	static int8_t saved_col = -1;
	static int8_t saved_row = -1;
	
	if(escaped == ESC_START && ch == '['){
		escaped = ESC_READ;
	}else if(escaped == ESC_READ){
		if(ch == 'A'){
			// Cursor up
			if(buffer[0] < 0){
				buffer[0] = 1;
			}
			console_setcursor(vcol, vrow - buffer[0]);
			escaped = ESC_NONE;
		}else if(ch == 'B'){
			// Cursor down
			if(buffer[0] < 0){
				buffer[0] = 1;
			}
			console_setcursor(vcol, vrow + buffer[0]);
			escaped = ESC_NONE;
		}else if(ch == 'C'){
			// Cursor right
			if(buffer[0] < 0){
				buffer[0] = 1;
			}
			console_setcursor(vcol + buffer[0], vrow);
			escaped = ESC_NONE;
		}else if(ch == 'D'){
			// Cursor left
			if(buffer[0] < 0){
				buffer[0] = 1;
			}
			console_setcursor(vcol - buffer[0], vrow);
			escaped = ESC_NONE;
		}else if(ch == 'E'){
			// Cursor next line
			if(buffer[0] < 0){
				buffer[0] = 1;
			}
			console_setcursor(0, vrow + buffer[0]);
			escaped = ESC_NONE;
		}else if(ch == 'F'){
			// Cursor previous line
			if(buffer[0] < 0){
				buffer[0] = 1;
			}
			console_setcursor(0, vrow - buffer[0]);
			escaped = ESC_NONE;
		}else if(ch == 'G'){
			// Cursor horizontal absolute
			console_setcursor(buffer[0], vrow);
			escaped = ESC_NONE;
		}else if(ch == 'H' || ch == 'f'){
			// Set cursor position
			if(buffer[0] <= 0){
				buffer[0] = 1;
			}
			if(buffer[1] <= 0){
				buffer[1] = 1;
			}
			console_setcursor(buffer[1]-1, buffer[0]-1);
			escaped = ESC_NONE;
		}else if(ch == 'J'){
			// Erase Display
			if(buffer[0] == 2){
				console_clear(CLEAR_ALL);
			}else if(buffer[0] == 1){
				console_clear(CLEAR_BEFORE);
			}else{
				console_clear(CLEAR_AFTER);
			}
			escaped = ESC_NONE;
		}else if(ch == 'K'){
			// Erase line
			if(buffer[0] == 2){
				console_clearln(CLEAR_ALL);
			}else if(buffer[0] == 1){
				console_clearln(CLEAR_BEFORE);
			}else{
				console_clearln(CLEAR_AFTER);
			}
			escaped = ESC_NONE;
		}else if(ch == 'S'){
			// Scroll up
			if(buffer[0] < 0){
				buffer[0] = 1;
			}
			console_scroll(buffer[0]);
			escaped = ESC_NONE;
		}else if(ch == 'T'){
			// Scroll down
			if(buffer[0] < 0){
				buffer[0] = 1;
			}
			console_scroll(-buffer[0]);
			escaped = ESC_NONE;
		}else if(ch == 's'){
			// Save cursor position
			saved_col = vcol;
			saved_row = vrow;
			escaped = ESC_NONE;
		}else if(ch == 'u'){
			// Restore cursor position
			if(saved_col >= 0){
				vcol = saved_col;
				saved_col = -1;
			}
			if(saved_row >= 0){
				vrow = saved_row;
				saved_row = -1;
			}
			escaped = ESC_NONE;
		}else if(ch == 'm'){
			// Set graphics mode
			for(int i = 0; i <= pos; i++){
				if(buffer[i] == 0){
					vbg = (VTEXT & 0xF0) >> 4;
					vfg = (VTEXT & 0x0F);
					reverse = false;
					bright = false;
					conceal = false;
				}else if(buffer[i] == 1){
					bright = true;
				}else if(buffer[i] == 2){
					bright = false;
				}else if(buffer[i] == 7){
					reverse = true;
				}else if(buffer[i] == 8){
					conceal = true;
				}else if(buffer[i] == 27){
					reverse = false;
				}else if(buffer[i] == 28){
					conceal = false;
				}else if(buffer[i] == 30){
					vfg = BLACK;
				}else if(buffer[i] == 31){
					vfg = RED;
				}else if(buffer[i] == 32){
					vfg = GREEN;
				}else if(buffer[i] == 33){
					vfg = BROWN;
				}else if(buffer[i] == 34){
					vfg = BLUE;
				}else if(buffer[i] == 35){
					vfg = MAGENTA;
				}else if(buffer[i] == 36){
					vfg = CYAN;
				}else if(buffer[i] == 37){
					vfg = LGRAY;
				}else if(buffer[i] == 40){
					vbg = BLACK;
				}else if(buffer[i] == 41){
					vbg = RED;
				}else if(buffer[i] == 42){
					vbg = GREEN;
				}else if(buffer[i] == 43){
					vbg = BROWN;
				}else if(buffer[i] == 44){
					vbg = BLUE;
				}else if(buffer[i] == 45){
					vbg = MAGENTA;
				}else if(buffer[i] == 46){
					vbg = CYAN;
				}else if(buffer[i] == 47){
					vbg = LGRAY;
				}
			}
			escaped = ESC_NONE;
		}else if(isdigit(ch)){
			if(buffer[pos] < 0){
				buffer[pos] = ch - '0';
			}else{
				buffer[pos] = (buffer[pos] * 10) + (ch - '0');
			}
		}else if(ch == ';'){
			pos++;
		}else{
			// Error parsing escape sequence
			escaped = ESC_NONE;
		}
	}else{
		escaped = ESC_NONE;
	}
	
	if(escaped == ESC_NONE){
		// Escape code has ended or errored, clear the buffer.
		for(int i = 0; (unsigned int)i < sizeof(buffer)/sizeof(unsigned char); i++){
			buffer[i] = 0;
		}
		pos = 0;
	}
}

/**
 * Write to console output.
 * @param value Value to write.
 * @return Error code or EOK.
 */
int console_write(char value){
	if(escaped){
		proc_escape(value);
	}else if(value == '\e'){
		escaped = ESC_START;
		console_setcursor(vcol, vrow);
	}else if(!conceal){
		if(value == '\n'){
			vcol = 0;
			vrow++;
			if(vrow > VROW_MAX){
				console_scroll(1);
				vrow = VROW_MAX;
			}
			console_setcursor(vcol, vrow);
		}else if(value == '\t'){
			int count = TAB_WIDTH - (vcol % TAB_WIDTH);
			for(int i = 0; i < count; i++){
				console_write(' ');
				if(!vcol){
					break;
				}
			}
		}else if(isprint(value)){
			uint8_t outc = COLOR(vbg, vfg);
			if(reverse){
				outc = (outc >> 4) | (outc << 4);
			}
			if(bright){
				outc = outc | 0x08;
			}
			vga_desc.bwrite(VPOS(vcol, vrow), (outc << 8) | value);
			vcol++;
		}
		if(vcol > VCOL_MAX){
			vcol = 0;
			vrow++;
			if(vrow > VROW_MAX){
				console_scroll(1);
				vrow = VROW_MAX;
			}
			console_setcursor(vcol, vrow);
		}
	}
	console_desc.write_count++;
	return EOK;
}

/**
 * Read from the console input.
 * @return The value read.
 */
char console_read(){
	console_setcursor(-1, -1);
	char ch = keyboard_get_key() & 0xFF;
	if(isprint(ch)){
		console_write(ch);
		console_setcursor(-1, -1);
	}else if(ch == '\b'){
		console_mvcursor(-1);
		console_write(' ');
		console_mvcursor(-1);
	}
	console_desc.read_count++;
	return ch;
}

/**
 * Flushes the console input and output streams.
 * @return Error code or EOK.
 */
int console_flush(){
	keyboard_clear_buffer();
	return EOK;
}

//! Console I/O device descriptor.
device_descriptor console_desc = {
	"Console I/O",
	DEVICE_FLAG_INOUT | DEVICE_FLAG_STREAM | DEVICE_FLAG_VIRTUAL,
	0, 0,
	0, 0,
	0, 0,
	console_read, console_write, console_flush
};
