/***
	returns the length of a string
	Parameter:
		str - string to find the length of
***/
int len( char *str ) {

	int i = 0; /*counter*/
	while( str[i] != '\0' ) { /*while not at end of string*/
		i++; /*increment counter*/
	}
	
	return i;
}

/***
	copies one string to another location
	Parameters:
		dest - destination string
		src - string to copy
***/
char *cpy( char *dest, char *src ) {
	
	int i,
	    nLen = len( src );

	for ( i = 0; i < nLen; i++ ) {
		dest[i] = src[i];
		if( src[i] == '\0' ) {
			break;
		}
	}

	return dest;
}

/***
	checks if two strings are equal
	Parameters:
		str1 - destination string
		str2 - string to copy
***/
int cmp( char *str1, char *str2 ) {

	int j = 0;
	
	/*while not at end of either string*/
	while( str1[j] != '\0' && str2[j] != '\0' ) {

		/*if characters are not equal*/
		if( str1[j] != str2[j] ) {
			break;
		} else {
			j++; /*next char*/
		}
	}

	return ( str1[j] - str2[j] );
}

/***
	concatenates two strings
	Parameters:
		str1 - destination string
		str2 - string to concatenate
***/
char *concat( char *str1, char *str2 ) {
	int i = len( str1 ), /*get length of string*/
	    j = 0; /*counter for str2*/
	
	/*while not end of str2*/
	while( str2[j] != '\0' ) {
		
		/*add character at end*/
		str1[i] = str2[j];

		/*increment counters*/
		i++; j++; 
	}

	/*put null char*/
	str1[i] = 0;

	return str1;
}

/***
	converts a string to lowercase
	Parameter:
		str - string to convert
***/
char *toLowercase( char *str ) {

	int i; /*counter*/

	/*for each char*/
	for( i = 0; str[i] != '\0'; i++ ) {

		/*if uppercase*/
		if( ( str[i] - 'A' ) < 26 && ( str[i] - 'A' ) >= 0 ) {
			str[i] += 'a' - 'A'; /*convert to lowercase*/
		}
	}
	
	return str;
}

/***
	converts a string to uppercase
	Parameter:
		str - string to convert
***/
char *toUppercase( char *str ) {

	int i; /*counter*/

	/*for each char*/
	for( i = 0; str[i] != '\0'; i++ ) {

		/*if lowercase*/
		if( ( str[i] - 'a' ) < 26 && ( str[i] - 'a' ) >= 0 ) {
			str[i] -= 'a' - 'A'; /*convert to lowercase*/
		}
	}
	
	return str;
}

/***
	compares ignoring case. 
	WARNING: converts actual parameters to lowercase
	Parameters:
		str1 - destination string
		str2 - string to concatenate
***/
int cmpIgnoreCase( char *str1, char *str2 ) {
	
	return cmp( toLowercase( str1 ), toLowercase( str2 ) );
}
