#define VID_COLS 80 /*columns on the screen*/
#define VID_ROWS 25 /*rows on the screen*/
#define VID_DATA_SIZE 2 /*bytes per cell on screen*/

#define VID_PTR 0xb8000 /*video location on screen*/
#define GREY_ON_BLACK 0x07 /*grey text on black background*/

extern unsigned int i; /*basic video index*/
extern unsigned int k; /*next line index zero-based*/
extern char *vidPtr; /*global pointer to video portion in memory*/
extern unsigned int shellRow; /*row where shell prompt is located*/
extern char marqueeOffset;

/***
	shifts screen contents one row upwars
***/
void shiftScreen() {
	
	/*for each row except last*/
	for( i = 0; i < VID_COLS * VID_DATA_SIZE * ( VID_ROWS - 1 ); i++ ) {
		/*sets the contents of a cell to the one below it*/
		vidPtr[i] = vidPtr[ i + VID_COLS * VID_DATA_SIZE ];
	}
	
	/*clears last row*/
	while( i < VID_COLS * VID_DATA_SIZE * VID_ROWS ){
		vidPtr[i++] = 0;
		vidPtr[i++] = GREY_ON_BLACK;
	}
	
	i = ( VID_ROWS - 1 ) * 160; /*sets pointer to bottom-left cell*/

	setCursor(); /*sets cursor to current location*/
	shellRow--; /*the row where the shell prompt is located moves up*/
	marqueeOffset--;
}

/***
	prints a newline
***/
void newLine() {

	/*if not last row*/
	if( k < 25 ) {
		/*sets i to the first column of next row*/
		i = k++ * VID_DATA_SIZE * VID_COLS;
	} else {
		shiftScreen(); /*shifts screen*/
	}
}

/***
	 puts a value at current index location, moves index forward, and adjusts 
	 line no.; color is light-grey on black
	 Parameter: 
		 c - character to place
***/
void putChar( char c ) {

	if( c == '\b') { /*if backspace*/
		if( i > 0 ) { /*if not at beginning of video memory*/

			/*set previous character to null*/
			vidPtr[--i] = GREY_ON_BLACK;
			vidPtr[--i] = 0;

			/*if went back one line*/
			if( i % 160 == 158 ) {
				k--; /*decrement row counter*/
			}
		}
	} else if( c == '\n' ) { /*if newline*/
		newLine(); /*print newline*/
	} else if( c != '\0' ) {
		/*put character onscreen*/
		vidPtr[i++] = c;
		vidPtr[i++] = GREY_ON_BLACK;

		/*if on the next line*/
		if( c != 0 && i % 160 == 0 ) {
			newLine(); /*print newline*/
		}
	}
}

/***
	clears the screen
***/	
void clear() {
	
	/*clears screen*/
	for( i = 0; i < VID_COLS * VID_DATA_SIZE * VID_ROWS; ){
		/*put null character onscreen*/
		vidPtr[i++] = '\0';
		vidPtr[i++] = GREY_ON_BLACK;
	}

	i = 0; /*resets pointer*/
	k = 1; /*resets line*/

	setCursor(); /*sets cursor to current location*/
	marqueeOffset = -100;
}

/***
	prints a string
	Parameters:;
		str - string to print
***/
void printStr( char *str ){

	unsigned int j = 0; /*counter*/
	
	while( str[j] != '\0' ){ /*while not end of string*/
		putChar( str[j] ); /*set character*/
		j++; /*next char*/
	}

	setCursor(); /*sets cursor to current location*/
}

/***
	prints a string with cycling colors
	Parameters:
		str - string to print
***/
void printStrColor( char *str ){

	char color = 0x01; /*initial color*/
	unsigned int j = 0; /*counter*/
	
	while( str[j] != '\0' ){ /*while not at end of string*/
		if( str[j] != '\0' ) { /*if not last char*/
			putChar( str[j] ); /*puts char on screen*/
			vidPtr[i-1] = color++; /*changes color*/
			j++; /*next char*/

			if( color == 0x10 ) { /*if past last color*/
				color = 0x01; /*reset*/
			}
		}
	}

	setCursor(); /*sets cursor to current location*/
}

/***
	prints an integer recursively
	Parameter:
		n - integer to print
***/
void printIntRecursive( int n ) {

	int digit; /*current digit*/

	if( n > 0 ) { /*while integer is not 0*/
		digit = n % 10; /*get ones digit*/
		n /= 10; /*trim number*/
		
		printIntRecursive( n ); /*print rest of number*/
	
		putChar( digit + 0x30 ); /*put value on screen*/
	}
}

/***
	prints an integer
	Parameter:
		n - integer to print
***/
void printInt( int n ) {

	if( n == 0 ) { /*prints zero*/
		putChar( 0x30 );
	} else if( n < 0 ) {
		putChar( '-' );
		printInt( -n );
	} else { /*prints nonzero numbers*/
		printIntRecursive( n );
	}

	setCursor(); /*sets cursor to current location*/
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
void printHexRecursive( unsigned int n ) {

	int digit; /*current hex digit*/

	if( n > 0 ) { 
		digit = n % 16; /*gets last hex digit*/
		n /= 16; /*trims number*/
		
		printHexRecursive( n ); /*prints rest of number*/
		
		putChar( getHexDigit( digit ) ); /*puts char on screen*/
	}
}

/***
	prints an integer in hex
	Parameter:
		n - integer to print in hex
***/
void printHex( unsigned int n ) {
	
	if( n == 0 ) { /*prints zero*/
		putChar( 0x30 );
	} else {
		printHexRecursive( n ); /*prints nonzero nos*/
	}

	setCursor(); /*sets cursor to current location*/
}
