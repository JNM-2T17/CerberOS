#include "printer.h"

#define VID_DATA_SIZE 2 /*bytes per video cell*/
#define BUFFER_SIZE 512 /*characters in command buffer*/

extern unsigned int i; /*current screen position*/
extern unsigned int k; /*next row number*/
extern unsigned int shellRow; /*row number of shell onscreen*/
extern char *splash; /*splash screen*/
extern char *cmdList; /*command list*/

char keyBuffer[BUFFER_SIZE]; /*command buffer*/

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
	fixes the command extracted from video memory
***/
void *fixCmd() {
	
	int i, j; /*counters*/

	i = j = 0; /*initialize counters*/

	while( keyBuffer[i] != '\0' ) { /*while not at end of buffer*/
		keyBuffer[j] = keyBuffer[i]; /*copy character*/ 
		j++; /*next char*/ 
		i += 2; /*skip color byte*/
	}
	keyBuffer[j] = '\0'; /*append null byte*/
}

/***
	gets a command from the command line
***/
void getCmd() {

	/*if no command*/
	if( vidPtr[( shellRow - 1 ) * 160 + 18] == '\0' ) {
		keyBuffer[0] = '\0'; /*set key buffer to null*/
	} else {
		/*copies all text in current shell row*/
		cpy( keyBuffer, vidPtr + ( shellRow - 1 ) * 160 + 18 );
	}
	fixCmd(); /*eliminate color bytes*/
}

/***
	processed command
***/
void process() {

	getCmd();

	if( !cmpIgnoreCase( keyBuffer, "cls" ) ) {
		clear(); /*clear screen*/
	} else if( !cmpIgnoreCase( keyBuffer, "help" ) ) {
		printStr( cmdList ); /*show commands*/
	} else if( !cmpIgnoreCase( keyBuffer, "woof" ) ) {
		clear();
		printStrColor( splash ); /*show doge*/
		sleep(4000);
		clear();
	} else if( len( keyBuffer ) > 0 ) { /*if not empty function*/
		printStr("\n       \"");
		printStr( keyBuffer );
		printStr( "\" is not a valid function. \n       Enter \"help\""
			  " for all possible commands, woof woof woof. - Cerb"
			  "erOS" );
	}

	/*if screen wasn't cleared*/
	if( i > 0 ) {
		newLine();
	}			
	printStr("CerberOS>"); /*put shell*/
	shellRow = i / 160 + 1; /*update shell row*/
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
			process();
		}
		sleep( 125 ); /*make keyboard sleep*/
	}
}
