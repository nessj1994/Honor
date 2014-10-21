/***********************************************************************\
|																		|
|	File:			SGD_String.h  										|
|	Author:			Douglas Monroe										|
|	Last Modified:	2014-02-03											|
|																		|
|	Purpose:		To provide typedefs & defines for wchar_t			|
|																		|
|	© 2014 Full Sail, Inc. All rights reserved. The terms "Full Sail", 	|
|	"Full Sail University", and the Full Sail University logo are 	   	|
|	either registered service marks or service marks of Full Sail, Inc.	|
|																		|
\***********************************************************************/

#ifndef SGD_STRING_H
#define SGD_STRING_H


#include <string>
#include <sstream>


namespace SGD
{
	//*****************************************************************//
	// Unicode-friendly std string:
	typedef std::wstring			String;

	//*****************************************************************//
	// Unicode-friendly std ostringstream
	typedef std::wostringstream		OStringStream;

}	// namespace SGD


//*********************************************************************//
// Convert a (compile-time) expression to a literal string
#define STRINGIFY( exp )	L(#exp)

//*********************************************************************//
// Convert any stream-able data to an ostringstream
#define STREAM( any )		((SGD::OStringStream&)(SGD::OStringStream() << any))


//*********************************************************************//
// Unicode-friendly string functions

#define WSTRCPY_S			wcscpy_s
#define WSTRCMP				wcscmp
#define WSTRICMP			wcsicmp
#define WSTRLEN				wcslen
#define WSPRINTF_S			swprintf_s

#define WTOUPPER			towupper
#define WTOLOWER			towlower


//*********************************************************************//
// Ansi-to-Unicode conversions (should use an ostringstream instead)

#define CSTR_TO_WSTR( /* wchar_t* */ wszDest,	/* size_t */ unNumberOfElements,	/* const char* */ szSource ) \
	mbstowcs_s(nullptr, wszDest, unNumberOfElements, szSource, _TRUNCATE );

#define WSTR_TO_CSTR( /* char* */ szDest,		/* size_t */ unNumberOfElements,	/* const wchar_t* */ wszSource ) \
	wcstombs_s(nullptr, szDest, unNumberOfElements, wszSource, _TRUNCATE );
	
#define WSTR_TO_WSTR( /* wchar_t* */ wszDest,	/* size_t */ unNumberOfElements,	/* const wchar_t* */ wszSource ) \
	wcsncpy_s(wszDest, unNumberOfElements, wszSource, _TRUNCATE );
	
#define CSTR_TO_CSTR( /* char* */ szDest,		/* size_t */ unNumberOfElements,	/* const char* */ szSource ) \
	strncpy_s(szDest, unNumberOfElements, szSource, _TRUNCATE );



#endif	//SGD_STRING_H
