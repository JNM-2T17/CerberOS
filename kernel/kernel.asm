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
extern kmain ;kmain is an external function
extern shellIn
extern systemTimer
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
	;passes &eip, ebp, and esp as parameters to systemTimer
	pusha ;push registers
	push esp ;push location of registers
	mov ebx, esp ;get stack
	add ebx, 36 ;move beyond registers to eip location
	push ebx ;push eip location
		
	call systemTimer ;call function with parameters
	
	pop ebx ;pop eip location
	pop ebx ;pop register pointer
	
	;check flags
	cmp eax, 0 ;if flag is zero
	je noframe ;go to end
	
		mov ebx, esp ;
		add ebx, 12 ;move to esp
		mov edx, [ebx] ;get esp
		sub edx, 12 ;move new esp minus 4 int sizes
		mov [ebx], edx ;update new esp
		push edx ;push location
		add ebx, 20 ;go to iretd values
		push ebx ;push pointer to iretd values
		jmp endtimer
			
	noframe: ;sets up new frame
		
		mov ebx, esp ;get stack pointer
		add ebx, 8 ;go to ebp
		mov ebx, [ebx] ;go to new ebp
		sub ebx, 4 ;move to one space below
		mov edx, kmain ;move kmain as default return value
		mov [ebx], edx ;put kmain in bottom of process stack
		sub ebx, 12 ;go to new ebp minus 4 int sizes
		push edx ;push location
		mov ebx, esp ;get stack pointer
		add ebx, 8 ;go to ebp
		mov edx, [ebx] ;store ebp in edx
		sub edx, 12 ;minus 4 int sizes
		add ebx, 4 ;got to esp
		mov [ebx], edx ;move esp to iretd values
		add ebx, 36 ;go to iretd values
		push ebx ;push pointer to iretd values
		
	endtimer:
		call fixInterrupt ;fix iretd values
		pop ebx ;pop parameters
		pop ebx
		popa
		
	iretd

shellProc:
	call shellIn
	iretd

start: ;main subroutine
	cli ;disable interrupts
	call kmain ;call subroutine
	hlt ;halt kernel
