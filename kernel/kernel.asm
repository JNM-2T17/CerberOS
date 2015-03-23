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
global clearHandler ;calls C clear function
global switchTo ;switches to a process pointed to by the parameter
global switchHandler ;calls C switch main process function
global _newMarquee ;creates new marquee
global marqueeHandler ;calls new marquee function
extern kmain ;kmain is an external function
extern shellIn
extern systemTimer
extern shiftScreen
extern clear
extern switchMain
extern newMarquee
extern test
extern test2
extern fixInterrupt

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
	int 30h ;call shift interrupt
	ret
	
shiftHandler:
	call shiftScreen ;call C function
	iretd

clearHandler:
	call clear ;call C function
	iretd

clrscr:
	int 31h ;call clear interrupt
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

start: ;main subroutine
	cli ;disable interrupts
	call kmain ;call subroutine
	hlt ;halt kernel
