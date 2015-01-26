extern VID_DATA_SIZE;
extern unsigned int i; /*current screen position*/
extern unsigned int k; /*next row number*/
extern unsigned int shellRow; /*row number of shell onscreen*/

/***
	calls the assembly instruction outb
	Parameters:
		_port - hardware port to push data to
		_data - data to push to port
***/
void outb (unsigned short _port, unsigned char _data);

/***
	calls the assembly instruction inb
	Parameters:
		_port - hardware port to get data from
	Returns:
		data received from port
***/
unsigned char inb (unsigned short _port);

/***
	sets cursor location to where i is at.
***/
void setCursor();

/***
	sends the system to sleep
	Parameter:
		msec - approx no. of msec
***/
void sleep( unsigned int msec );

/***
	shows the shell and handles shell input
***/
void shell();
