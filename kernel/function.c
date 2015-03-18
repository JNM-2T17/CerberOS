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
	function functions[2];
	int nIndex;
	int prevIndex;
	int nCtr;
} aFunctions;

unsigned int ret[3];
unsigned char funcInit = 0;

void func1() {
	
	char c = 'A';
	int i = 0;
	while(1) {
		/*if( i % 50000 == 0 ) {
			/*putChar(c);
			setCursor();
			c++;
			if( c == 'Z' + 1 ) {
				c = 'A';
			}*/
			printStr("X");
		/*}
		i++;*/
	}
}

void func2() {
	
	char c = 0;
	int i = 0;
	while(1) {
		/*if( i % 50000 == 0 ) {
			/*printInt((int)c);
			c++;
			if( c > 9 ) {
				c = 0;
			}*/
			printStr("O");
		/*}
		i++;*/
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
	aFunctions.prevIndex = 0;
	aFunctions.nCtr = 2;
}

/***
	updates the function to be executed
	Parameters:
		returnLoc - points to the eip to return to
		regs - pointer to registers
***/
int updateFunc( int *returnLoc, registers *regs ) {
	
	function *f;
	int ret;
	
	if( !funcInit ) {
		initFunctions();
	}
		
	/*store where execution stopped*/
	if( funcInit ) {
		f = aFunctions.functions + aFunctions.prevIndex;
		f->eip = (unsigned int)*returnLoc;
		f->reg = *regs;
	}
	
	/*set functions to initialized*/
	if( !funcInit ) {
		funcInit = 1;
	}
		
	/*get next function*/
	f = aFunctions.functions + aFunctions.nIndex;
	
	if( f->isStarted ) { /*if function has started*/
		ret = 1; /*flag is positive*/
		*regs = f->reg; /*save registers*/
	} else { /*if function has not yet started*/
		ret = 0; /*flag is negative*/
		
		/*place base at end of allocated space*/
		f->reg.ebp = (unsigned int)(f->frame + 1023);
		f->isStarted = 1; /*mark as started*/
	}
	
	/*put next instruction in line*/
	*returnLoc = (int)f->eip;
	
	/*update previous index*/
	aFunctions.prevIndex = aFunctions.nIndex;
	
	/*update index*/
	aFunctions.nIndex = ( aFunctions.nIndex + 1 ) % aFunctions.nCtr;
	
	return ret;
}

void fixInterrupt( interruptRets *retLoc, interruptRets retVals ) {

	*retLoc = retVals;
}


