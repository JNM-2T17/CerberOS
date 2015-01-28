#include "printer.h"

/***
	returns the length of a string
	Parameter:
		str - string to find the length of
***/
int len( char *str );

/***
	copies one string to another location
	Parameters:
		dest - destination string
		src - string to copy
***/
char *cpy( char *dest, char *src );

/***
	checks if two strings are equal
	Parameters:
		str1 - destination string
		str2 - string to copy
***/
int cmp( char *str1, char *str2 );

/***
	concatenates two strings
	Parameters:
		str1 - destination string
		str2 - string to concatenate
***/
char *concat( char *str1, char *str2 );

/***
	converts a string to lowercase
	Parameter:
		str - string to convert
***/
char *toLowercase( char *str );

/***
	converts a string to uppercase
	Parameter:
		str - string to convert
***/
char *toUppercase( char *str );

/***
	compares ignoring case. 
	WARNING: converts actual parameters to lowercase
	Parameters:
		str1 - destination string
		str2 - string to concatenate
***/
int cmpIgnoreCase( char *str1, char *str2 );
