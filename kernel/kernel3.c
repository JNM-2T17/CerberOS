#include "printer.h"
#include "keyboard.h"
#include "stringdoge.h"
#include "interrupt.h"

void kmain(){

	idt_init();
	kb_init();
	shell();
}
