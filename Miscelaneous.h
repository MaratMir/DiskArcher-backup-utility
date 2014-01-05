// Miscelaneous functions.
// Header file.
// (C) Marat Mirgaleev, 2001-2002.
// Modifications:
//	(1) 20.01.2002. Moved from MArc2.h in a separated file.
//	(2) 20.01.2002. GetErrDescription(): Parameter added.
//	(3) 04.02.2002. MyWindowClose() added.
//	(4) 11.11.2002. DeleteReadOnlyFile() added.
//	(6) 26.10.2003. MyShellExecute() and RemoveRightSlash() added.
//=================================================================

#if !defined(MISCELANEOUS_H__INCLUDED_)
#define MISCELANEOUS_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



bool BrowseForFolder( CString strCaption, CString& strFolder );

CString GetErrDescription( bool bWithoutLF=false );

void MyWindowClose( CMDIChildWnd* pWnd );	// (3)

bool DeleteReadOnlyFile( CString fname );	// (4)

DWORD MyShellExecute( CString sExeFile, CString sParams, CString sDir,
					  int nShow ); // (6)

CString RemoveRightSlash( const CString& str );	// (6)

#endif // !defined(MISCELANEOUS_H__INCLUDED_)
