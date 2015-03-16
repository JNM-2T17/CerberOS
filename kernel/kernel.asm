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
	mov ebx, esp ;get stack pointer
	add ebx, 4 ;get location of eip
	push ebx ;push eip
	push ebp ;push base pointer
	mov ebx, esp ;get stack pointer
	add ebx, 8 ;get end of previous function's frame
	push ebx ;push stack pointer
	call systemTimer ;call function with parameters
	pop ebx ;pop previous values
	pop ebx
	pop ebx
	mov ebx, eax ;get return value
	mov eax, 0 ;set eax to zero
	cmp [ebx], eax ;if flags are zero
	je noframe ;go to end
		add ebx, 4 ;go to ebp value
		mov ebp, [ebx] ;move to ebp
		add ebx, 4 ;go to esp value
		mov eax, [ebx] ;move esp to eax
		pop edx ;get eip
		mov esp, eax ;move stack pointer to new frame
		push edx ;push eip
		jmp endtimer ;go to endif
	noframe:
	
	endtimer:
	iretd

shellProc:
	call shellIn
	iretd

start: ;main subroutine
	cli ;disable interrupts
	call kmain ;call subroutine
	hlt ;halt kernel
