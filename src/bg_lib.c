/*
 * $Id$
 */

// Copyright (C) 1999-2000 Id Software, Inc.
//
// bg_lib.c -- standard C library replacement routines used by code
// compiled for the virtual machine

#include "q_shared.h"

size_t strlen( const char *string ) {
	const char	*s;

	s = string;
	while ( *s ) {
		s++;
	}
	return s - string;
}

char *strcat( char *strDestination, const char *strSource ) {
	char	*s;

	s = strDestination;
	while ( *s ) {
		s++;
	}
	while ( *strSource ) {
		*s++ = *strSource++;
	}
	*s = 0;
	return strDestination;
}

char *strcpy( char *strDestination, const char *strSource ) {
	char *s;

	s = strDestination;
	while ( *strSource ) {
		*s++ = *strSource++;
	}
	*s = 0;
	return strDestination;
}

char *strchr( const char *string, int c ) {
	while ( *string ) {
		if ( *string == c ) {
			return ( char * )string;
		}
		string++;
	}

	if ( !c )
		return ( char * )string;

	return (char *)0;
}

// qqshka: have no example, so do my'n own, dunno good or bad
char *strrchr( const char *string, int c ) {
	char *from = ( char * )string + strlen( string );

	while ( from >= string ) {
		if ( *from == c ) {
			return from;
		}
		from--;
	}
	return (char *)0;
}

char *strstr( const char *string, const char *strCharSet ) {
	while ( *string ) {
		int		i;

		for ( i = 0 ; strCharSet[i] ; i++ ) {
			if ( string[i] != strCharSet[i] ) {
				break;
			}
		}
		if ( !strCharSet[i] ) {
			return (char *)string;
		}
		string++;
	}
	return (char *)0;
}

int tolower( int c ) {
	if ( c >= 'A' && c <= 'Z' ) {
		c += 'a' - 'A';
	}
	return c;
}


int toupper( int c ) {
	if ( c >= 'a' && c <= 'z' ) {
		c += 'A' - 'a';
	}
	return c;
}

void *memmove( void *dest, const void *src, size_t count ) {
	int		i;

	if ( dest > src ) {
		for ( i = count-1 ; i >= 0 ; i-- ) {
			((char *)dest)[i] = ((char *)src)[i];
		}
	} else {
		for ( i = 0 ; i < (int) count ; i++ ) {
			((char *)dest)[i] = ((char *)src)[i];
		}
	}
	return dest;
}

//=====================================

double tan( double x ) {
	return sin(x) / cos(x);
}

int abs( int n ) {
	return n < 0 ? -n : n;
}

double fabs( double x ) {
	return x < 0 ? -x : x;
}

float fabsf( float x ) {
	return x < 0 ? -x : x;
}

//=====================================

static int randSeed = 0;

void	srand( unsigned seed ) {
	randSeed = seed;
}

int		rand( void ) {
	randSeed = (69069 * randSeed + 1);
	return randSeed & 0x7fff;
}

//=====================================

double atof( const char *string ) {
	float sign;
	float value;
	int		c;


	// skip whitespace
	while ( *string <= ' ' ) {
		if ( !*string ) {
			return 0;
		}
		string++;
	}

	// check sign
	switch ( *string ) {
	case '+':
		string++;
		sign = 1;
		break;
	case '-':
		string++;
		sign = -1;
		break;
	default:
		sign = 1;
		break;
	}

	// read digits
	value = 0;
	c = string[0];
	if ( c != '.' ) {
		do {
			c = *string++;
			if ( c < '0' || c > '9' ) {
				break;
			}
			c -= '0';
			value = value * 10 + c;
		} while ( 1 );
	} else {
		string++;
	}

	// check for decimal point
	if ( c == '.' ) {
		double fraction;

		fraction = 0.1;
		do {
			c = *string++;
			if ( c < '0' || c > '9' ) {
				break;
			}
			c -= '0';
			value += c * fraction;
			fraction *= 0.1;
		} while ( 1 );

	}

	// not handling 10e10 notation...

	return value * sign;
}

double _atof( const char **stringPtr ) {
	const char	*string;
	float sign;
	float value;
	int		c = '0'; // bk001211 - uninitialized use possible

	string = *stringPtr;

	// skip whitespace
	while ( *string <= ' ' ) {
		if ( !*string ) {
			*stringPtr = string;
			return 0;
		}
		string++;
	}

	// check sign
	switch ( *string ) {
	case '+':
		string++;
		sign = 1;
		break;
	case '-':
		string++;
		sign = -1;
		break;
	default:
		sign = 1;
		break;
	}

	// read digits
	value = 0;
	if ( string[0] != '.' ) {
		do {
			c = *string++;
			if ( c < '0' || c > '9' ) {
				break;
			}
			c -= '0';
			value = value * 10 + c;
		} while ( 1 );
	}

	// check for decimal point
	if ( c == '.' ) {
		double fraction;

		fraction = 0.1;
		do {
			c = *string++;
			if ( c < '0' || c > '9' ) {
				break;
			}
			c -= '0';
			value += c * fraction;
			fraction *= 0.1;
		} while ( 1 );

	}

	// not handling 10e10 notation...
	*stringPtr = string;

	return value * sign;
}

int atoi( const char *string ) {
	int		sign;
	int		value;
	int		c;


	// skip whitespace
	while ( *string <= ' ' ) {
		if ( !*string ) {
			return 0;
		}
		string++;
	}

	// check sign
	switch ( *string ) {
	case '+':
		string++;
		sign = 1;
		break;
	case '-':
		string++;
		sign = -1;
		break;
	default:
		sign = 1;
		break;
	}

	// read digits
	value = 0;
	do {
		c = *string++;
		if ( c < '0' || c > '9' ) {
			break;
		}
		c -= '0';
		value = value * 10 + c;
	} while ( 1 );

	// not handling 10e10 notation...

	return value * sign;
}

int _atoi( const char **stringPtr ) {
	int		sign;
	int		value;
	int		c;
	const char	*string;

	string = *stringPtr;

	// skip whitespace
	while ( *string <= ' ' ) {
		if ( !*string ) {
			return 0;
		}
		string++;
	}

	// check sign
	switch ( *string ) {
	case '+':
		string++;
		sign = 1;
		break;
	case '-':
		string++;
		sign = -1;
		break;
	default:
		sign = 1;
		break;
	}

	// read digits
	value = 0;
	do {
		c = *string++;
		if ( c < '0' || c > '9' ) {
			break;
		}
		c -= '0';
		value = value * 10 + c;
	} while ( 1 );

	// not handling 10e10 notation...

	*stringPtr = string;

	return value * sign;
}

//=========================================================

#define ALT			0x00000001		/* alternate form */
#define HEXPREFIX	0x00000002		/* add 0x or 0X prefix */
#define LADJUST		0x00000004		/* left adjustment */
#define LONGDBL		0x00000008		/* long double */
#define LONGINT		0x00000010		/* long integer */
#define QUADINT		0x00000020		/* quad integer */
#define SHORTINT	0x00000040		/* short integer */
#define ZEROPAD		0x00000080		/* zero (as opposed to blank) pad */
#define FPT			0x00000100		/* floating point number */

#define to_digit(c)		((c) - '0')
#define is_digit(c)		((unsigned)to_digit(c) <= 9)
#define to_char(n)		((n) + '0')

void AddInt( char **buf_p, int val, int width, int flags ) {
	char	text[32];
	int		digits;
	int		signedVal;
	char	*buf;

	digits = 0;
	signedVal = val;
	if ( val < 0 ) {
		val = -val;
	}
	do {
		text[digits++] = '0' + val % 10;
		val /= 10;
	} while ( val );

	if ( signedVal < 0 ) {
		text[digits++] = '-';
	}

	buf = *buf_p;

	if( !( flags & LADJUST ) ) {
		while ( digits < width ) {
			*buf++ = ( flags & ZEROPAD ) ? '0' : ' ';
			width--;
		}
	}

	while ( digits-- ) {
		*buf++ = text[digits];
		width--;
	}

	if( flags & LADJUST ) {
		while ( width-- ) {
			*buf++ = ( flags & ZEROPAD ) ? '0' : ' ';
		}
	}

	*buf_p = buf;
}

void AddFloat( char **buf_p, float fval, int width, int prec, int flags ) {
	char	text[32];
	int		digits;
	int     prec_len, len;
	float	signedVal;
	char	*buf;
	int		val;

	// set sanity precison
	if (prec < 0 || prec > 6)
		prec = 6;

	prec_len = prec ? (prec + 1) : 0;

	// get the sign
	signedVal = fval;
	if ( fval < 0 ) {
		fval = -fval;
	}

	// write the float number
	digits = 0;
	val = (int)fval;
	do {
		text[digits++] = '0' + val % 10;
		val /= 10;
	} while ( val );

	if ( signedVal < 0 ) {
		text[digits++] = '-';
	}

	buf = *buf_p;

	len = digits;
	if ( !(flags & LADJUST) ) { // right adjust
		while ( (len + prec_len ) < width ) {
			*buf++ = ( flags & ZEROPAD ) ? '0' : ' ';
			width--;
		}
	}

	while ( digits-- ) {
		*buf++ = text[digits];
	}

	*buf_p = buf;

	// write the fraction
	digits = 0;
	while (digits < prec) {
		fval -= (int) fval;
		fval *= 10.0;
		val = (int) fval;
		text[digits++] = '0' + val % 10;
	}

	if (digits > 0) {
		buf = *buf_p;
		*buf++ = '.';
		for (prec = 0; prec < digits; prec++) {
			*buf++ = text[prec];
		}
		*buf_p = buf;
	}

	if ( flags & LADJUST ) { // left adjust
		buf = *buf_p;
		while ( (len + prec_len) < width ) {
			*buf++ = ( flags & ZEROPAD ) ? '0' : ' ';
			width--;
		}
		*buf_p = buf;
	}
}

void AddString( char **buf_p, char *string, int width, int prec, int flags ) {
	int		size;
	char	*buf;

	buf = *buf_p;

	if ( string == NULL ) {
		string = "(null)";
		prec = -1;
	}

	if ( prec >= 0 ) {
		for( size = 0; size < prec; size++ ) {
			if( string[size] == '\0' ) {
				break;
			}
		}
	}
	else {
		size = strlen( string );
	}

	width -= size;

	if ( !(flags & LADJUST) ) { // right adjust
		while( width-- > 0 )
			*buf++ = ( flags & ZEROPAD ) ? '0' : ' ';
	}

	while( size-- ) {
		*buf++ = *string++;
	}

	if ( flags & LADJUST ) { // left adjust
		while( width-- > 0 )
			*buf++ = ( flags & ZEROPAD ) ? '0' : ' ';
	}

	*buf_p = buf;
}

/*
vsprintf

I'm not going to support a bunch of the more arcane stuff in here
just to keep it simpler.  For example, the '*' and '$' are not
currently supported.  I've tried to make it so that it will just
parse and ignore formats we don't support.
*/
int vsprintf( char *buffer, const char *fmt, va_list argptr ) {
	int		*arg;
	char	*buf_p;
	char	ch;
	int		flags;
	int		width;
	int		prec;
	int		n;
	char	sign;

	buf_p = buffer;
	arg = (int *)argptr;

	while( true ) {
		// run through the format string until we hit a '%' or '\0'
		for ( ch = *fmt; (ch = *fmt) != '\0' && ch != '%'; fmt++ ) {
			*buf_p++ = ch;
		}
		if ( ch == '\0' ) {
			goto done;
		}

		// skip over the '%'
		fmt++;

		// reset formatting state
		flags = 0;
		width = 0;
		prec = -1;
		sign = '\0';

rflag:
		ch = *fmt++;
reswitch:
		switch( ch ) {
		case '-':
			flags |= LADJUST;
			goto rflag;
		case '.':
			n = 0;
			while( is_digit( ( ch = *fmt++ ) ) ) {
				n = 10 * n + ( ch - '0' );
			}
			prec = n < 0 ? -1 : n;
			goto reswitch;
		case '0':
			flags |= ZEROPAD;
			goto rflag;
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			n = 0;
			do {
				n = 10 * n + ( ch - '0' );
				ch = *fmt++;
			} while( is_digit( ch ) );
			width = n;
			goto reswitch;
		case 'c':
			*buf_p++ = (char)*arg;
			arg++;
			break;
		case 'd':
		case 'i':
			AddInt( &buf_p, *arg, width, flags );
			arg++;
			break;
		case 'f':
			AddFloat( &buf_p, *(double *)arg, width, prec, flags );
#ifdef __LCC__
			arg += 1;	// everything is 32 bit in my compiler
#else
			arg += 2;
#endif
			break;
		case 's':
			AddString( &buf_p, (char *)*arg, width, prec, flags );
			arg++;
			break;
		case '%':
			*buf_p++ = ch;
			break;
		default:
			*buf_p++ = (char)*arg;
			arg++;
			break;
		}
	}

done:
	*buf_p = 0;
	return buf_p - buffer;
}

/* this is really crappy */
int sscanf( const char *buffer, const char *fmt, ... ) {
	int		cmd;
	int		**arg;
	int		count;

	arg = (int **)&fmt + 1;
	count = 0;

	while ( *fmt ) {
		if ( fmt[0] != '%' ) {
			fmt++;
			continue;
		}

		cmd = fmt[1];
		fmt += 2;

		switch ( cmd ) {
		case 'i':
		case 'd':
		case 'u':
			**arg = _atoi( &buffer );
			break;
		case 'f':
			*(float *)*arg = _atof( &buffer );
			break;
		}
		arg++;
	}

	return count;
}

// ignore count :(
int Q_vsnprintf(char *buffer, size_t count, const char *format, va_list argptr)
{
	int ret;

	if (!count)
		return 0;

	ret = vsprintf(buffer, format, argptr);
	buffer[count - 1] = 0;

	return ret;
}

// ignore count :(
int snprintf(char *buffer, size_t count, char const *format, ...)
{
	int ret;
	va_list argptr;

	if (!count)
		return 0;

	va_start(argptr, format);
	ret = Q_vsnprintf(buffer, count, format, argptr); // which is ignore count :(
	buffer[count - 1] = 0;
	va_end(argptr);

	return ret;
}
