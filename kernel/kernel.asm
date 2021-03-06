bits 32 ;runs in 32-bit

section .text
	
	align 4 ;align the following double words
	dd 0x1BADB002 ;magic number
	dd 0x00 ;flags
	dd - ( 0x1BADB002 + 0x00 ) ;checksum, which when added to magic no. and 
							   ;flags returns zero

global load_idt ;loading idt is ALSO global
global shellProc ;calls C function for shellIn
global timer ;calls C function for timer
global start ;start is accesible from other object files
global asmtest ;hands on function frame exercise
global asmtest2 ;hands on function frame exercise
global shiftScr ;calls shift screen interrupt
global shiftHandler ;calls shift screen
global clrscr ;calls clear screen interrupt
global clrscr2 ;calls clear screen interrupt
global clearHandler ;calls C clear function
global switchTo ;switches to a process pointed to by the parameter
global switchHandler ;calls C switch main process function
global _newMarquee ;creates new marquee
global marqueeHandler ;calls new marquee function
global processRunner ;runs a process
global deacAsm ;deactivator function
global deac ;deactivator interrupt
global activate
global activateAsm ;activator interrupt
global strIn ;scans a string
global printAsm
global strOut
global intOut
global hexOut
global charOut
extern kmain ;kmain is an external function
extern shellIn
extern systemTimer
extern shiftScreen
extern clear
extern switchMain
extern newMarquee
extern deactivate
extern newProc
extern test
extern test2
extern fixInterrupt
extern getCurrProc
extern scanStr
extern printHandler

asmtest:
	mov ebx, [esp + 4] ;get argument
	push ebx ;push argument
	mov ebx, esp ;get stack pointer
	sub ebx, 8 ;get value of esp that will point to eip after function call
	push ebx ;push eip location after function call
	call test ;call c function

asmtest2:
	mov eax, [esp + 4]
	mul eax
	pop esi
	push test2
	ret

load_idt:
	mov edx, [esp+4]
	lidt [edx]
	sti
	ret

timer:
	pusha ;push registers
	push esp ;push location of registers
	mov ebx, esp ;get stack
	add ebx, 36 ;move beyond registers to eip location
	push ebx ;push eip location
		
	call systemTimer ;call function with parameters
	
	pop ebx ;pop eip location
	pop ebx ;pop register pointer
	
	;pushing iretd values location
	mov ebx, esp ;get stack pointer
	add ebx, 32 ;go to iretd values
	push ebx ;push iretd values location
	
	sub ebx, 20 ;go to new esp
	mov edx, [ebx] ;get esp
	push edx ;push location
		
	call fixInterrupt ;fix iretd values
	pop ebx ;pop parameters
	pop ebx
	
	popa ;pop registers
	sub esp, 20 ;go to esp
	mov esp, [esp] ;go to new stack
		
	iretd

shiftScr:
	push edx
	mov edx, [esp + 8]
	int 30h ;call shift interrupt
	pop edx
	ret
	
shiftHandler:
	push edx
	call shiftScreen ;call C function
	pop edx
	iretd

clearHandler:
	push eax
	call clear ;call C function
	pop eax
	iretd

clrscr:
	push eax
	call getCurrProc
	int 31h ;call clear interrupt
	pop eax
	ret
	
clrscr2:
	push eax
	mov eax, [esp + 8]
	int 31h ;call clear interrupt
	pop eax
	ret
	
shellProc:
	call shellIn ;call C keyboard handler
	iretd
	
switchTo:
	push edx ;backup reg
	mov edx, [esp + 8] ;get process ptr	
	int 32h ;trigger interrupt
	pop edx ;restore reg
	ret

switchHandler:
	push edx ;push as parameter
	call switchMain ;switch main
	pop edx ;remove from stack
	iretd ;return from interrupt

_newMarquee:
	push edx ;backup registers
	push ebx
	mov edx, [esp + 12] ;get text location
	mov ebx, [esp + 16] ;get shell row
	int 33h ;create new marquee
	pop ebx ;restore registers
	pop edx
	ret
	
marqueeHandler: 
	push ebx ;push shell row
	push edx ;push text location
	call newMarquee
	pop edx ;remove args
	pop ebx
	iretd

deac:
	push ecx ;push parameters
	push edx
	push ebx
	call deactivate ;call C deactivator
	pop ebx ;pop parameters
	pop edx
	pop ecx
	iretd	

deacAsm:
	push edx ;backup registers
	push ebx
	push ecx
	mov edx, [esp + 16] ;get process pointer
	mov ebx, esp ;get return address
	add ebx, 16
	mov ecx, [esp + 20] ;get new address
	cmp ecx, 0 ;if ecx is null
	je l1 ;go to l1
	cmp ecx, 1 ;if ecx is 1
	je l2 ;go to l2
	l1:
		mov ecx, [esp + 12] ;get basic return location
		jmp l3 ;endif
	l2: 
		mov ecx, 0 ;set return eip to null flag
	l3:
	int 34h ;call interrupt
	pop ecx ;restore registers
	pop ebx
	pop edx
	add esp, 4 ;go to return location
	ret

activate:
	push edx ;backup register
	mov edx, [esp + 8] ;get program #
	int 35h ;call interrupt
	pop edx ;restore register
	ret	

activateAsm:
	push edx ;push program #
	call newProc ;create new process
	pop edx ;pop parameter
	iretd

processRunner:
	mov edx, [esp + 8] ;get instruction address
	call edx ;call program
	call deacAsm ;deactivate program
	
strIn:
	push edx ;backup register
	call getCurrProc ;get process who called this function
	push eax ;push process pointer
	call scanStr ;call scan string
	pop edx ;pop parameter
	pop edx ;restore register
	ret

printAsm:
	push eax
	push edx
	call getCurrProc
	push eax
	call printHandler
	pop edx
	pop edx
	pop edx
	iretd

strOut:
	push edx
	push eax
	mov edx, [esp + 12];
	mov eax, 1
	int 36h
	pop eax
	pop edx
	ret

intOut:
	push edx
	push eax
	mov edx, [esp + 12];
	mov eax, 2
	int 36h
	pop eax
	pop edx
	ret

hexOut:
	push edx
	push eax
	mov edx, [esp + 12];
	mov eax, 3
	int 36h
	pop eax
	pop edx
	ret

charOut:
	push edx
	push eax
	mov edx, [esp + 12];
	mov eax, 4
	int 36h
	pop eax
	pop edx
	ret

start: ;main subroutine
	cli ;disable interrupts
	call kmain ;call subroutine
	hlt ;halt kernel
