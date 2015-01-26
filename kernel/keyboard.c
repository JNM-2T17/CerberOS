#include "keymap.h"
#include "system.h"

/***
	returns the first character pressed fro the keyboard
***/
char getChar() {

	char c;

	do {
		c = inb( 0x60 );
	} while( c & 0x80 );

	return keymap[c];
}
