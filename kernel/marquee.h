unsigned char marqueeOffset;

/***
	creates new marquee
	Parameters:
		message - message to marquee
		row - row to display marquee on
***/
void newMarquee( char *message, unsigned char row );

/***
	move marquees
***/
void updateMarquees();
