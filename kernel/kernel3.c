#include "system.h"
#include "printer.h"
#include "keyboard.h"

unsigned int i = 0; /*basic video index*/
unsigned int k = 1; /*next line index zero-based*/
char *vidPtr = (char *)VID_PTR; /*global pointer to video portion in memory*/

void kmain(){

	int nCtr;

	/*splash screen*/
	char *splash =  "      _              _      _              _      _              _\n"
					"     | |           _- |    | #           _- |    | #           _- |\n"
					"     |  |        _-   |    |  #        _-   |    |  #        _-   |\n"
					"      |_-  ----___-     |   |_-  ----___-     |   |_-  ----___-     |\n"
					"    __-           #  _# | __-           #  _# | __-           #  _# |\n"
					"    _-               -##- |-               -##- |-               -##- |\n"
					"  |   __              -_  | __              -_  | __              -_  |\n"
					"  |  |#-     _-#_       # ||#-     _-#_       # ||#-     _-#_       # |\n"
					" |           |##-        -_|       |##-        -_|       |##-        -_|\n"
					" |  _##_                   |#_                   |#_                   |\n"
					"- -|_#_#|_ -               |_#|_ -               |_#|_ -               |\n"
					"|  |-|-  __ _               |  __ _               |  __ _               |\n"
					"|   --__   _               |__   _               |__   _               |\n"
					"|      ---                |  ---                |  ---                |\n"
					"|                     _  |                  _  |                  _  |\n"
					" -_                 _    |                _    |                _    |\n"
					"  -_          ___-    _--_          ___-    _--_          ___-    _-\n"
					"    -______---     __-    -______---     __-    -______---     __-\n"
					"           --                    --                    --\n\n"
					"                          WELCOME TO CerberOS!";
	
	clear(); /*clear screen*/
	printStrColor( splash ); /*print dogedogedoge with colors*/

	sleep( 4000 ); /*sleep for four seconds*/
	
	clear(); /*clear screen*/

	/*for( nCtr = 0; nCtr < 100; i++ ) {
		printInt( nCtr );
		printStr( " , " );
		printHex( nCtr );
		sleep( 500 );
		printStr( "\n" );
	}*/

	clear();
	printStr( "CerberOS>" ); /*display shell*/
	
	while( 1 ) {
		char c = getChar();
		if( c == '\b' && i % 160 >= 20 || c != '\n' && c != '\b') {
			putChar(c);
			setCursor();
		} else if( c != '\b') {
			printStr("\nCerberOS>");
		}
		sleep( 150 );
	}
}
