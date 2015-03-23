#include "printer.h"

#define VID_COLS 80 /*columns on the screen*/
#define VID_ROWS 25 /*rows on the screen*/
#define VID_DATA_SIZE 2 /*bytes per cell on screen*/

#define VID_PTR 0xb8000 /*video location on screen*/
#define GREY_ON_BLACK 0x07 /*grey text on black background*/

extern unsigned int i; /*basic video index*/
extern unsigned int k; /*next line index zero-based*/
extern char *vidPtr; /*global pointer to video portion in memory*/
extern char marqueeOffset;
extern void shiftScr( process * ); /*assembly function calling an 
												interrupt*/
extern unsigned int mainIndex;
extern process *console;

/***
	shifts screen contents one row upwars
***/
void shiftScreen( process *proc ) {

	outb( 0x20, 0x20 );
			
	/*for each row except last*/
	for( proc->screen.i = 0; proc->screen.i < VID_COLS * VID_DATA_SIZE * ( VID_ROWS - 1 ); 
		 (proc->screen.i)++ ) {
		/*sets the contents of a cell to the one below it*/
		proc->screen.screen[proc->screen.i] = 
			proc->screen.screen[ proc->screen.i + VID_COLS * VID_DATA_SIZE];
		if( proc->isMain ) {
			vidPtr[proc->screen.i] = 
				vidPtr[ proc->screen.i + VID_COLS * VID_DATA_SIZE];				
		}
	}
	
	/*clears last row*/
	while( proc->screen.i < VID_COLS * VID_ROWS * VID_DATA_SIZE ){
		proc->screen.screen[(proc->screen.i)++] = 0;
		proc->screen.screen[(proc->screen.i)++] = GREY_ON_BLACK;
		if( proc->isMain ) {
			vidPtr[(proc->screen.i) - 2] = 0;
			vidPtr[(proc->screen.i) - 1] = GREY_ON_BLACK;
		}
	}
	
	proc->screen.i = ( VID_ROWS - 1 ) * 160; /*sets pointer to bottom-left cell*/
	proc->screen.shellRow--;
	
	if( proc == console ) {
		marqueeOffset--;
	}
	
	setCursor(); /*sets cursor to current location*/
}

/***
	prints a newline
***/
void newLine( process *proc ) {

	/*if not last row*/
	if( proc->screen.j < 25 ) {
		/*sets i to the first column of next row*/
		proc->screen.i = (proc->screen.j)++ * VID_COLS * VID_DATA_SIZE;
	} else {
		shiftScr( proc );
	}
}

/***
	 puts a value at current index location, moves index forward, and adjusts 
	 line no.; color is light-grey on black
	 Parameter: 
		 c - character to place
***/
void putChar( process *proc, char c ) {

	if( c == '\b') { /*if backspace*/
		if( proc->screen.i > 0 ) { /*if not at beginning of video memory*/

			/*set previous character to null*/
			proc->screen.screen[--(proc->screen.i)] = GREY_ON_BLACK;
			proc->screen.screen[--(proc->screen.i)] = 0;

			/*if went back one line*/
			if( proc->screen.i % ( VID_ROWS * VID_DATA_SIZE ) == ( VID_ROWS - 1 ) * VID_DATA_SIZE  ) {
				(proc->screen.j)--; /*decrement row counter*/
			}
			
			if( proc->isMain ) {
				vidPtr[proc->screen.i + 2] = GREY_ON_BLACK;
				vidPtr[proc->screen.i + 1] = 0;	
			}
		}
	} else if( c == '\n' ) { /*if newline*/
		newLine( proc ); /*print newline*/
	} else if( c != '\0' ) {
		/*if last on the line*/
		if( proc->screen.i % 160 == 158 ) {
			newLine( proc ); /*print newline*/
		}
		
		/*put character onscreen*/
		proc->screen.screen[proc->screen.i++] = c;
		proc->screen.screen[proc->screen.i++] = GREY_ON_BLACK;
		
		if( proc->isMain ) {
			/*put character onscreen*/
			vidPtr[proc->screen.i - 2] = c;
			vidPtr[proc->screen.i - 1] = GREY_ON_BLACK;
		}
	}
}

/***
	clears the screen
***/	
void clear( process *proc ) {
	
	outb( 0x20, 0x20 );
	
	int i;
	
	/*clears screen*/
	for( i = 0; i < VID_COLS * VID_DATA_SIZE * VID_ROWS; ){
		/*put null character onscreen*/
		proc->screen.screen[i++] = '\0';
		proc->screen.screen[i++] = GREY_ON_BLACK;
		
		if( proc->isMain ) {
			vidPtr[i - 2] = '\0';
			vidPtr[i - 1] = GREY_ON_BLACK;
		}
	}

	proc->screen.i = 0; /*resets pointer*/
	proc->screen.j = 1; /*resets line*/

	if( proc->isMain ) {
		setCursor(); /*sets cursor to current location*/
	}
	
	if( mainIndex == 0 ) {
		marqueeOffset = -100;
	}
}

/***
	prints a string
	Parameters:;
		str - string to print
***/
void printStr( process *proc, char *str ){

	unsigned int j = 0; /*counter*/
	
	while( str[j] != '\0' ){ /*while not end of string*/
		putChar( proc, str[j] ); /*set character*/
		j++; /*next char*/
	}
	
	if( proc->isMain ) {
		setCursor(); /*sets cursor to current location*/
	}
}

/***
	prints a string with cycling colors
	Parameters:
		str - string to print
***/
void printStrColor( process *proc, char *str ){

	char color = 0x01; /*initial color*/
	unsigned int j = 0; /*counter*/
	
	while( str[j] != '\0' ){ /*while not at end of string*/
		if( str[j] != '\0' ) { /*if not last char*/
			putChar( proc, str[j] ); /*puts char on screen*/
			j++; /*next char*/

			if( proc->isMain ) {
				vidPtr[proc->screen.i - 1] = color++;
			}
			
			if( color == 0x10 ) { /*if past last color*/
				color = 0x01; /*reset*/
			}
		}
	}
	
	if( proc->isMain ) {
		setCursor(); /*sets cursor to current location*/
	}
}

/***
	prints an integer recursively
	Parameter:
		n - integer to print
***/
void printIntRecursive( process *proc, int n ) {

	int digit; /*current digit*/

	if( n > 0 ) { /*while integer is not 0*/
		digit = n % 10; /*get ones digit*/
		n /= 10; /*trim number*/
		
		printIntRecursive( proc, n ); /*print rest of number*/
	
		putChar( proc, digit + 0x30 ); /*put value on screen*/
	}
}

/***
	prints an integer
	Parameter:
		n - integer to print
***/
void printInt( process *proc, int n ) {

	if( n == 0 ) { /*prints zero*/
		putChar( proc, 0x30 );
	} else if( n < 0 ) {
		putChar( proc, '-' );
		printInt( proc, -n );
	} else { /*prints nonzero numbers*/
		printIntRecursive( proc, n );
	}

	if( proc->isMain ) {
		setCursor( proc ); /*sets cursor to current location*/
	}
}

/***
	returns the hex equivalent of a number form 0 to 15
***/
char getHexDigit( int n ) {

	if( n >= 0 && n <= 9 ) { /*if decimally representable*/
		return n + 0x30;
	} else if( n < 16 ){ /*if valid hex digit still*/
		return n + 0x37;
	} else { /*if invalid*/
		return '\0';
	}
}

/***
	prints an integer in hex recursively
	Parameter:
		n - integer to print in hex
***/
void printHexRecursive( process *proc, unsigned int n ) {

	int digit; /*current hex digit*/

	if( n > 0 ) { 
		digit = n % 16; /*gets last hex digit*/
		n /= 16; /*trims number*/
		
		printHexRecursive( proc, n ); /*prints rest of number*/
		
		putChar( proc, getHexDigit( digit ) ); /*puts char on screen*/
	}
}

/***
	prints an integer in hex
	Parameter:
		n - integer to print in hex
***/
void printHex( process *proc, unsigned int n ) {
	
	if( n == 0 ) { /*prints zero*/
		putChar( proc, 0x30 );
	} else {
		printHexRecursive( proc, n ); /*prints nonzero nos*/
	}

	if( proc->isMain ) { 
		setCursor( proc ); /*sets cursor to current location*/
	}
}
