#include "function.h"

#define LEFT 0 /*macro for going left*/
#define RIGHT 1 /*macro for going right*/

#define SCREEN_WIDTH 80 /*characters per row*/ 
#define BYTES_PER_CELL 2 /*bytes in a cell onscreen*/
#define MARQUEE_CTR 12 /*number of marquees in screen*/

/*options for marquee*/
#define OPTIONS "\n\n   -l makes a marquee going to the left\n   -r makes a marquee going to the right\n   -help displays possible modes\n"

/*structure for a marquee*/
typedef struct {
	char row; /*row the marquee is on*/
	char marquee[SCREEN_WIDTH]; /*content of marquee*/
	unsigned char direction; /*direction of marquee*/
	unsigned char new; /*whether the marquee is new*/
	unsigned char active; /*whether the marquee is active*/
	unsigned char endangered; /*top of the terminal*/
}marquee;

marquee marquees[MARQUEE_CTR]; /*allocate twelve marquees*/

extern char *vidPtr; /*pointer to video memory*/
extern unsigned char mainIndex;
extern process *console;
char marqueeOffset = 0; /*offset for all marquees*/

/***
	moves a marquee
***/
void moveMarquee( marquee *m ) {

	int l, n; /*counters*/
	unsigned char color;

	/*randomizes color*/
	color = ( (unsigned long)(m) & 0xff ) % 0x0f + 1;
	
	/*select direction*/
	switch( m->direction ) {
		case LEFT: /*if going left*/
			/*move everything left*/
			for( l = 1; l < SCREEN_WIDTH; l++ ) {
				m->marquee[l - 1] = m->marquee[l];
			}

			m->marquee[l - 1] = ' '; /*put a space at the right*/

			/*if already flushed to the left*/
			if( m->marquee[0] != ' ' ) {
				/*toggle direction*/
				m->direction = 1 - m->direction;
			}
			break;
		default: /*if going right*/
			/*move everything right*/
			for( l = SCREEN_WIDTH - 1; l > 0; l-- ) {
				m->marquee[l] = m->marquee[l - 1];
			}

			m->marquee[l] = ' '; /*put a space at the left*/

			/*if already flushed to the right*/
			if( m->marquee[SCREEN_WIDTH - 1] != ' ' ) {
				/*toggle direction*/
				m->direction = 1 - m->direction;
			}
	}	

	/*place marquee in row*/
	for( n = 0, l = m->row * SCREEN_WIDTH * BYTES_PER_CELL; n < SCREEN_WIDTH; 
			n++ ) {
		console->screen.screen[l++] = m->marquee[n];
		l++;
		
		if( mainIndex == 0 ) {
			vidPtr[l - 2] = m->marquee[n]; /*place character*/
			vidPtr[l - 1] = color++; /*place color*/
		}
		
		if( color == 0x10 ) { /*if next color*/
			color = 0x01; /*reset*/
		}
	}
}

/***
	adjusts a marquee
***/
void adjustMarquee( marquee *m ) {
	
	if( m->new ) { /*if new*/
		m->new = 0; /*set to old*/
	} else { /*if old*/
		m->row += marqueeOffset; /*apply offset*/
		if( m->row < 0 ) { /*if out of screen*/
			m->active = 0; /*set inactive*/
		}
		if( m->row < 2 ) { /*if almost inactive*/
			m->endangered = 1; /*set endangered*/
		}
	}
}

/***
	creates new marquee
	Parameters:
		args - arguments for the maruqee
		row - row to display marquee on
***/
void newMarquee( char *args, unsigned char row ) {
	
	int l = 0, n; /*counter*/
	marquee *m; /*pointer to the marquee*/
	unsigned char direction = RIGHT; /*marquee direction*/
	char *words; /*points to words to marquee*/

	outb( 0x20, 0x20 );

	if( len( args ) == 0 ) { /*if no args*/
		printStr( console, "\nWarning: No args to Marquee");
	} else { 
		/*if first argument is an option*/
		if( args[0] == '-' ) { /*if there is a mode*/

			/*while first argument*/
			while( args[l] != ' ' && args[l] != '\0' ) {
				l++;
			}
			args[l++] = '\0'; /*end argument*/
			
			/*if asking for options*/
			if( !cmpIgnoreCase( args, "-help" ) ) {
				printStr( console, OPTIONS ); /*display options*/
				return; /*exit*/
			} 

			/*if there are words to marquee*/
			if( l != len(args) ) {
				/*trim whitespace*/
				while( args[l] == ' ' && args[l] != '\0' ) {
					l++;
				}
				words = args + l; /*point to words to marquee*/

				/*if argument says for marquee to go left*/
				if( !cmpIgnoreCase( args, "-l" ) ){
					direction = LEFT; /*set direction*/

				/*if argument is invalid*/
				} else if( cmpIgnoreCase( args, "-r" ) ) {
					printStr( console, "\nUnrecognized flag \"-"); 
					printStr( console, args + 1 );
					printStr( console, 
							"\". \nType mode \"-help\" for possible modes.");
					return;
				}
			} else { /*if no words to marquee*/
				printStr( console, "\nWarning: No args to Marquee");
				return;
			}
		} else { /*if no mode*/
			words = args;
		}
		
		l = 0; /*reset counter*/

		/*cycles through marquees; while current marquee is active
		  or not at the end yet*/
		while( marquees[l].active && l < MARQUEE_CTR ) {
			l++;
		}

		if( l == MARQUEE_CTR ) { /*if no inactive marquees*/
			l = 0;
			while( !marquees[l].endangered && l < MARQUEE_CTR ) {
				l++;	
			}
		}

		if( l == MARQUEE_CTR ) { /*if no inactive or endangered marquees*/
			printStr( console, "\nError: Can't Create Marquee");
		} else {
			/*assign to pointer for easy access*/
			m = &marquees[l];
		
			/*if last row*/
			if( row == 25 ) {
				row = 23; /*set to second to last row*/
			}
			m->row = row; /*assign row*/
			printInt( console, m->row);
			if( direction == LEFT ) {
				for( l = 0; l < SCREEN_WIDTH - len(words); l++ ) {
					m->marquee[l] = ' ';
				}
				for( n = 0; l < SCREEN_WIDTH; l++, n++ ) {
					m->marquee[l] = words[n];
				}
			} else {
				cpy( m->marquee, words ); /*copy args*/

				/*pad args with spaces*/
				for( l = len( m->marquee); l <= SCREEN_WIDTH; l++ ) {
					m->marquee[l] = ' ';
				}
			}
	
			m->direction = direction; /*set initial direction*/
			m->new = 1; /*set as new*/
			m->endangered = 0; /*not endangered*/
			newLine( console ); /*put on newline*/
			m->active = 1; /*set as active*/
		}	
	}
	
	if( console->screen.i > 0 ) {
		newLine( console );
	}	
	printStr(console, "CerberOS>"); /*put shell*/
	console->screen.shellRow = console->screen.i / 160 + 1; /*update shell row*/
}

/***
	move marquees
***/
void updateMarquees() {
	
	int i; /*counter*/
	
	/*for each marquee*/
	for( i = 0; i < MARQUEE_CTR; i++ ) {
		/*adjust marquees*/
		adjustMarquee( marquees + i );
		
		/*if still onscreen*/
		if( marquees[i].active ) {
			moveMarquee( marquees + i ); /*move marquee*/
		}
	}
	marqueeOffset = 0; /*reset offset*/
}
