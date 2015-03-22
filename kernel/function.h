#define PROC_COUNT 20

#define VID_COLS 80 /*columns on the screen*/
#define VID_ROWS 25 /*rows on the screen*/
#define VID_DATA_SIZE 2 /*bytes per cell on screen*/

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
	char screen[2000];
	char keyBuffer[513];
	char command[11];
	char args[501];
	int i; /*screen index*/
	int j; /*next row zero-based*/
	int shellRow;
} screen;

typedef struct process {
	unsigned char isActive;
	unsigned char isStarted;
	unsigned char isMain;
	int frame[1024];
	unsigned int eip;
	registers reg;
	screen screen;
} process;

typedef struct {
	struct process procs[PROC_COUNT + 1];
	int nIndex;
	int prevIndex;
	int nCtr;
} arrProcesses;

/***
	initializes functions array
***/
process *initProcesses();

process *getMainProc();

/***
	updates the function to be executed
	Parameters:
		returnLoc - points to the eip to return to
		regs - pointer to registers
***/
void updateFunc( int *returnLoc, registers *regs );


