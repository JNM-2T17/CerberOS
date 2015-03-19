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

struct {
	function functions[3];
	int nIndex;
	int prevIndex;
	int nCtr;
} aFunctions;

unsigned char funcInit = 0;

extern void kmain();

void func1() {
	
	char c = 'A';
	int i = 0;
	while(1) {
		printStr("X");
		sleep(100);
	}
}

void func2() {
	
	char c = 0;
	int i = 0;
	while(1) {
		printStr("O");
		sleep(100);
	}
}

/***
	initializes functions array
***/
void initFunctions() {
	
	function f;
	f.isStarted = 0;
	f.eip = (unsigned int)func1;
	aFunctions.functions[0] = f;
	f.eip = (unsigned int)func2;
	aFunctions.functions[1] = f;
	aFunctions.nIndex = 0;
	aFunctions.prevIndex = 2;
	aFunctions.nCtr = 3;
}

/***
	updates the function to be executed
	Parameters:
		returnLoc - points to the eip to return to
		regs - pointer to registers
***/
void updateFunc( int *returnLoc, registers *regs ) {
	
	function *f;
	
	if( !funcInit ) { /*if functions aren't initialized yet*/
		initFunctions(); /*initialize functions*/
		funcInit = 1; /*set to initialized*/
	}
		
	/*store where execution stopped*/
	f = aFunctions.functions + aFunctions.prevIndex;
	f->eip = (unsigned int)*returnLoc;
	f->reg = *regs;
	
	/*get next function*/
	f = aFunctions.functions + aFunctions.nIndex;
	
	if( f->isStarted ) { /*if function has started*/
		*regs = f->reg; /*save registers*/
	} else { /*if function has not yet started*/
		
		/*place base at end of allocated space*/
		f->frame[1023] = (unsigned int)kmain;
		regs->ebp = (unsigned int)(f->frame + 1023);
		regs->esp = (unsigned int)(f->frame + 1020);
		f->isStarted = 1; /*mark as started*/
	}
	
	/*put next instruction in line*/
	*returnLoc = (int)f->eip;
	
	/*update previous index*/
	aFunctions.prevIndex = aFunctions.nIndex;
	
	/*update index*/
	aFunctions.nIndex = ( aFunctions.nIndex + 1 ) % aFunctions.nCtr;
}

/***
	transfers iret values to new stack
	Parameters:
		retLoc - new stack
		retVals - points to old values
***/
void fixInterrupt( interruptRets *retLoc, interruptRets *retVals ) {

	*retLoc = *retVals;
}


