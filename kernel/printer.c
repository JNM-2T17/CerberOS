#define VID_COLS 80
#define VID_ROWS 25
#define VID_DATA_SIZE 2

#define VID_PTR 0xb8000

unsigned int i = 0; /*basic video index*/
unsigned int k = 1; /*next line index zero-based*/
char *vidPtr = (char *)VID_PTR;
	
void clear() {
	
	char color = 0x01;
	unsigned int i = 0;
	unsigned int j = 0;
	
	while( i < VID_COLS * VID_DATA_SIZE * VID_ROWS ){
		vidPtr[i] = 0;
		vidPtr[i+1] = 0x07;
		i += 2;
	}

	i = 0;	
}

void shiftScreen() {
	i = 0;
	for( i = 0; i < VID_COLS * VID_DATA_SIZE * ( VID_ROWS - 1 ); i++ ) {
		vidPtr[i] = vidPtr[i+160];
	}
	while( i < VID_COLS * VID_DATA_SIZE * VID_ROWS ){
		vidPtr[i] = '\0';
		vidPtr[i+1];
		i += 2;
	}
	i = ( VID_ROWS - 1 ) * 160;
}

void adjust() {
	
	if( i == VID_COLS * VID_DATA_SIZE * VID_ROWS - 1 ) {
		shiftScreen();
	}
}

void printStr( char *str ){

	unsigned int j = 0;
	
	while( str[j] != '\0' ){
		adjust();
		while( str[j] == '\n' && str[j] != '\0' ){
			if( k == 25 ) {
				shiftScreen();
			} else {
				i = k++ * 160;
			}
			j++;
		}
		if( str[j] != '\0' ){
			vidPtr[i] = str[j];
			vidPtr[i+1] = 0x07;
		
			i += 2;
			if( i % 160 == 0 ){
				k++;
			}
			j++;
		}	
	}
}

void printStrColor( char *str ){

	char color = 0x01;
	unsigned int j = 0;
	
	while( str[j] != '\0' ){
		adjust();
		while( str[j] == '\n' && str[j] != '\0' ){
			if( k == 25 ) {
				shiftScreen();
			} else {
				i = k++ * 160;
			}
			j++;
		}
		if( str[j] != '\0' ){
			vidPtr[i] = str[j];
			vidPtr[i+1] = color;
		
			i += 2;
			if( i % 160 == 0 ){
				k++;
			}			
			j++;

			color++;
			if( color == 0x0F ) {
				color = 0x01;
			}
		}
	}
}

void printIntRecursive( int n ) {

	int digit;	

	if( n > 0 ) {
		adjust();
		digit = n % 10;
		n /= 10;
		
		printIntRecursive( n );
		vidPtr[i] = digit + 0x30;
		vidPtr[i+1] = 0x07;	
		i += 2;

		if( i % 160 == 0 ) {
			i = k++ * 160;
		}
	}
}

void printInt( int n ) {

	if( n == 0 ) {
		adjust();
		vidPtr[i] = 0x30;
		if( i % 160 == 0 ){
			k++;
		}
	} 
	else {
		printIntRecursive( n );
	}
}

char getHexDigit( int n ) {

	if( n >= 0 && n <= 9 ) {
		return n + 0x30;
	} else {
		return n + 0x37;
	}
}

void printHexRecursive( int n ) {

	int digit;	

	if( n > 0 ) {
		adjust();
		digit = n % 16;
		n /= 16;
		
		printHexRecursive( n );
		vidPtr[i] = getHexDigit( digit );
		vidPtr[i+1] = 0x07;	
		i += 2;

		if( i % 160 == 0 ) {
			i = k++ * 160;
		}
	}
}

void printHex( int n ) {
	
	if( n == 0 ) {
		vidPtr[i] = 0x30;
		vidPtr[i+1];
		if( i % 160 == 0 ){
			k++;
		}
	} 
	else {
		printHexRecursive( n );
	}
}

void kmain(){

	unsigned int i;

	clear();
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
	for( i = 0; i < 0xFFFF; i++ );
	for( i = 0; i < 100; i++ ) {
		printInt( i );
		printStr( "\n" );
	}
}
	