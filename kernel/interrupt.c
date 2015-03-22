#define IDT_SIZE 256

/*structure for an interrupt descriptor*/
typedef struct IDTEntryTag {
	unsigned short int offset_lowerbits; /*pointer to function to be called*/
	unsigned short int selector;
	unsigned char zero;
	unsigned char type_attr;
	unsigned short int offset_higherbits; /*pointer to function to be called*/
}IDTEntry;

IDTEntry idtTable[IDT_SIZE]; /*interrupt descriptor table*/

extern void outb (unsigned short, unsigned char);
extern void shellProc();
extern void timer();
extern void shiftHandler();
extern void clearHandler();
extern void load_idt( unsigned long * );

void idt_init() {

	unsigned long address; /*address of keyboard handler*/
	unsigned long idt_address; 
	unsigned long idt_ptr[2];

	/* populate IDT entry of keyboard's interrupt */
	address = (unsigned long)shellProc; 
	idtTable[0x21].offset_lowerbits = address & 0xffff;
	idtTable[0x21].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
	idtTable[0x21].zero = 0;
	idtTable[0x21].type_attr = 0x8e; /* INTERRUPT_GATE */
	idtTable[0x21].offset_higherbits = (address & 0xffff0000) >> 16;

	/* populate IDT entry of PIT's interrupt */
	address = (unsigned long)timer; 
	idtTable[0x20].offset_lowerbits = address & 0xffff;
	idtTable[0x20].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
	idtTable[0x20].zero = 0;
	idtTable[0x20].type_attr = 0x8e; /* INTERRUPT_GATE */
	idtTable[0x20].offset_higherbits = (address & 0xffff0000) >> 16;
	
	/* populate IDT entry of screen shift's interrupt */
	address = (unsigned long)shiftHandler; 
	idtTable[0x30].offset_lowerbits = address & 0xffff;
	idtTable[0x30].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
	idtTable[0x30].zero = 0;
	idtTable[0x30].type_attr = 0x8e; /* INTERRUPT_GATE */
	idtTable[0x30].offset_higherbits = (address & 0xffff0000) >> 16;

	/* populate IDT entry of clear screen's interrupt */
	address = (unsigned long)clearHandler; 
	idtTable[0x31].offset_lowerbits = address & 0xffff;
	idtTable[0x31].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
	idtTable[0x31].zero = 0;
	idtTable[0x31].type_attr = 0x8e; /* INTERRUPT_GATE */
	idtTable[0x31].offset_higherbits = (address & 0xffff0000) >> 16;

	/*     Ports
	*	 PIC1	PIC2
	*Command 0x20	0xA0
	*Data	 0x21	0xA1
	*/

	/* ICW1 - begin initialization */
	outb(0x20 , 0x11);
	outb(0xA0 , 0x11);

	/* ICW2 - remap offset address of IDT */
	/*
	* In x86 protected mode, we have to remap the PICs beyond 0x20 because
	* Intel have designated the first 32 interrupts as "reserved" for cpu exceptions
	*/
	outb(0x21 , 0x20);
	outb(0xA1 , 0x28);

	/* ICW3 - setup cascading */
	outb(0x21 , 0x00);  
	outb(0xA1 , 0x00);  

	/* ICW4 - environment info */
	outb(0x21 , 0x01);
	outb(0xA1 , 0x01);
	/* Initialization finished */

	/* mask interrupts */
	outb(0x21 , 0xff);
	outb(0xA1 , 0xff);

	/* fill the IDT descriptor */
	idt_address = (unsigned long)idtTable;
	idt_ptr[0] = ( sizeof ( IDTEntry ) * IDT_SIZE ) + 
				 ( ( idt_address & 0xffff ) << 16 );
	idt_ptr[1] = idt_address >> 16 ;

	load_idt(idt_ptr);
}

void irq_init() {
	
	outb( 0x21, 0xFC );
}
