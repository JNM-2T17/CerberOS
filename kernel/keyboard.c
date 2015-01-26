#include "keymap.h"
#include "system.h"

/***
	Returns:
		the first character pressed fro the keyboard
***/
char getChar() {

	char c; /*receiver for the character*/

	do {
		c = inb( 0x60 ); /*try polling for a character*/
	} while( c & 0x80 ); /*while no character*/

	return keymap[c]; /*return mapped character*/
}
