#include "function.h"

arrProcesses aProcesses;

unsigned char mainIndex = 0;
unsigned char funcInit = 0;

void prog1(process *);
void prog2(process *);
void prog3(process *);
void prog4(process *);
void prog5(process *);

extern void kmain();

void func1() {
	
	char c = 'A';
	int i = 0;
	while(1) {
		printStr(aProcesses.procs, "X");
		sleep(100);
	}
}

void func2() {
	
	char c = 0;
	int i = 0;
	while(1) {
		printStr(aProcesses.procs, "O");
		sleep(100);
	}
}

void initProc( process *proc, unsigned int eip ) {
	
	int i = 0;

	proc->frame[1023] = (unsigned int)kmain;
	proc->eip = eip;
	while( i < VID_COLS * VID_ROWS ) {
		proc->screen.screen[i++] = '\0';
		proc->screen.screen[i++] = 0x07;
	}
	
	proc->isStarted = 0;
	proc->isMain = 0;
	proc->screen.keyBuffer[0] = '\0';
	proc->screen.command[0] = '\0';
	proc->screen.args[0] = '\0';
	proc->screen.i = 0;
	proc->screen.j = 1;
	proc->isActive = 1;
}

void prog1(process *proc ) {

}

void prog2(process *proc ) {

}

void prog3(process *proc ) {

}

void prog4(process *proc ) {

}

void prog5(process *proc ) {

}

/***
	initializes processes array
***/
process *initProcesses() {
	
	initProc( aProcesses.procs, (unsigned int)kmain );
	aProcesses.procs->isMain = 1;
	aProcesses.procs->isStarted = 1;
	aProcesses.nIndex = 0;
	aProcesses.prevIndex = 0;
	aProcesses.nCtr = 1;
	return aProcesses.procs;
}

process *getMainProc() {

	return aProcesses.procs + mainIndex;
}

/***
	updates the process to be executed
	Parameters:
		returnLoc - points to the eip to return to
		regs - pointer to registers
***/
void updateFunc( int *returnLoc, registers *regs ) {
	
	process *f;
	
	/*store where execution stopped*/
	f = aProcesses.procs + aProcesses.prevIndex;
	f->eip = (unsigned int)*returnLoc;
	f->reg = *regs;
	
	/*get next process*/
	f = aProcesses.procs + aProcesses.nIndex;
	
	if( f->isStarted ) { /*if process has started*/
		*regs = f->reg; /*save registers*/
	} else { /*if process has not yet started*/
		/*place base at end of allocated space*/
		f->frame[1023] = (unsigned int)kmain;
		regs->ebp = (unsigned int)(f->frame + 1023);
		regs->esp = (unsigned int)(f->frame + 1020);
		f->isStarted = 1; /*mark as started*/
	}
	
	/*put next instruction in line*/
	*returnLoc = (int)f->eip;
	
	/*update previous index*/
	aProcesses.prevIndex = aProcesses.nIndex;
	
	/*update index*/
	aProcesses.nIndex = ( aProcesses.nIndex + 1 ) % aProcesses.nCtr;
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


