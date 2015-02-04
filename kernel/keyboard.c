#include "keymap.h"
#include "system.h"

/***
	Returns:
		the first character pressed fro the keyboard
***/
char getChar() {

	char c; /*receiver for the character*/
	
	if( inb(0x64) & 0x01 ) {
		c = inb( 0x60 ); /*try polling for a character*/
		if( c < 0 ) {
			return '\0';
		} else {
			return keymap[c]; /*return mapped character*/		
		}
	}
}
