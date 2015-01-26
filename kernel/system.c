#define VID_DATA_SIZE 2

extern int i; /*current screen position*/

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
	for( i = 0; i < msec * 20000; i++ );
}


