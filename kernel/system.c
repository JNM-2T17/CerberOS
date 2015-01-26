#include "printer.h"

#define VID_DATA_SIZE 2

extern unsigned int i; /*current screen position*/
extern unsigned int k; /*next row number*/
extern unsigned int shellRow; /*row number of shell onscreen*/

/***
	calls the assembly instruction outb
	Parameters:
		_port - hardware port to push data to
		_data - data to push to port
***/
void outb (unsigned short _port, unsigned char _data) {

		__asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));
}

/***
	calls the assembly instruction inb
	Parameters:
		_port - hardware port to get data from
	Returns:
		data received from port
***/
unsigned char inb (unsigned short _port) {

		unsigned char _data;

		__asm__ __volatile__ ("inb %1, %0" : "=a" (_data) : "dN" (_port) );

		return _data;
}

/***
	sets cursor location to where i is at.
***/
void setCursor() {

	unsigned short pos = i / VID_DATA_SIZE; /*gets actual current position, dis-
											  regarding the 2 bytes/cell*/
	
	outb( 0x3D4, 0x0F ); /*writes to lower byte of output port*/
	outb( 0x3D5, (unsigned char)( pos & 0xFF ) ); /*pushes position's lower 
													bytes*/
	outb( 0x3D4, 0x0E ); /*writes to upper bytes of output port*/
	outb( 0x3D5, (unsigned char)( pos >> 8 ) & 0xFF ); /*pushes position's upper 
														 bytes*/
}

/***
	sends the system to sleep
	Parameter:
		msec - approx no. of msec
***/
void sleep( unsigned int msec ) {
	
	int i;
	for( i = 0; i < msec * 20000; i++ );
}

/***
	shows the shell and handles shell input
***/
void shell() {

	char c; /*character to be read*/
	
	printStr( "CerberOS>" ); /*display shell*/
	shellRow = i / 160 + 1;	/*get shell row*/

	while( 1 ) { /*infinite loop*/
		c = getChar(); /*get a character*/

		/*if backspace and cursor is beyond shell or row is beyond shellRow or 
		  if not a newline and not a backspace*/
		if( c == '\b' && ( i % 160 >= 20 || k > shellRow ) || c != '\n' && 
			c != '\b') {
			putChar(c);
			setCursor();
		} else if( c == '\n') { /*if newline*/
			printStr("\nCerberOS>"); /*put shell*/
			shellRow = i / 160 + 1; /*update shell row*/
		}
		sleep( 125 ); /*make keyboard sleep*/
	}
}
