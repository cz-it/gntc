#ifndef GNT_OS_H_
#define GNT_OS_H_



#ifndef IN
#	define IN
#endif /* IN */

#ifndef OUT
#	define OUT
#endif /* OUT */

#ifndef INOUT
#	define INOUT
#endif /* INOUT */

#ifdef __cplusplus
extern "C"
{
#endif

#if defined(_WIN32) || defined(_WIN64)
#	if(_WIN32_WINNT >= 0x0400)
#		include <winsock2.h>
#		include <windows.h>
#	else /* _WIN32_WINNT */
#		include <winsock2.h>
#		include <windows.h>
#	endif /* _WIN32_WINNT */
#	include <io.h>
#	include <process.h>
#else
#	include <unistd.h>
#	include <endian.h>
#	include <libgen.h>
#endif


#if defined (_WIN32) || defined (_WIN64)

#include <stdarg.h>

/** 
* for snprintf class
*/
#	if  _MSC_VER < 1400
#		define snprintf				_snprintf
#		define vsnprintf				_vsnprintf
#		define snwprintf				_snwprintf
#		define vsnwprintf				_vsnwprintf
#	else
#		define _CRT_SECURE_NO_WARNINGS
#		define snprintf(s,n,f,...)				_snprintf_s((s),(n),_TRUNCATE ,(f),__VA_ARGS__)
//#   define vsnprintf(s, n, f, v) _vsnprintf_s((s), (n), _TRUNCATE, (f), (v))
#		define snwprintf				_snwprintf
#		define vsnwprintf				_vsnwprintf
#	endif  /** end for snprintf class */


void usleep(unsigned long usec);

#endif /* end of defined (_WIN32) || defined (_WIN64) */


#ifdef __cplusplus
}
#endif
#endif