void kmain(){

  /*basic message*/
  char *str = "Welcome to CerberOS!\n\nThat's it.\n\n...\n\nThis is our first"
        " kernel.\n\n...\n\nWhat were you expecting?\n\nMac OS X?";
  char *vidPtr = (char *)0xB8000; /*pointer to the video portion in memory*/
  unsigned int i = 0; /*counters*/
  unsigned int j = 0;
  unsigned int k = 1; /*next line index, zero-based*/

  /*clears the screen*/
  while( i < 80 * 25 * 2 ){
    vidPtr[i] = 0; /*assign null char*/
    vidPtr[i+1] = 0x07; /*assign black background and light grey text*/
    i += 2;
  }
  
  i = 0; /*reset counter*/

  /*while not at end of string*/
  while( str[j] != '\0' ){
    while( str[j] == '\n'){ /*while newlines*/
      i = k++ * 160; /*assigns start of next row, and increments line*/
      j++; /*next character*/
    }
    vidPtr[i] = str[j++]; /*assign character*/
    vidPtr[i+1] = 0x07; /*assign black background and light grey text*/
    i += 2; /*add 2 to pointer index*/
  }
}