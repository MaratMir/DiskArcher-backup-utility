// Miscelaneous functions.
// (C) Marat Mirgaleev, 2001-2002.
// Modifications:
//	(1) 20.01.2002. GetErrDescription() :Parameter added.
//	(2) 04.02.2002. MyWindowClose() added.
//	(3) 09.03.2002. MyWindowClose() error correction.
//	(4) 11.11.2002. DeleteReadOnlyFile() added.
//	(5) 11.01.2003. Correction in (4) - if the file was not found,
//					consider it is succcess.
//	(6) 26.10.2003. MyShellExecute() and RemoveRightSlash() added.
//=================================================================

#include "stdafx.h"
#include "Miscelaneous.h"


bool BrowseForFolder( CString strCaption, CString& strFolder )
{
	bool bSuccess = true;
	BROWSEINFO bi;
//	LPITEMIDLIST pidlRoot;						// PIDL for Root folder
	LPITEMIDLIST SelFolder;						// PIDL for selected folder
	LPMALLOC pMalloc;							// IMalloc interface handler
	char dispName[MAX_PATH];

	SHGetMalloc(&pMalloc);						// Get the IMalloc interface
//	SHGetSpecialFolderLocation(m_hWnd, CSIDL_DESKTOP, &pidlRoot);	// Get the Root folder

	bi.hwndOwner = 	AfxGetMainWnd()->m_hWnd;
    bi.pidlRoot = NULL;//pidlRoot;
    bi.pszDisplayName = dispName;
    bi.lpszTitle = strCaption;
    bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_RETURNFSANCESTORS | BIF_STATUSTEXT
		     /*| BIF_NEWDIALOGSTYLE*/ | BIF_VALIDATE ;
	bi.lpfn = NULL;//BrowseCallbackProc;
	bi.lParam = (long)((LPCTSTR)strFolder);	// parse the initial folder value
	bi.iImage = NULL;

	//	Call the Browse for folder dialog:
	if ((SelFolder = SHBrowseForFolder(&bi)) == NULL)	// User canceled
		bSuccess = false;
	else
	{
		if (SHGetPathFromIDList(SelFolder, dispName))
		{
			strFolder = dispName;
			if( strFolder.Right(1) != "\\" )
				strFolder += "\\";
		}
		else
		{
			AfxMessageBox( "You have selected an invalid folder.\nPlease try again." );
			bSuccess = false;
		}
		//UpdateData(FALSE);		// Update the data in the window
		pMalloc->Free(SelFolder);	// Free resources
	}
//	pMalloc->Free(pidlRoot);		// Free the PIDL resource

	return bSuccess;
}


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
	mess.Format( "Code: %d.%sDescription: %s", err, bWithoutLF ? " " : "\n", (LPCSTR)lpMsgBuf );
	
	return mess;
}


void MyWindowClose( CMDIChildWnd* pWnd )	// (2)
{
	pWnd->MDIRestore();
	pWnd->ShowWindow( SW_HIDE );
	CWnd* next = pWnd->GetNextWindow();
	if( next )	// (3)
		((CMDIChildWnd*)next)->MDIActivate();
	else
	{
		CMDIFrameWnd* pMainWnd = (CMDIFrameWnd*) AfxGetMainWnd();
		pMainWnd->ActivateFrame();
		CMDIChildWnd* pchw = pMainWnd->MDIGetActive();
	}
}


// (4) Delete file even if it is read-only
bool DeleteReadOnlyFile( CString fname )
{
	bool bSuccess = true;
	bool bFileFound = false;	// (5)
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = FindFirstFile( fname, &FindFileData );
	if( hFind == INVALID_HANDLE_VALUE )
	{	CString msg;
		msg.Format( "Deleting file:\n"	// (6) added
					"Cannot find file\n%s.\n%s", fname, GetErrDescription() );
		AfxMessageBox( msg );
		bSuccess = true;	// (5) Was: false.
			// We need to delete a file, but it does not exist.
			// It's Ok, no problem
		bFileFound = false;	// (5)
	}
	else
	// If this file has a read-only attribute, clear attribute
	{
		bFileFound = true;	// (5)
		if ( FindFileData.dwFileAttributes & FILE_ATTRIBUTE_READONLY )
		{	DWORD fileAttribs = ~((DWORD)FILE_ATTRIBUTE_READONLY);
				// It is mask for clear "read-only" in file attribute
			fileAttribs = fileAttribs & FindFileData.dwFileAttributes;
			if ( ! SetFileAttributes( fname, fileAttribs ) )
			{	CString msg;
				msg.Format( "Deleting file:\n"	// (6) added
							"Cannot clear read-only attribute.\n%s\n%s",
						    fname, GetErrDescription() );
				AfxMessageBox( msg );
				bSuccess = false;
			}
		}
	}
	if( bSuccess && bFileFound/*(5)*/ )
	{
		bSuccess = ( DeleteFile( fname ) != 0 ); 
					// Delete the file from disk
		if( ! bSuccess )
		{	
			CString msg;
			msg.Format( "Cannot delete a file\n%s.\n%s", 
						fname, GetErrDescription() );
			AfxMessageBox( msg );
		}
	}
	if ( hFind != INVALID_HANDLE_VALUE )
		if( ! FindClose(hFind) )	// Free memory used while file searching
		{	CString msg;
			msg.Format( "FindFileClose Error:\n%s", GetErrDescription() );
			AfxMessageBox( msg );
		} 
	return bSuccess;
}


// (6) Execute a program with parameters in a given directory.
//	Parameters:
//		nShow - specifies how the window is to be shown - SW_MAXIMIZE or other.
//	Returns the programs exit code.
DWORD MyShellExecute( CString sExeFile, CString sParams, CString sDir, int nShow )
{
	DWORD dwExitCode = 333333;	// uninitialized
	SHELLEXECUTEINFO execInfo;
	execInfo.cbSize = sizeof( SHELLEXECUTEINFO );
	execInfo.lpFile = sExeFile;	// Execute this file
		// z execInfo.lpFile = theArchive.m_pCompressor->m_strExePath;	
	execInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	execInfo.hwnd = NULL;
	execInfo.lpVerb = NULL;
	execInfo.lpParameters = sParams;
	execInfo.lpDirectory = sDir;
	execInfo.nShow = nShow;	// z SW_MAXIMIZE;
	execInfo.hInstApp = NULL;

	BOOL bRes = ShellExecuteEx( &execInfo );
	if( bRes == FALSE )
		AfxMessageBox( "ShellExecuteEx() error." );
	else
	{
		HANDLE hProcess = execInfo.hProcess;
		if( hProcess == 0 )
		{
			CString s;
			s.Format( "MyShellExecute() error:\n"
					  "Process handle is 0.\nExe: %s\nParams: %s",
					  sExeFile, sParams );
			AfxMessageBox( s );
		}
		else
		{
			DWORD dwResult = WaitForSingleObject(hProcess, INFINITE);
			if( dwResult != WAIT_OBJECT_0 )
				AfxMessageBox( "MyShellExecute():\nWaitForSingleObject() error." );

			if( ! GetExitCodeProcess( hProcess, &dwExitCode ) )
				AfxMessageBox( "MyShellExecute():\nGetExitCodeProcess() error." );
			else
				if( dwExitCode != 0 )
				{
					CString s;
					s.Format( "MyShellExecute() error:\n%s\n%s\nExit Code: %d",
							  sExeFile, sParams, dwExitCode );
					AfxMessageBox( s );
				}
// z Useless?
//		bRes = CloseHandle( hProcess );
//		if( bRes == FALSE )
//			AfxMessageBox( "CloseHandle() error." );
		}
	}
	return dwExitCode;
}


// (6) Remove right slash, if it is there
CString RemoveRightSlash( const CString& str )
{
	CString res = str;
	if( res.Right( 1 ) == "\\" )
		res = res.Left( res.GetLength()-1 );
	return res;
}
