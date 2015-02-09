#include "printer.h"
#include "interrupt.h"

#define VID_DATA_SIZE 2 /*bytes per video cell*/
#define BUFFER_SIZE 513 /*characters in command buffer*/
#define CMD_SIZE 11 /*command length*/

#define ADD 0 /*macro for addition in arith(int) function*/
#define SUB 1 /*macro for subtraction in arith(int) function*/
#define MUL 2 /*macro for multiplication in arith(int) function*/

extern unsigned int i; /*current screen position*/
extern unsigned int k; /*next row number*/
extern unsigned int shellRow; /*row number of shell onscreen*/
extern char *splash; /*splash screen*/
extern char *cmdList; /*command list*/

char keyBuffer[BUFFER_SIZE]; /*command buffer*/
char temp[BUFFER_SIZE]; /*dump string*/
char command[CMD_SIZE]; /*command*/
char args[BUFFER_SIZE - CMD_SIZE]; /*arguments for command*/
unsigned int cmdIndex; /*index to place character in command*/

unsigned char shellStart = 1;
unsigned int i = 0; /*basic video index*/
unsigned int k = 1; /*next line index zero-based*/
char *vidPtr = (char *)VID_PTR; /*global pointer to video portion in memory*/
unsigned int shellRow; /*row on screen where the current shell is printed*/

#define ANNA 0
#define ELSA 1
#define LET_IT_GO_CTR 55
#define SNOWMAN_CTR 11

typedef char songline[55];
songline letItGo[LET_IT_GO_CTR] = {"The snow blows white on the mount tonight\n",
				"Not a footprint to be seen\n",
				"A kingdom of isolation\n",
				"And it looks like I'm the queen\n",
				"The wind is howling like this swirling storm inside\n",
				"Couldn't keep it in\n",
				"Heaven knows I've tried\n",
				"Don't let them in\n",
				"Don't let them see\n",
				"Be the good girl you always have to be\n",
				"Conceal don't feel\n",
				"Don't let them know\n",
				"Well now they knoooooow\n",
				"Let it go\n",
				"Let it go\n",
				"Can't hold it back anymore\n",
				"Let it go\n",
				"Let it go\n",
				"Turn away and the slam the door\n",
				"I don't care what they're going to say\n",
				"Let the storm rage on\n",
				"The cold never bothered me anyway\n",
				"It's funny how some distance\n",
				"Makes everything seem small\n",
				"And the fears that once controlled me\n",
				"Can't get to me at aaaaaaaall\n",
				"It's time to see what I can do\n",
				"To test my limits and break through\n",
				"No right, no wrong\n",
				"No rules for me\n",
				"I'm free\n",
				"Let it go\n",
				"Let it go\n",
				"I am one with the wind and sky\n",
				"Let it go\n",
				"Let it go\n",
				"They'll never see me cry\n",
				"Here I stand\n",
				"And here I'll stay\n",
				"Let the storm rage on\n",
				"My power flurries through the air into the ground\n",
				"My soul is spiraling in frozen fractals all around\n",
				"And one thought crystallizes like an icy blasy\n",
				"I'm never going back\n",
				"The past is in the past\n",
				"Let it go\n",
				"Let it go\n",
				"I will rise the break of dawn\n",
				"Let it go\n",
				"Let it go\n",
				"That perfect girl is gone\n",
				"Here I stand\n",
				"In the light of day\n",
				"Let the storm rage on\n",
				"The cold never bothered me anyway\n"
				};
songline snowMan[SNOWMAN_CTR] = {"*knock**knock**knock**knock**knock**knock*\n",
				"Do you wanna build a snowman\n",
				"Come on let's go and play\n",
				"I never see you anymore\n",
				"Come out the door\n",
				"It's like you've gone awaaaaaay\n",
				"We used to be best buddies\n",
				"And now we're not\n",
				"I wish you would tell me whyyyyy\n",
				"Do you want to build a snowman\n",
				"I doueshnt have ter ber a snermern\n"};

unsigned char isAnnaSinging = 1;
unsigned char isElsaSinging = 1;
unsigned char singer = ANNA;
unsigned int timerCtr = 0;
unsigned int elsaCtr = 0;
unsigned int annaCtr = 0;

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
	for( i = 0; i < msec * 20000; i++ ); /*loop for approximate time given in 
										   input*/
}

/***
	shows splash screen
***/
void showDoge() {

	clear();
	printStrColor( splash ); /*show doge*/
	sleep(4000);
	clear();
}

/***
	fixes the command extracted from video memory
***/
void *fixCmd() {
	
	int i = 0; /*counters*/
	
	/*eliminate leading whitespace*/
	while( keyBuffer[i] == ' ' ) { /*while whitespace*/
		i++;
	}

	cpy( temp, keyBuffer + i ); /*copy command without leading whitespace*/
	cpy( keyBuffer, temp ); /*copy to buffer*/
}

/***
	gets a command from the command line
***/
void getCmd() {

	fixCmd(); /*eliminate whitespace*/
	
	int nCtr = 0; /*counter*/

	/*while not at end of command, until a space is found*/
	while( keyBuffer[nCtr] != ' ' && keyBuffer[nCtr] != '\0' ) {
		nCtr++;
	}

	/*if has arguments*/
	if( keyBuffer[nCtr] == ' ' ){
		keyBuffer[nCtr] = '\0'; /*end command*/
		nCtr++; /*next char*/
		while( keyBuffer[nCtr] == ' ' ) { /*disregard whitespace*/
			nCtr++;
		}
		cpy( args, keyBuffer + nCtr );
		
		/*remove trailing spaces*/
		nCtr = len( args ) - 1; /*get last char index*/
		while( args[nCtr] == ' ' ) { /*while space*/
			nCtr--; /*prev char*/
		}
		args[ nCtr + 1 ] = '\0';
	} else { /*empty arguments*/
		cpy( args, "" );
	}
	cpy( command, keyBuffer );
	command[CMD_SIZE] = '\0';
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
int arithIsValid() {
	
	int i, /*counter*/
	    valid = 1, /*whether args is valid*/ 
	    argCtr = 0;	/*number of arguments*/
	
	for( i = 0; valid && i <= len( args ); i++ ) { /*check if args are valid*/
		if( args[i] == ' ' || args[i] == '\0' ) { /*if delimiter*/
			argCtr++; /*increment counter*/
			if( argCtr > 2 ) { /*if more than 2 args*/
				valid = 0; /*invalid*/
			}
			if( args[i] == ' ' ){ /*if not end of args*/
				do {
					i++; /*next char*/
				} while( args[i] == ' ' ); /*until next argument*/
				i--; /*step back for increment*/
			}
		} else if( args[i] - '0' < 0 || args[i] - '0' >= 10 ) { /*if not digit*/
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
int getArg2Index() {

	int i = 0; /*counter*/

	while( args[i] - '0' >= 0 && args[i] - '0' < 10 ) { /*while digit*/
		i++; /*next char*/
	}
	while( args[i] == ' ' ) { /*while whitespace*/
		i++; /*next char*/
	}
	
	return i;
}

/***
	displays the sum of the integers in the args buffer
***/
void arith( int oper ) {
	
	int res; /*result*/

	if( arithIsValid() ) { /*if valid arguments*/
		res = parseInt( args ); /*get first argument*/
		i = getArg2Index(); /*get next argument's index*/
		switch( oper ) { /*determine operation*/
			case ADD:
				res += parseInt( args + i ); /*add*/
				break;
			case SUB:
				res -= parseInt( args + i ); /*subtract*/
				break;
			default:
				res *= parseInt( args + i ); /*multiply*/
				break;
		}

		/*display result*/
		newLine();
		printInt( res );
	} else { /*if invalid*/
		printStr("\nPlease input two non-negative integers"); /*display error*/
	}
}

/***
	processed command
***/
void process() {

	getCmd(); /*get command*/

	cpy( temp, command ); /*copy command because cmpIgnoreCase converts it to 
						    lowercase*/

	if( !cmpIgnoreCase( command, "cls" ) ) {
		clear(); /*clear screen*/
	} else if( !cmpIgnoreCase( command, "help" ) ) {
		printStr( cmdList ); /*show commands*/
	} else if( !cmpIgnoreCase( command, "woof" ) ) {
		showDoge();		
	} else if( !cmpIgnoreCase( command, "say" ) ) {
		newLine(); /*show argument*/
		printStr( args );
	} else if( !cmpIgnoreCase( command, "add" ) ) {
		arith( ADD );/*add arguments*/
	} else if( !cmpIgnoreCase( command, "sub" ) ) {
		arith( SUB ); /*subtract arguments*/
	} else if( !cmpIgnoreCase( command, "mul" ) ) {
		arith( MUL ); /*multiply arguments*/
	} else if( !cmpIgnoreCase( command, "goAway" ) ) {
		if( !cmpIgnoreCase( args, "anna" ) ) {
			printStr("\n\n\nokay bye.....\n\n\n");
			isAnnaSinging = 0;
		} else if( !cmpIgnoreCase( args, "elsa" ) ) {
			printStr("\n\n\nThe cold never bothered me anyway.....\n\n\n");
			isElsaSinging = 0;
		}	
	} else if( !cmpIgnoreCase( command, "hey" ) ) {
		if( !cmpIgnoreCase( args, "anna" ) ) {
			if( isAnnaSinging ) {
				printStr( "\n\nAnna: I'm already singing!\n\n" );
			} else {
				isAnnaSinging = 1;
				annaCtr = 0;
				clear();
			}
		} else if( !cmpIgnoreCase( args, "elsa" ) ) {
			if( isElsaSinging ) {
				printStr( "\n\nElsa: I'm already singing!\n\n" );
			} else {
				isElsaSinging = 1;
				elsaCtr = 0;
				clear();
			}
		}
	} else if( len( command ) > 0 ) { /*if not empty function*/
		cpy( command, temp ); /*return actual input*/
		printStr("\n       \"");
		printStr( command );
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
	appends a character to the command
***/
void appendCmd( char c ) {

	if( c == '\b' && cmdIndex > 0 ) { /*if backspace and buffer is not empty*/
		cmdIndex--;
	} else if( c != '\b' ) { /*if not backspace*/
		keyBuffer[cmdIndex] = c; /*set character*/
		cmdIndex++; /*next index*/
	}
}

/***
	handles timer functino
***/
void systemTimer() {
	
	outb( 0x20, 0x20 );
	if( timerCtr % ( 20 / ( isAnnaSinging + isElsaSinging + 1 ) ) == 0 ) {
		if( singer == ANNA && isAnnaSinging) {
			printStr(snowMan[annaCtr % SNOWMAN_CTR]);
			annaCtr++;
		} else if( singer == ELSA && isElsaSinging ) {
			printStr(letItGo[elsaCtr % LET_IT_GO_CTR]);
			elsaCtr++;
		}
		singer = 1 - singer;
	}
	timerCtr++;
}

/***
	processes shell input
***/
void shellIn() {

	char c; /*character to be read*/

	outb( 0x20, 0x20 );

	c = getChar(); /*get a character*/

	/*if backspace and cursor is beyond shell or row is beyond shellRow or 
	  if not a newline and not a backspace and not null*/
	if( c == '\b' && ( i % 160 >= 20 || k > shellRow ) || c != '\n' && 
		c != '\b' && c != '\0' ) {
		putChar(c); /*put character onscreen*/
		appendCmd(c); /*append character to buffer*/
		setCursor();
	
	} else if( c == '\n') { /*if newline*/
		appendCmd('\0'); /*end command*/
		process(); /*process command*/
		cmdIndex = 0; /*reset index*/
	}
}

/***
	shows the shell and handles shell input
***/
void shell() {

	showDoge(); /*show splash screen*/

	/*initialize interrupts and keyboard*/
	idt_init();
	kb_init();

	while( getChar() != '\0' );

	clear(); /*clear screen*/

	printStr( "CerberOS>" ); /*display shell*/
	shellRow = i / 160 + 1;	/*get shell row*/

	/*initialize command*/
	cmdIndex = 0;
	keyBuffer[0] = '\0';

	while( 1 ); /*infinite loop for processing*/
}
