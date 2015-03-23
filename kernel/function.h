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
	unsigned int i; /*screen index*/
	unsigned int j; /*next row zero-based*/
	unsigned int shellRow;
	char screen[4000];
} screen;

typedef struct process {
	unsigned char processNow;
	unsigned char activate;
	unsigned char isActive;
	unsigned char isStarted;
	unsigned char isMain;
	unsigned char shellLength;
	unsigned int cmdIndex;
	unsigned int eip;
	struct process *next;
	char name[8];
	char command[11];
	registers reg;
	char args[501];
	char keyBuffer[513];
	int frame[1024];
	screen screen;
} process;

typedef struct {
	struct process procs[PROC_COUNT + 2];
	process *curr;
	process *next;
} arrProcesses;

/***
	initializes functions array
***/
process *initProcesses();

/***
	switches between processes
***/
void switchProc();


process *getMainProc();

process *getSwitcher();

/***
	updates the function to be executed
	Parameters:
		returnLoc - points to the eip to return to
		regs - pointer to registers
***/
void updateFunc( unsigned int *returnLoc, registers *regs );


