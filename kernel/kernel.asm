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
	;passes &eip, ebp, and esp as parameters to systemTimer
	mov ebx, esp ;get stack pointer
	add ebx, 12 ;get location of previous esp
	push ebx ;push stack pointer
	push ebp ;push base pointer
	mov ebx, esp ;get stack pointer
	add ebx, 8 ;get eip's location
	push ebx ;push eip's location
	
	call systemTimer ;call function with parameters
	
	;pop previous values
	pop ebx
	pop ebx
	pop ebx
	
	;check flags
	mov ebx, eax ;get return value
	mov eax, 0 ;set eax to zero
	cmp [ebx], eax ;if flags are zero
	je noframe ;go to end
	
		;goes to new values
		add ebx, 4 ;go to new ebp value
		mov ebp, [ebx] ;move value to ebp
		add ebx, 4 ;go to new esp value
		mov eax, [ebx] ;move new esp to eax
		jmp endtimer ;go to endif
		
	noframe: ;sets up new frame
		add ebx, 4 ;go to base of frame
		mov ebp, [ebx] ;move ebp to base of frame
		sub ebp, 4; move down one dword
		mov ebx, ebp ;get index to base
		mov edx, kmain ;move kmain address to edx
		mov [ebx], edx ;default return instruction is main
		sub ebp, 4 ;move down another space
		mov eax, ebp ;move top of stack to eax
		
	endtimer: ;puts flags, CS, and new EIP on stack
		mov ebx, esp ;move ebx to top of stack
		add ebx, 8 ;move ebx to flags
		mov esp, eax ;move esp to top of new function frame
		mov eax, [ebx] ;get flags
		push eax ;push flags
		sub ebx, 4 ;move down to CS
		mov eax, [ebx] ;get CS
		push eax ;push CS
		sub ebx, 4 ;move down to EIP
		mov eax, [ebx] ;get EIP
		push eax ;push EIP
		
	iretd

shellProc:
	call shellIn
	iretd

start: ;main subroutine
	cli ;disable interrupts
	call kmain ;call subroutine
	hlt ;halt kernel
