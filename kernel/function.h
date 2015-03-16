typedef struct {
	int frame[250];
	unsigned int eip;
	unsigned int ebp;
	unsigned int esp;
	char isStarted;
} function;

/***
	initializes functions array
***/
void initFunctions();

/***
	updates the function to be executed
	Parameters:
		returnLoc - points to the eip to return to
		ebp - ebp of source function frame
		esp - esp of source function frame
***/
int *updateFunc( int *returnLoc, int ebp, int esp );


