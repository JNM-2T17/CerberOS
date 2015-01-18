void kmain(){

	char *str = "Welcome to CerberOS!\n\nThat's it.\n\n...\n\nThis is our first"
				" kernel.\n\n...\n\nWhat were you expecting?\n\nMac OS X?";
	char *vidPtr = (char *)0xB8000;
	unsigned int i = 0;
	unsigned int j = 0;
	unsigned int k = 1;

	while( i < 80 * 25 * 2 ){
		vidPtr[i] = 0;
		vidPtr[i+1] = 0x07;
		i += 2;
	}
	
	i = 0;

	while( str[j] != '\0' ){
		while( str[j] == '\n'){
			i = k++ * 160;
			j++;
		}
		vidPtr[i] = str[j++];
		vidPtr[i+1] = 0x07;
		i += 2;
	}
}