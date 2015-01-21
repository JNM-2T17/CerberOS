#define VID_COLS 80 /*columns on the screen*/
#define VID_ROWS 25 /*rows on the screen*/
#define VID_DATA_SIZE 2 /*bytes per cell on screen*/

#define VID_PTR 0xb8000 /*video location on screen*/
#define GREY_ON_BLACK 0x07 /*grey text on black background*/

unsigned int i = 0; /*basic video index*/
unsigned int k = 1; /*next line index zero-based*/
char *vidPtr = (char *)VID_PTR; /*global pointer to video portion in memory*/

/***
   puts a value at current index location, moves index forward, and adjusts 
   line no.; color is light-grey on black
   Parameter: 
     c - character to place
***/
void putChar( char c ) {

  vidPtr[i++] = c;
  vidPtr[i++] = GREY_ON_BLACK;
  if( k != 25 && i % 160 == 0 ){ /*if end of row*/
    k++; /*increment line*/
  }
}

/***
  clears the screen
***/  
void clear() {
  
  /*clears screen*/
  for( i = 0; i < VID_COLS * VID_DATA_SIZE * VID_ROWS; ){
    putChar( '\0' );
  }

  i = 0; /*resets pointer*/
  k = 1; /*resets line*/
}

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
    putChar( '\0' );
  }
  
  i = ( VID_ROWS - 1 ) * 160; /*sets pointer to bottom-left cell*/
}

/***
  checks if pointer is at bottom-right of the screen and shifts contents upwards
  if so
***/
void adjust() {

  /*if at the bottom-right part of screen*/
  if( i == VID_COLS * VID_DATA_SIZE * VID_ROWS - 1 ) {
    shiftScreen(); /*shift contents upward*/
  }
}

/***
  prints a string
  Parameters:;
    str - string to print
***/
void printStr( char *str ){

  unsigned int j = 0; /*counter*/
  
  while( str[j] != '\0' ){ /*while not end of string*/
    adjust(); /*adjusts for last cell in screen*/
    
    while( str[j] == '\n' && str[j] != '\0' ){ /*while newline and not end of 
	                                             string*/
      if( k == 25 ) { /*if last row*/
        shiftScreen();
      } else { /*go to next line*/
        i = k++ * 160;
      }
      j++; /*next char*/
    }
    if( str[j] != '\0' ){ /*if not last char*/
      putChar( str[j] ); /*set character*/
      j++; /*next char*/
    }  
  }
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
    adjust();/*adjusts for last cell in screen*/
    
    while( str[j] == '\n' && str[j] != '\0' ){ /*while newline and not end of 
	                                             string*/
      if( k == 25 ) {/*if last row*/
        shiftScreen();
      } else { /*go to next line*/
        i = k++ * 160;
      }
      j++; /*next char*/
    }
    if( str[j] != '\0' ){ /*if not last char*/
      putChar( str[j] ); /*puts char on screen*/
	  vidPtr[i-1] = color++; /*changes color*/
      j++; /*next char*/

      if( color == 0x10 ) { /*if past last color*/
        color = 0x01; /*reset*/
      }
    }
  }
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
	
    adjust(); /*adjust for last character*/
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
    adjust(); /*adjust for last cell*/
    putChar( 0x30 );
  } 
  else { /*prints nonzero numbers*/
    printIntRecursive( n );
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
void printHexRecursive( int n ) {

  int digit; /*current hex digit*/

  if( n > 0 ) { 
    digit = n % 16; /*gets last hex digit*/
    n /= 16; /*trims number*/
    
    printHexRecursive( n ); /*prints rest of number*/
    
	adjust(); /*adjusts for bottom-right cell*/
	
    putChar( getHexDigit( digit ) ); /*puts char on screen*/
  }
}

/***
  prints an integer in hex
  Parameter:
    n - integer to print in hex
***/
void printHex( int n ) {
  
  if( n == 0 ) { /*prints zero*/
    putChar( 0x30 );
  } 
  else {
    printHexRecursive( n ); /*prints nonzero nos*/
  }
}

void kmain(){

  unsigned int i;

  clear();
  printStr("Hello\nHello\n");
  printInt(25495);
  printStr("\n");
  printHex(65535);  
  printStr("\n");
  printStr("1I am a pancake\n");
  printStr("2I am a pancake\n");
  printStr("3I am a pancake\n");
  printStr("4I am a pancake\n");
  printStr("5I am a pancake\n");
  printStr("6I am a pancake\n");
  printStr("7I am a pancake\n");
  printStr("8I am a pancake\n");
  printStr("9I am a pancake\n");
  printStr("10I am a pancake\n");
  printStr("11I am a pancake\n");
  printStr("12I am a pancake\n");
  printStrColor("13I am a pancake\n");
  printStrColor("14I am a pancake\n");
  printStrColor("15I am a pancake\n");
  printStrColor("16I am a pancake\n");
  printStrColor("17I am a pancake\n");
  printStrColor("18I am a pancake\n");
  printStrColor("19I am a pancake\n");
  printStrColor("20I am a pancake\n");
  printStrColor("21I am a pancake\n");
  printStrColor("OMG!!I am a sexy pancake\n");
  printStrColor("22xI am a pancake\nHUY! SEXY PANCAKE DAW!!!");
  printStrColor("23I am a pancake\n");
  printStrColor("24I am a pancake\n");
  printStrColor("25I am a pancake");
  
  for( i = 0; i < 0xFFFFFF; i++ ); /*pause*/
  
  /*prints nos 1 - 100 in decimal and hex*/
  for( i = 1; i <= 100; i++ ) {
    printInt( i );
    printStr( "," );
    printHex( i );
    if( i < 100 ) {
      printStr( "\n" );
    }
  }
}
