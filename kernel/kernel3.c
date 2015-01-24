#include "printer.h"

unsigned int i = 0; /*basic video index*/
unsigned int k = 1; /*next line index zero-based*/
char *vidPtr = (char *)VID_PTR; /*global pointer to video portion in memory*/

/***
	calls the assembly instruction outb
	Parameters:
		_port - hardware port to push data to
		_data - data to push to port
***/
void outb (unsigned short _port, unsigned char _data)
{
		__asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));
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

void kmain(){

	unsigned int i;
	/*splash screen*/
	char *splash =  "          _              _      _              _      _              _\n"
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
	setCursor();

	for( i = 0; i < 0x3FFFFFF; i++ ); /*makeshift sleep function*/
	
	clear(); /*clear screen*/
	printStr( "CerberOS>" ); /*display shell*/
	setCursor();
}