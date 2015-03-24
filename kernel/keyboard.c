#include "keymap.h"
#include "system.h"

unsigned char lShift = 0; /*boolean for left shift*/
unsigned char rShift = 0; /*boolean for right shift*/
unsigned char capsLock = 0; /*boolean for caps lock*/
unsigned char alt = 0; /*boolean for alt*/
unsigned char ctrl = 0; /*boolean for ctrl*/

/***
	Returns:
		the first character pressed from the keyboard
***/
unsigned char getChar() {

	unsigned char c; /*receiver for the character*/
	
	if( inb(0x64) & 0x01 ) {
		c = inb( 0x60 ); /*try polling for a character*/
 		/*printInt( getMainProc(), c & 0x7F );
 		printStr( getMainProc(), " " );*/
		if( c & 0x80 ) { /*if key was released*/
			c = c & 0x7F; /*get bits 0 - 6*/
			if( keyMap[c] == LEFT_SHIFT ) { /*if left shift*/
				lShift = 0; /*reset left shift*/
			} else if( keyMap[c] == RIGHT_SHIFT ) { /*if right shift*/
				rShift = 0; /*reset right shift*/
			} else if( keyMap[c] == CTRL ) { /*if control*/
				ctrl = 0; /*reset control*/
			} else if( keyMap[c] == ALT ) { /*if alt*/
				alt = 0; /*reset alt*/
			}
			return '\0'; /*return nothing*/
		} else { /*if key was pressed*/
			if( keyMap[c] == LEFT_SHIFT ) { /*if left shift*/
				lShift = 1; /*set left shift*/
				c = 0; /*set character to null*/
			} else if( keyMap[c] == RIGHT_SHIFT ) { /*if right shift*/
				rShift = 1; /*set right shift*/
				c = 0; /*set character to null*/
			} else if( keyMap[c] == ALT ) { /*if alt*/
				alt = 1; /*set alt*/
				c = 0; /*set character to null*/
			} else if( keyMap[c] == CTRL ) { /*if control*/
				ctrl = 1; /*reset control*/
				c = 0; /*set character to null*/
			} else if( keyMap[c] == CAPS_LOCK ) { /*if caps lock*/
				capsLock = 1 - capsLock; /*toggle caps lock*/
				c = 0; /*set character to null*/
			} else if( lShift || rShift ) { /*if shifted*/
				c = shiftKeyMap[c]; /*return shifted character*/
			} else {
				c = keyMap[c]; /*return mapped character*/		
			}
	
			if( capsLock ) { /*if caps lock*/
				if( c >= 'A' && c <= 'Z' ) { /*if uppercase*/
					return c += 'a' - 'A'; /*return lowercase*/
				} else if( c >= 'a' && c <= 'z' ) { /*if lowercase*/
					return c -= 'a' - 'A'; /*return uppercase*/
				} else {
					return c;
				}
			} else return c;
		}
	}
}
