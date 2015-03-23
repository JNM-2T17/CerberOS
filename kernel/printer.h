#include "function.h"

#define VID_COLS 80 /*columns on the screen*/
#define VID_ROWS 25 /*rows on the screen*/
#define VID_DATA_SIZE 2 /*bytes per cell on screen*/

#define VID_PTR 0xb8000 /*video location on screen*/
#define GREY_ON_BLACK 0x07 /*grey text on black background*/

extern char *vidPtr; /*global pointer to video portion in memory*/

/***
	shifts screen contents one row upwars
***/
void shiftScreen();

void shiftProcScreen( process *proc );

/***
	prints a newline
***/
void newLine( process *proc );

/***
	 puts a value at current index location, moves index forward, and adjusts 
	 line no.; color is light-grey on black
	 Parameter: 
		 c - character to place
***/
void putChar( process *proc, char c );

/***
	clears the screen
***/	
void clear( process *);

/***
	prints a string
	Parameters:;
		str - string to print
***/
void printStr( process *proc, char *str );

/***
	prints a string with cycling colors
	Parameters:
		str - string to print
***/
void printStrColor( process *proc, char *str );

/***
	prints an integer
	Parameter:
		n - integer to print
***/
void printInt( process *proc, int n );

/***
	prints an integer in hex
	Parameter:
		n - integer to print in hex
***/
void printHex( process *proc, unsigned int n );
