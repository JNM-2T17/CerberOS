#include "function.h"

#define NULL 0

arrProcesses aProcesses;

unsigned char mainIndex = 0;

void prog1(process *);
void prog2(process *);
void prog3(process *);
void prog4(process *);
void prog5(process *);

extern char *vidPtr;
extern void kmain();
extern void getCmd( process *);
extern void switchTo( process * );
extern void processRunner( process *proc, unsigned int *);
unsigned int parseInt( char * );
unsigned char procCtr;

process *console = aProcesses.procs;
process *switchP = aProcesses.procs + PROC_COUNT + 1;

void initProc( process *proc, char *name, unsigned int eip ) {
	
	int i = 0;
	
	if( proc != console ) {
		proc->reg.esp = (unsigned int)(proc->frame + 1019);
		proc->reg.ebp = (unsigned int)(proc->frame + 1022);
		proc->frame[1022] = (unsigned int)proc;
		proc->frame[1023] = eip;
		
		proc->eip = (unsigned int)processRunner;
	} else {
		proc->eip = eip;
	}
	
	while( i < VID_COLS * VID_ROWS * VID_DATA_SIZE ) {
		proc->screen.screen[i++] = '\0';
		proc->screen.screen[i++] = 0x07;
	}
	
	proc->isStarted = 0;
	proc->isMain = 0;
	proc->cmdIndex = 0;
	cpy( proc->name, name );
	proc->keyBuffer[0] = '\0';
	proc->command[0] = '\0';
	proc->args[0] = '\0';
	proc->screen.i = 0;
	proc->screen.j = 1;
	proc->isActive = 1;
}

/***
	one preprogrammed process
	Parameter:
		proc - process this function is attached to
***/
void prog1( process *proc ) {
	
	char c = 'A';
	int i = 0;
	while( i < 800 ) {
		putChar( proc, c );
		printInt( proc, i );
		c++; i++;
		sleep(10);
		if( proc->processNow ) {
			proc->processNow = 0;
			c = 'A';
			i = 0;
			clrscr( proc );
		}
	}
}

/***
	one preprogrammed process
	Parameter:
		proc - process this function is attached to
***/
void prog2( process *proc ) {
	
	char c = 0;
	int i = 0;
	while( 1 ) {
		putChar(proc, c );
		printInt( proc, i );
		c--; i--;
		sleep(10);
		if( proc->processNow ) {
			proc->processNow = 0;
			c = 'A';
			i = 0;
			clrscr( proc );
		}
	}
}

/***
	one preprogrammed process
	Parameter:
		proc - process this function is attached to
***/
void prog3(process *proc ) {

	printStr( proc, "This is prog3..." );
	sleep( 400 );
}

/***
	one preprogrammed process
	Parameter:
		proc - process this function is attached to
***/
void prog4(process *proc ) {

	printStr( proc, "This is prog4..." );
	sleep( 400 );
}

/***
	one preprogrammed process
	Parameter:
		proc - process this function is attached to
***/
void prog5(process *proc ) {

	printStr( proc, "This is prog5..." );
	sleep( 400 );
}

/***
	returns current main process
***/
process *getMainProc() {

	return aProcesses.procs + mainIndex;
}

/***
	checks if args are numerical
	Returns:
		1 if valid, 0 otherwise
***/
int argsCheck() {
	
	int i = 0; /*counter*/
	char *temp = switchP->keyBuffer; /*gets switcher's key buffer*/
	
	while( temp[i] == ' ' && temp[i] != '\0' ) {
		i++;
	}
	
	if( temp[i] == '\0' ) {
		return 0;
	}
	
	cpy( temp, temp + i );
	
	i = 0;
	
	while( temp[i] != '\0' ) {
		if( temp[i] < '0' || temp[i] > '9' ) {
			return 0;
		}
		i++;
	}
	
	
	return 1;
}

process *getSwitcher() {
	return switchP;
}

int getProcCtr() {
	
	int i = 1;
	process *temp = console;
	
	do {
		i++;
		temp = temp->next;
	} while( temp != console );
	
	return --i;
}

void displayRunningProcs() {
	
	int i = 1;
	process *temp = console;
	
	do {
		putChar( switchP, '[' );
		printInt( switchP, i );
		printStr( switchP, "] - " );
		printStr( switchP, temp->name );
		newLine( switchP );
		i++;
		temp = temp->next;
	} while( temp != console );
	
	printStr( switchP, "Enter a process no.: " );
	switchP->screen.shellRow = switchP->screen.i / 160 + 1; /*update shell row*/
}

void linkProcs() {

	int i;
	process *activeList = console,
			*currProc,
			*temp;
			
	console->next = console;
			
	/*for each process*/
	for( i = 1, currProc = console; i < PROC_COUNT + 1; i++ ) {
		
		currProc[i].next = NULL;
		currProc[i].prev = NULL;
		
		/*if active*/
		if( currProc[i].isActive ) {
			/*go to end of list*/
			for( temp = activeList; temp->next != console; temp = temp->next );
		
			/*attack process to list*/
			temp->next = &currProc[i];
			currProc[i].prev = temp;
			currProc[i].next = console;
			console->prev = &currProc[i];
		}
	}
}

void deactivate( unsigned int *returnLoc, process *proc ) {

	process *temp;
	
	proc->isActive = 0;
	switchTo( NULL );
	linkProcs();
	/*if( aProcesses.curr == proc ) {
		temp = aProcesses.next;
		
		/*update previous index*/
		/*aProcesses.curr = aProcesses.next;*/
	
		/*printStr( getMainProc(), aProcesses.curr->name );
		newLine( getMainProc() );*/
	
		/*if( temp->next == console && temp != switchP && switchP->isMain ) {
			aProcesses.next = switchP;
		} else {
			aProcesses.next = temp->next;
		}
	} else if( aProcesses.next == proc ) {
		aProcesses.next = temp->next;
	}*/
	*returnLoc = console->eip;
}

/***
	switches between processes
***/
void switchProc() {

	int i,
		input;
	process *activeList = aProcesses.procs,
			*temp;
	
	linkProcs();
	clrscr( switchP );
	procCtr = getProcCtr();
	displayRunningProcs();
	
	do {
		if( switchP->processNow ) {
			switchP->processNow = 0;
			
			if( !argsCheck() ) {
				clrscr( switchP );
				printStr( switchP, "Please enter a number\n" );
				procCtr = getProcCtr();
				displayRunningProcs();
			} else {
				input = parseInt( switchP->keyBuffer );
			
				if( input < 1 || input > procCtr ) {				
					clrscr( switchP );
					printStr( switchP, "Please enter a number from 1 to " );
					printInt( switchP, procCtr );
					newLine( switchP );
					displayRunningProcs();
				} else {
					temp = activeList;
					for( i = 1; i < input; i++ ) {
						temp = temp->next;
					}
					switchTo( temp );
				}
			}
		}
	} while( 1 );
}

void switchMain( process *proc ) {

	int i;
	
	outb( 0x20, 0x20 );

	if( proc != getMainProc() ) {
	
		if( proc == NULL ) {
			proc = console;
		}	
	
		proc->isMain = 1;
		getMainProc()->isMain = 0;
		for( i = 0; i < PROC_COUNT + 2; i++ ) {
			if( proc == &aProcesses.procs[i] ) {
				mainIndex = i;
				break;
			}
		}
		
		if( proc == switchP ) {
			clrscr( switchP );
			linkProcs();	
			displayRunningProcs();
			procCtr = getProcCtr();
		} else {
			switchP->eip = (unsigned int)switchProc;
			for( i = 0; i < 4000; i++ ) {
				vidPtr[i] = proc->screen.screen[i];
			}
		}
		setCursor();
	}
}

/***
	initializes processes array
***/
process *initProcesses() {
	
	int i;
	process *p;
	
	for( i = 0; i < PROC_COUNT + 2; i++ ) {
		p = aProcesses.procs + i;
		p->processNow = 0;
		p->isStarted = 0;
		p->isMain = 0;
		p->isActive = 0;
	}
	
	initProc( console, "console", (unsigned int)kmain );
	initProc( console + 1, "prog1", (unsigned int)prog1 );
	initProc( console + 2, "prog2", (unsigned int)prog2 );
	initProc( console + 21, "switch", (unsigned int)switchProc );
	console->next = console;
	console->prev = console;
	switchP->next = console;
	console->isMain = 1;
	console->isStarted = 1;
	aProcesses.curr = console;
	aProcesses.next = console;
	return console;
}

void newProc( int i ) {
	
	int j;
	
	if( getProcCtr() == 21 ) {
		newLine( getMainProc() );
		printStr( getMainProc(), "Too Many Processes" );
	} else {
		for( j = 1; j < 22; j++ ) {
			if( !aProcesses.procs[j].isActive ) {
				switch( i ) {
					case 1:
						initProc( aProcesses.procs + j, "prog1", 
									(unsigned int)prog1 );
						break;
					case 2:
						initProc( aProcesses.procs + j, "prog2", 
									(unsigned int)prog2 );
						break;
					case 3:
						initProc( aProcesses.procs + j, "prog3", 
									(unsigned int)prog3 );
						break;
					case 4:
						initProc( aProcesses.procs + j, "prog4", 
									(unsigned int)prog4 );
						break;
					case 5:
						initProc( aProcesses.procs + j, "prog5", 
									(unsigned int)prog5 );
						break;
					default:
						break;
				}
				break;
			}
		}
		linkProcs();
		switchTo( aProcesses.procs + j );
	}
}

/***
	updates the process to be executed
	Parameters:
		returnLoc - points to the eip to return to
		regs - pointer to registers
***/
void updateFunc( unsigned int *returnLoc, registers *regs ) {
	
	process *f;
	
	linkProcs();
	
	/*store where execution stopped*/
	f = aProcesses.curr;
	f->eip = *returnLoc;
	f->reg = *regs;
	
	/*get next process*/
	f = aProcesses.next;
	
	if( f->isStarted ) { /*if process has started*/
		*regs = f->reg; /*save registers*/
	} else { /*if process has not yet started*/
	
		/*place base at end of allocated space*/
		regs->esp = f->reg.esp;
		regs->ebp = f->reg.ebp;
		
		/*mark as started*/
		f->isStarted = 1; 
	}
	
	/*put next instruction in line*/
	*returnLoc = f->eip;
	
	/*update previous index*/
	aProcesses.curr = aProcesses.next;
	
	/*printStr( getMainProc(), aProcesses.curr->name );
	newLine( getMainProc() );*/
	
	if( f->next == console && f != switchP && switchP->isMain ) {
		aProcesses.next = switchP;
	} else {
		aProcesses.next = f->next;
	}
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


