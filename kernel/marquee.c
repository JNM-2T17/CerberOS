#define LEFT 0 /*macro for going left*/
#define RIGHT 1 /*macro for going right*/

#define SCREEN_WIDTH 80 /*characters per row*/ 
#define BYTES_PER_CELL 2 /*bytes in a cell onscreen*/
#define MARQUEE_CTR 12 /*number of marquees in screen*/

/*structure for a marquee*/
typedef struct {
	char row; /*row the marquee is on*/
	char marquee[SCREEN_WIDTH]; /*content of marquee*/
	unsigned char direction; /*direction of marquee*/
	unsigned char new; /*whether the marquee is new*/
	unsigned char active; /*whether the marquee is active*/
}marquee;

marquee marquees[MARQUEE_CTR]; /*allocate twelve marquees*/

extern char *vidPtr; /*pointer to video memory*/
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
	for( n = 0, l = m->row * SCREEN_WIDTH * BYTES_PER_CELL; n < SCREEN_WIDTH; n++ ) {
		vidPtr[l++] = m->marquee[n]; /*place character*/
		vidPtr[l++] = color; /*place color*/
		color++; /*increment color*/
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
	}
}

/***
	creates new marquee
	Parameters:
		message - message to marquee
		row - row to display marquee on
***/
void newMarquee( char *message, unsigned char row ) {
	
	int l = 0; /*counter*/
	marquee *m; /*pointer to the marquee*/
	
	if( len( message ) == 0 ) { /*if no message*/
		printStr("\nWarning: No Message to Marquee");
	} else { 
		/*cycles through marquees; while current marquee is active
		  or not at the end yet*/
		while( marquees[l].active && l < MARQUEE_CTR ) {
			l++;
		}

		if( l == MARQUEE_CTR ) { /*if no available marquees*/
			printStr("\nError: Can't Create Marquee");
		} else {
			/*assign to pointer for easy access*/
			m = &marquees[l];
		
			/*if last row*/
			if( row == 25 ) {
				row = 23; /*set to second to last row*/
			}
			m->row = row; /*assign row*/
			cpy( m->marquee, message ); /*copy message*/

			/*pad message with spaces*/
			for( l = len( m->marquee); l <= SCREEN_WIDTH; l++ ) {
				m->marquee[l] = ' ';
			}
	
			m->direction = RIGHT; /*set initial direction*/
			m->new = 1; /*set as new*/
			m->active = 1; /*set as active*/
			newLine(); /*put on newline*/
		}	
	}
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
