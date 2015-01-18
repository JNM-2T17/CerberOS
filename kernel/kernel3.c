#define VID_COLS 80
#define VID_ROWS 25
#define VID_DATA_SIZE 2

#define VID_PTR 0xb8000

void print( char *str ) {

	char *vidPtr = (char *)VID_PTR;
	unsigned int i = 0;
	unsigned int j = 0;
	unsigned int k = 1;
	
	while( i < VID_COLS * VID_DATA_SIZE * VID_ROWS ) {
		vidPtr[i] = 0;
		vidPtr[i+1] = 0x07;
		i += 2;
	}
	
	i = 0;
	
	while( str[j] != '\0' ) {
		while( str[j] == '\n' && str[j] != '\0' ) {
			i = k++ * 160;
			j++;
		}
		if( str[j] != '\0' ) {
			vidPtr[i] = str[j];
			vidPtr[i+1] = 0x07;
		
			i += 2;
			if( i % 160 == 0 ) {
				k++;
			}
			j++;
		}
	}
}

void kmain() {

	char *splash =  "          _              _      _              _      _              _\n"
					"		 | |           _- |    | #           _- |    | #           _- |\n"
					"		 |  |        _-   |    |  #        _-   |    |  #        _-   |\n"
					"	    |_-  ----___-     |   |_-  ----___-     |   |_-  ----___-     |\n"
					"	  __-           #  _# | __-           #  _# | __-           #  _# |\n"
					"    _-               -##- |-               -##- |-               -##- |\n"
					"  |   __              -_  | __              -_  | __              -_  |\n"
					"  |  |#-     _-#_       # ||#-     _-#_       # ||#-     _-#_       # |\n"
					" |           |##-        -_|       |##-        -_|       |##-        -_|\n"
					" |  _##_                   |#_                   |#_                   |\n"
					"- -|_#_#|_ -               |_#|_ -               |_#|_ -               |\n"
					"|  |-|-  __ _               |  __ _               |  __ _               |\n"
					"|   --__   _               |__   _               |__   _               |\n"
					"|      ---                |  ---                |  ---                |\n"
					"|                     _  |                  _  |                  _  |\n"
					" -_                 _    |                _    |                _    |\n"
					"	-_          ___-    _--_          ___-    _--_          ___-    _-\n"
					"	  -______---     __-    -______---     __-    -______---     __-\n"
					"				   --                    --                    --\n\n"
					"                               WELCOME TO CerberOS!";
	
	print( splash );
}
	