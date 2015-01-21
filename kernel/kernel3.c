#define VID_COLS 80 /*columns on the screen*/
#define VID_ROWS 25 /*rows on the screen*/
#define VID_DATA_SIZE 2 /*bytes per cell on screen*/

#define VID_PTR 0xb8000 /*video location on screen*/
#define GREY_ON_BLACK 0x07 /*grey text on black background*/

unsigned int i = 0; /*basic video index*/
unsigned int k = 1; /*next line index zero-based*/

/***
  clears the screen
***/  
void clear() {

  char *vidPtr = (char *)VID_PTR; /*points to video part in memory*/
 
  /*clears screen*/
  for( i = 0; i < VID_COLS * VID_DATA_SIZE * VID_ROWS; i += 2 ){
	/*sets each cell to show null character*/
    vidPtr[i] = 0;
    vidPtr[i+1] = GREY_ON_BLACK;
  }

  i = 0; /*put video pointer on top-left*/
}

/***
  prints a string
  Parameters:;
    str - string to print
***/
void printStr( char *str ){

  char *vidPtr = (char *)VID_PTR; /*points to video part in memory*/
  unsigned int j = 0; /*counter*/
  
  while( str[j] != '\0' ){ /*while not end of string*/
    while( str[j] == '\n' && str[j] != '\0' ){ /*while newline and not end of 
                                                 string*/
      i = k++ * 160; /*go to next line and increment line counter*/
      j++; /*next char*/
    }
	
	/*if not end of string*/
    if( str[j] != '\0' ){
      vidPtr[i] = str[j]; /*set character*/
      vidPtr[i+1] = GREY_ON_BLACK; /*set black background, light grey text*/
    
      i += 2; /*increment counter*/
      if( i % 160 == 0 ){ /*if end of row*/
        k++; /*increment line*/
      }
      j++; /*next char*/
    }  
  }
}

/***
  prints a string with cycling colors
  Parameters:
    str - string to print
***/
void printStrColor( char *str ){

  char *vidPtr = (char *)VID_PTR; /*points to video part in memory*/
  char color = 0x01; /*initial color*/
  unsigned int j = 0; /*counter*/
  
  while( str[j] != '\0' ){ /*while not at end of string*/
    while( str[j] == '\n' && str[j] != '\0' ){ /*while newline and not end of 
	                                             string*/
      i = k++ * 160; /*go to next line and increment line counter*/
      j++; /*next char*/
    }
    if( str[j] != '\0' ){ /*of not end of string*/
      vidPtr[i] = str[j]; /*set character*/
      vidPtr[i+1] = color; /*set black background, variable color*/
    
      i += 2; /*increment counter*/
      if( i % 160 == 0 ){ /*if end of line*/ 
        k++; /*increment line counter*/
      }
      j++; /*next char*/

      color++; /*cycle through color*/
      if( color == 0x10 ) { /*if past last color*/
        color = 0x01; /*reset*/
      }
    }
  }
}

void kmain(){

  unsigned int i;
  /*splash screen*/
  char *splash =  "          _              _      _              _      _              _\n"
          "     | |           _- |    | #           _- |    | #           _- |\n"
          "     |  |        _-   |    |  #        _-   |    |  #        _-   |\n"
          "      |_-  ----___-     |   |_-  ----___-     |   |_-  ----___-     |\n"
          "    __-           #  _# | __-           #  _# | __-           #  _# |\n"
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
          "  -_          ___-    _--_          ___-    _--_          ___-    _-\n"
          "    -______---     __-    -______---     __-    -______---     __-\n"
          "           --                    --                    --\n\n"
          "                               WELCOME TO CerberOS!";
  
  clear(); /*clear screen*/ 
  printStrColor( splash ); /*print dogedogedoge with colors*/
  
  for( i = 0; i < 0xFFFFFF; i++ ); /*makeshift sleep function*/
  
  clear(); /*clear screen*/
  printStr( "CerberOS> " ); /*display shell*/
  
}
  