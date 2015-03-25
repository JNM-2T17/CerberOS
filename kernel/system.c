#include "printer.h"
#include "interrupt.h"
#include "frozen.h"
#include "marquee.h"

#define VID_DATA_SIZE 2 /*bytes per video cell*/
#define BUFFER_SIZE 513 /*characters in command buffer*/
#define CMD_SIZE 11 /*command length*/

#define ADD 0 /*macro for addition in arith(int) function*/
#define SUB 1 /*macro for subtraction in arith(int) function*/
#define MUL 2 /*macro for multiplication in arith(int) function*/

extern char *splash; /*splash screen*/
extern char *cmdList; /*command list*/
extern unsigned char alt; /*whether alt is pressed*/
extern unsigned char ctrl; /*whether ctrl is pressed*/

char temp[BUFFER_SIZE]; /*dump string*/

unsigned int dump; /*dump int variable*/
char *vidPtr = (char *)VID_PTR; /*global pointer to video portion in memory*/
unsigned int timerCtr = 0; /*count of timer ticks*/
unsigned char switchNow = 0;
unsigned char deact = 0;
process *console;

extern unsigned char mainIndex;

extern void asmtest( int x );
extern int asmtest2( int x );
extern void clrscr( process *);
extern void _newMarquee( char *args, unsigned char row );
extern void switchTo( process * );
extern void activate( int );
extern void deacAsm( process *, unsigned int);

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

	/*gets actual current position, disregarding the 2 bytes/cell*/
	unsigned short pos = getMainProc()->screen.i / VID_DATA_SIZE; 
	
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
	for( i = 0; i < msec * 20000; i++ ); /*loop for approximate time given in 
										   input*/
}

/***
	shows splash screen
	Parameter:
		duration - how long to show doge in milliseconds
***/
void showDoge( int duration ) {

	clrscr( console );
	printStrColor( console, splash ); /*show doge*/
	sleep(duration);
	clrscr( console );
}

/***
	fixes the command extracted from video memory
***/
void *fixCmd( process *proc ) {
	
	int i = 0; /*counters*/
	
	/*eliminate leading whitespace*/
	while( proc->keyBuffer[i] == ' ' ) { /*while whitespace*/
		i++;
	}
	
	cpy( temp, proc->keyBuffer + i ); /*copy command without leading whitespace*/
	cpy( proc->keyBuffer, temp ); /*copy to buffer*/
}

/***
	gets a command from the command line
***/
void getCmd( process *proc ) {

	fixCmd( proc ); /*eliminate whitespace*/
	
	int nCtr = 0; /*counter*/

	/*while not at end of command, until a space is found*/
	while( proc->keyBuffer[nCtr] != ' ' && proc->keyBuffer[nCtr] != '\0' ) {
		nCtr++;
	}

	if( nCtr > 10 ) {
		nCtr = 10;
	}

	/*if has arguments*/
	if( proc->keyBuffer[nCtr] != '\0' ) {
		proc->keyBuffer[nCtr] = '\0'; /*end command*/
		nCtr++; /*next char*/
		while( proc->keyBuffer[nCtr] == ' ' ) { /*disregard whitespace*/
			nCtr++;
		}
		cpy( proc->args, proc->keyBuffer + nCtr );
		
		/*remove trailing spaces*/
		nCtr = len( proc->args ) - 1; /*get last char index*/
		while( proc->args[nCtr] == ' ' ) { /*while space*/
			nCtr--; /*prev char*/
		}
		
		proc->args[ nCtr + 1 ] = '\0';
	} else { /*empty arguments*/
		cpy( proc->args, "" );
	}
	cpy( proc->command, proc->keyBuffer );
	proc->command[CMD_SIZE - 1] = '\0';
}

/***
	parses an unsigned integer
	Parameters:
		args - pointer to string containing integer
***/
unsigned int parseInt( char *args ) {
	
	unsigned int temp = 0;
	int i = 0; /*counter*/

	while( args[i] - '0' >= 0 && args[i] - '0' < 10 ) { /*while digit*/
		temp *= 10; /*move to the left*/
		temp += args[i] - '0'; /*add units digit*/
		i++; /*next character*/
	}
	
	return temp;
}

/***
	checks if "args" follows the regex
	"[0-9]*\s[0-9]*"
***/
int arithIsValid( process *proc ) {
	
	int i, /*counter*/
	    valid = 1, /*whether args is valid*/ 
	    argCtr = 0;	/*number of arguments*/
	
	for( i = 0; valid && i <= len( proc->args ); i++ ) { /*check if args are valid*/
		if( proc->args[i] == ' ' || proc->args[i] == '\0' ) { /*if delimiter*/
			argCtr++; /*increment counter*/
			if( argCtr > 2 ) { /*if more than 2 args*/
				valid = 0; /*invalid*/
			}
			if( proc->args[i] == ' ' ){ /*if not end of args*/
				do {
					i++; /*next char*/
				} while( proc->args[i] == ' ' ); /*until next argument*/
				i--; /*step back for increment*/
			}
		} else if( proc->args[i] - '0' < 0 || proc->args[i] - '0' >= 10 ) { /*if not digit*/
			valid = 0; /*set valid to false*/
		}
	}
	
	if( argCtr < 2 ) { /*if less than 2 arguments*/
		valid = 0; /*invalid*/
	}

	return valid;
}

/***
	returns the index of the second argument in "args"
***/
int getArg2Index( process *proc ) {

	int i = 0; /*counter*/

	while( proc->args[i] - '0' >= 0 && proc->args[i] - '0' < 10 ) { /*while digit*/
		i++; /*next char*/
	}
	while( proc->args[i] == ' ' ) { /*while whitespace*/
		i++; /*next char*/
	}
	
	return i;
}

/***
	displays the sum of the integers in the args buffer
***/
void arith( int oper ) {
	
	int res; /*result*/
	int i; /*counter*/
	
	if( arithIsValid( console ) ) { /*if valid arguments*/
		res = parseInt( console->args ); /*get first argument*/
		i = getArg2Index( console ); /*get next argument's index*/
		switch( oper ) { /*determine operation*/
			case ADD:
				res += parseInt( console->args + i ); /*add*/
				break;
			case SUB:
				res -= parseInt( console->args + i ); /*subtract*/
				break;
			default:
				res *= parseInt( console->args + i ); /*multiply*/
				break;
		}

		/*display result*/
		newLine( console );
		printInt( console, res );
	} else { /*if invalid*/
		printStr( console, 
				  "\nPlease input two non-negative integers"); /*display error*/
	}
}

/***
	appends a character to the command
***/
void appendCmd( process *proc, char c ) {

	if( c == '\b' && proc->cmdIndex > 0 ) { /*if backspace and buffer is not empty*/
		proc->cmdIndex--;
	} else if( c != '\b' ) { /*if not backspace*/
		proc->keyBuffer[proc->cmdIndex] = c; /*set character*/
		proc->cmdIndex++; /*next index*/
	}
}

/***
	processed command
***/
void processCmd() {

	getCmd( console ); /*get command*/
		
	cpy( temp, console->command ); /*copy command because cmpIgnoreCase converts it to 
						    lowercase*/

	if( !cmpIgnoreCase( console->command, "cls" ) ) {
		clrscr( console ); /*clear screen*/
	} else if( !cmpIgnoreCase( console->command, "help" ) ) {
		printStr( console, cmdList ); /*show console->commands*/
	} else if( !cmpIgnoreCase( console->command, "woof" ) ) {
		showDoge(1000);		
	} else if( !cmpIgnoreCase( console->command, "say" ) ) {
		newLine( console ); /*show argument*/
		printStr( console, console->args );
	} else if( !cmpIgnoreCase( console->command, "add" ) ) {
		arith( ADD );/*add arguments*/
	} else if( !cmpIgnoreCase( console->command, "sub" ) ) {
		arith( SUB ); /*subtract arguments*/
	} else if( !cmpIgnoreCase( console->command, "mul" ) ) {
		arith( MUL ); /*multiply arguments*/
	} else if( !cmpIgnoreCase( console->command, "goAway" ) ) {
		goAway(console->args); /*tells a singer to go away based on args*/
	} else if( !cmpIgnoreCase( console->command, "hey" ) ) {
		callSinger(console->args); /*calls a singer based on args*/
	} else if( !cmpIgnoreCase( console->command, "marquee" ) ) {
		console->args[78] = '\0'; /*cap marquee text*/
		_newMarquee(console->args, console->screen.i / 160 + 1 ); /*creates a marquee*/
	} else if( !cmpIgnoreCase( console->command, "switch" ) ) {
		switchNow = 1;
	} else if( !cmpIgnoreCase( console->command, "test" ) ) {
		dump = parseInt(console->args);
		asmtest(dump);
		printStr(console, "\nSUCCESS!");			
	} else if( !cmpIgnoreCase( console->command, "test2" ) ) {
		dump = parseInt(console->args);
		newLine( console );
		printInt( console, asmtest2(dump));
		printStr(console, "\nSUCCESS!");
	} else if( !cmpIgnoreCase( console->command, "prog1" ) ) {
		activate( 1 );
	} else if( !cmpIgnoreCase( console->command, "prog2" ) ) {
		activate( 2 );
	} else if( !cmpIgnoreCase( console->command, "prog3" ) ) {
		activate( 3 );
	} else if( !cmpIgnoreCase( console->command, "prog4" ) ) {
		activate( 4 );
	} else if( !cmpIgnoreCase( console->command, "prog5" ) ) {
		activate( 5 );
	} else if( len( console->command ) > 0 ) { /*if not empty function*/
		cpy( console->command, temp ); /*return actual input*/
		printStr(console, "\n       \"");
		printStr( console, console->command );
		printStr( console, "\" is not a valid function. \n       Enter \"help\""
			  " for all possible commands, woof woof woof. - Cerb"
			  "erOS" );
	}
	
	console->keyBuffer[0] = 0;
	
	if( cmpIgnoreCase( console->command, "marquee" ) ) {
		/*if screen wasn't cleared*/
		if( console->screen.i > 0 ) {
			newLine( console );
		}		
		printStr(console, "CerberOS>"); /*put shell*/
		console->screen.shellRow = console->screen.i / 160 + 1; /*update shell row*/
	}
}

/***
	test function for function frames
	Parameter:
		x - value to be passed
***/
void test2() {

	printStr( console, "\nI am test 2 now.\n\n");
	
	printStr( console, "CerberOS>" ); /*display shell*/
	
	console->keyBuffer[0] = '\0';

	while( 1 ) { /*infinite loop for processing*/
		if( switchNow && getSwitcher() != getMainProc() ) {
			switchNow = 0;
			switchTo( getSwitcher() );
		} else if( console->processNow ) { /*if command is ready to process*/
			console->processNow = 0; /*reset processing flag*/
			processCmd(); /*process command*/
		}
	}
}

/***
	test function for function frames
	Parameter:
		x - value to be passed
***/
void test( int *stack, int x ) {

	x++;
	newLine( console );
	printInt(console, x);
	*stack = (int)test2;
}

/***
	handles timer functino
***/
void systemTimer( int *returnLoc, registers *regs ) {
	
	outb( 0x20, 0x20 );
	
	updateFunc( returnLoc, regs );
	
	frozenSong();
	updateMarquees();	

	timerCtr++; /*increment counter*/	
}

/***
	processes shell input
***/
void shellIn() {

	char c; /*character to be read*/
	process *p;
	
	outb( 0x20, 0x20 );

	p = getMainProc();
	c = getChar(); /*get a character*/
	
	if( c == '\t' && alt  ) { /*if alt + tab*/
		if( getMainProc() != getSwitcher() ) {
			switchTo( getMainProc()->next );
		}
	} else if( c == 'c' && ctrl ) {
		if( getMainProc() != console && getMainProc() != getSwitcher() ) {
			deact = 1;
		}
	} else if( c == '`' || c == '~' ) {
		if( getMainProc() != getSwitcher() ) {
			switchNow = 1;
		}
	} else if( c == '\b' && ( p->cmdIndex > 0 ) 
				|| c != '\n' && c != '\b' && c != '\0' ) { /*if backspace and 
									 cursor is beyond shell or 
									 row is beyond shellRow or if not a newline 
									 and not a backspace and not null*/				
		putChar( p, c );
		appendCmd( p, c ); /*append character to buffer*/
		setCursor();
	} else if( c == '\n') { /*if newline*/
		appendCmd( p, '\0'); /*end command*/
		p->processNow = 1;
		p->cmdIndex = 0; /*reset index*/
	}
}

/***
	shows the shell and handles shell input
***/
void shell() {

	console = initProcesses();

	/*initialize interrupts and keyboard*/
	idt_init();
	irq_init();

	showDoge(2000); /*show splash screen*/

	while( getChar() != '\0' );

	clrscr(console); /*clear screen*/

	printStr( console, "CerberOS>" ); /*display shell*/
	console->screen.shellRow = console->screen.i / 160 + 1;	/*get shell row*/

	while( 1 ) { /*infinite loop for processing*/
		if( switchNow && getSwitcher() != getMainProc() ) {
			switchNow = 0;
			switchTo( getSwitcher() );
		}
		
		if( deact ) {
			deact = 0;
			deacAsm( getMainProc(), console->eip );
		}
		
		if( console->processNow ) { /*if command is ready to process*/
			console->processNow = 0; /*reset processing flag*/
			processCmd(); /*process command*/
		}
	}

}
