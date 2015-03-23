#include "function.h"

#define NULL 0

arrProcesses aProcesses;

unsigned char mainIndex = 0;
unsigned char funcInit = 0;

void prog1(process *);
void prog2(process *);
void prog3(process *);
void prog4(process *);
void prog5(process *);

extern void kmain();

process *switchP = aProcesses.procs + 21;

void func1() {
	
	char c = 'A';
	int i = 0;
	while(1) {
		printStr(aProcesses.procs + 1, "X");
		sleep(1);
	}
}

void func2() {
	
	char c = 0;
	int i = 0;
	while(1) {
		printStr(aProcesses.procs + 2, "O");
		sleep(1);
	}
}

void initProc( process *proc, char *name, unsigned int eip ) {
	
	int i = 0;

	proc->frame[1023] = (unsigned int)kmain;
	proc->eip = eip;
	while( i < VID_COLS * VID_ROWS * VID_DATA_SIZE ) {
		proc->screen.screen[i++] = '\0';
		proc->screen.screen[i++] = 0x07;
	}
	
	proc->activate = 1;
	proc->isStarted = 0;
	proc->isMain = 0;
	cpy( proc->name, name );
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

process *getMainProc() {

	return aProcesses.procs + mainIndex;
}

/***
	switches between processes
***/
void switchProc() {

	int i;
	process *activeList = aProcesses.procs,
			*currProc,
			*temp;
	activeList->isMain = 0;
	switchP->isMain = 1;
	mainIndex = 21;
	clrscr();	
		
	/*for each process*/
	for( i = 1, currProc = aProcesses.procs; i < 21; i++ ) {
	
		/*if active*/
		if( currProc[i].isActive ) {
			
			/*go to end of list*/
			for( temp = activeList; temp->next != NULL; temp = temp->next );
			
			/*attack process to list*/
			temp->next = &currProc[i];
		}
	}
	
	i = 1; /*reset counter*/
	temp = activeList; /*temp goes to start of list*/
	
	while( temp != NULL ) {
		putChar( switchP, '[' );
		printInt( switchP, i );
		printStr( switchP, "] - " );
		printStr( switchP, temp->name );
		newLine( switchP );
		i++;
		temp = temp->next;
	}
	printStr( switchP, "Enter a process no.: " );
	while(1);
}

/***
	initializes processes array
***/
process *initProcesses() {
	
	int i;
	process *p;
	
	for( i = 0; i < 22; i++ ) {
		p = aProcesses.procs + i;
		p->activate = 0;
		p->isStarted = 0;
		p->isMain = 0;
		p->isActive = 0;
		p->next = NULL;
	}
	
	initProc( aProcesses.procs, "console", (unsigned int)kmain );
	initProc( aProcesses.procs + 21, "switch", (unsigned int)switchProc );
	aProcesses.procs->activate = 0;
	aProcesses.procs->isMain = 1;
	aProcesses.procs->isStarted = 1;
	aProcesses.procs[21].activate = 0;
	aProcesses.nIndex = 0;
	aProcesses.prevIndex = 0;
	aProcesses.nCtr = 1;
	return aProcesses.procs;
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


