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
extern kmain ;kmain is an external function
extern shellIn
extern systemTimer
extern shiftScreen
extern test
extern test2
extern fixInterrupt
extern printReg

asmtest:
	mov ebx, [esp + 4] ;get argument
	push ebx ;push argument
	mov ebx, esp ;get stack pointer
	sub ebx, 8 ;get value of esp that will point to eip after function call
	push ebx ;push eip location after function call
	call test ;call c function
	;pop ebx
	;pop ebx
	;push test2
	;ret

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
	
	;push esp ;push register pointer
;	mov ebx, esp ;pushing return value poinrts
;	add ebx, 36
;	push ebx
;	push dword 5000 ;push sleep time
;	call printReg
;	pop ebx ;pop arguments
;	pop ebx
;	pop ebx
	
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
	
;	push esp ;push register pointer
;	push dword [esp + 16] ;push new stack pointer
;	push dword 5000 ;push sleep time
;	call printReg ;print stuff
;	pop ebx ;pop shit
;	pop ebx
;	pop ebx
	
	popa
	sub esp, 20
	mov esp, [esp]
		
	iretd

shiftScr:
	int 30h ;call dx interrupt
	ret
	
shiftHandler:
	call shiftScreen
	iretd

shellProc:
	call shellIn
	iretd

start: ;main subroutine
	cli ;disable interrupts
	call kmain ;call subroutine
	hlt ;halt kernel
