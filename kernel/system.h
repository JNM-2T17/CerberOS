extern VID_DATA_SIZE;
extern unsigned int i; /*current screen position*/

/***
	calls the assembly instruction outb
	Parameters:
		_port - hardware port to push data to
		_data - data to push to port
***/
void outb (unsigned short _port, unsigned char _data);

/***
	sets cursor location to where i is at.
***/
void setCursor();

/***
	sends the system to sleep for approximately 2 seconds
***/
void sleep();


