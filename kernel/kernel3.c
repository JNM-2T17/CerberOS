#include "system.h"
#include "printer.h"
#include "keyboard.h"
#include "stringdoge.h"

unsigned int i = 0; /*basic video index*/
unsigned int k = 1; /*next line index zero-based*/
char *vidPtr = (char *)VID_PTR; /*global pointer to video portion in memory*/
unsigned int shellRow; /*row on screen where the current shell is printed*/

void kmain(){

	int nCtr; /*generic counter*/
	
	/*clear();	
	printStr( "test\n");
	char *pika = "PoKaChU";
	char *poka = "pOkAcHu";
	printInt( cmpIgnoreCase( pika, poka ) );
	printStr( pika );
	printStr( poka );
	sleep(10000);*/
	
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

	clear(); /*clear screen*/
	shell();
}
