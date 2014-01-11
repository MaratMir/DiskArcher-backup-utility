// Miscelaneous functions.
// (C) Marat Mirgaleev, 2001-2014.
//=================================================================

#include "stdafx.h"
#include "Misc.h"

// Parameters:	- bWithoutLF - if true, don't insert "\n" in result string.
// Modifications:
//		(1) 20.01.2002. Parameter added.
CString GetErrDescription( bool bWithoutLF/*(1)*/ )
{
	LPVOID lpMsgBuf;
	FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | 
				   FORMAT_MESSAGE_IGNORE_INSERTS, NULL,
				   GetLastError(),
				   MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
				   (LPTSTR) &lpMsgBuf, 0, NULL );
			
	DWORD err = GetLastError();
	CString mess;
	mess.Format( _T("Code: %d.%sDescription: %s"), err, bWithoutLF ? " " : "\n", (LPCSTR)lpMsgBuf );
	
	return mess;
}


// (6) Remove right slash, if it is there
CString RemoveRightSlash( const CString& str )
{
	CString res = str;
	if( res.Right( 1 ) == "\\" )
		res = res.Left( res.GetLength()-1 );
	return res;
}
