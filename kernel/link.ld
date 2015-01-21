OUTPUT_FORMAT( elf32-i386 ) /*output emulates 32-bit on an i3 processor x86 
                              machine*/
ENTRY( start ) /*execution begins at start*/
SECTIONS {
  . = 0x100000; /*this is a kernel, so load this at memloc 0x100000*/
  .text : { *(.text) } /*link the following sections*/
  .data : { *(.data) }
  .bss  : { *(.bss) }
}