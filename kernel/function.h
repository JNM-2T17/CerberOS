typedef struct {
	unsigned int eip;
	unsigned int cs;
	unsigned int flags;	
} interruptRets;

typedef struct {
	unsigned int edi;
	unsigned int esi;
	unsigned int ebp;
	unsigned int esp;
	unsigned int ebx;
	unsigned int edx;
	unsigned int ecx;
	unsigned int eax;	
} registers;

typedef struct {
	int frame[1024];
	unsigned int eip;
	registers reg;
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
		regs - pointer to registers
***/
void updateFunc( int *returnLoc, registers *regs );


