typedef struct {
	int frame[1000];
	unsigned int eip;
	unsigned int ebp;
	unsigned int esp;
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
		ebp - ebp of source function frame
		esp - esp of source function frame
***/
int *updateFunc( int *returnLoc, int ebp, int esp ) {
	
	function *f;
	
	if( !funcInit ) {
		initFunctions();
	}
		
	/*store where execution stopped*/
	if( funcInit ) {
		f = aFunctions.functions + aFunctions.prevIndex;
		f->eip = (unsigned int)*returnLoc;
		f->ebp = ebp;
		f->esp = esp;
	}

	/*printHex((int)(func1) - *returnLoc);
	newLine();
	printHex((int)(func2) - *returnLoc);
	newLine();*/

	/*set functions to initialized*/
	if( !funcInit ) {
		funcInit = 1;
	}
		
	/*get next function*/
	f = aFunctions.functions + aFunctions.nIndex;
	
	if( f->isStarted ) { /*if function has started*/
		ret[0] = 1; /*flag is positive*/
		ret[1] = f->ebp; /*restore function frame*/
		ret[2] = f->esp; 

		/*printStr("Flag:");
		printHex( ret[0] );
		printStr("  EBP:");
		printHex( ret[1] );
		printStr("  ESP:");
		printHex( ret[2] );
		printStr("  EIP:");
		printHex( f->eip );
		newLine();*/

	} else { /*if function has not yet started*/
		ret[0] = 0; /*flag is negative*/
		
		/*place base at end of allocated space*/
		ret[1] = (unsigned int)(f->frame + 1000);
		f->isStarted = 1; /*mark as started*/

		/*printStr("Flag:");
		printHex( ret[0] );
		printStr("  EBP:");
		printHex( ret[1] );
		printStr("  EIP:");
		printHex( f->eip );
		newLine();
		sleep(5000);*/
	}
	
	/*put next instruction in line*/
	*returnLoc = (int)f->eip;
	
	/*update previous index*/
	aFunctions.prevIndex = aFunctions.nIndex;
	
	/*update index*/
	aFunctions.nIndex = ( aFunctions.nIndex + 1 ) % aFunctions.nCtr;
	
	return ret;
}


