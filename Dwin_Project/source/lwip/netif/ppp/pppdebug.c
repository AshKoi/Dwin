#include "pppdebug.h"
#include  <stdarg.h>
#include  <stdio.h>

void ppp_trace(int level, const char *format,...)
{
	static unsigned char buffer[256 + 1];
    va_list vArgs;

    va_start( vArgs, format );
    vsprintf( (char *)buffer, (char const *)format, vArgs );
    va_end( vArgs );
//    dbgu_puts( buffer );
}

