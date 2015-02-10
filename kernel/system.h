extern VID_DATA_SIZE; /*bytes per cell onscreen*/
extern unsigned int i; /*current screen position*/
extern unsigned int k; /*next row number*/
extern unsigned int shellRow; /*row number of shell onscreen*/

/*splash screen*/
	char *splash =  "      _              _      _              _      _              _\n"
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
					"                          WELCOME TO CerberOS!";
/*command list*/
char *cmdList = "\n\n   CerberOS Commands:\n      cls - clears screen\n      h"
                "elp - shows all commands\n      woof - shows doge\n      say "
		"- displays whatever arguments follow\n      add - add two arg"
		"uments\n      sub - subtract two arguments\n      mul - multi"
		"ply two arguments\n      hey - call a singer\n      goaway - "
		"stops a singer\n      marquee - marquees the text that follow"
		"s\n";

/***
	calls the assembly instruction outb
	Parameters:
		_port - hardware port to push data to
		_data - data to push to port
***/
void outb (unsigned short _port, unsigned char _data);

/***
	calls the assembly instruction inb
	Parameters:
		_port - hardware port to get data from
	Returns:
		data received from port
***/
unsigned char inb (unsigned short _port);

/***
	sets cursor location to where i is at.
***/
void setCursor();

/***
	sends the system to sleep
	Parameter:
		msec - approx no. of msec
***/
void sleep( unsigned int msec );

/***
	shows the shell and handles shell input
***/
void shell();
