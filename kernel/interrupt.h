#define IDT_SIZE 256

typedef struct IDTEntryTag{
	unsigned short int offset_lowerbits;
	unsigned short int selector;
	unsigned char zero;
	unsigned char type_attr;
	unsigned short int offset_higherbits;
}IDTEntry;

typedef IDTEntry IDT[IDT_SIZE];

IDT idtTable;

extern void outb (unsigned short _port, unsigned char _data);
extern char getChar();

/***
	initializes idt for keyboard
***/
void idt_init();

/***
	opens line for keyboard
***/
void kb_init();
