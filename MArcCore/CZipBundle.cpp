// DiskArcher.
// CZipBundle class - a bundle of copies in zip-format.
// (C) Marat Mirgaleev, 2001-2006.
// Modifications:
//  (1) 08.05.2006. A bug fixed - error code of CopyFile() wasn't processed.
//==============================================================================

#include "stdafx.h"
#include "CMyArchive.h"
#include "CZipBundle.h"
#include "CFileToArc.h"
#include "CFileCompressor.h"
//zzz #include "MArc2.h"
#include "..\MArcLib\Misc.h"
#include "Miscelaneous.h"


CFileCompressor* CZipBundle::m_pCompressor;


//==============================================================================
CZipBundle::CZipBundle(CString strFilename)
	: CBundle( strFilename )	// M
{
}


//==============================================================================
bool CZipBundle::addCopy(CFileToArc* pFile)
{
// (1)	bool bSuccess=true;

// Move the compressed file into the Room
	CWaitCursor wait;
	BOOL bOk = CopyFile( pFile->GetCompressedFileName(), this->getFullName(),
						           FALSE/*Overwrite existing*/ );
	if( ! bOk )
		g_TheArchive.m_LogFile.AddRecord( L"Temp dir",
								  		                L"Compressed " + pFile->m_strName,
									  	                GetErrDescription( true ) );
	return ( bOk == TRUE ); // (1) Was: return bSuccess;
}


//==============================================================================
bool CZipBundle::extractFile( const CString strGetFile, 
						                  const CString strWriteToPath,
							                const CString strWriteToFilename ) const
{
	CString sPath = RemoveRightSlash( strWriteToPath );	
												// It's necessary for PKZip25
  CString sParams = L"-extr \"" + getFullName() + L"\" \"" + sPath + L"\"";
	CWaitCursor wait;
	DWORD dwExitCode = MyShellExecute( g_TheArchive.m_pCompressor->m_strExePath,
						            sParams, g_TheArchive.getTempPath(), SW_SHOWMINIMIZED );
		// Error message will be in MyShellExecute(), if any

// Known PKZip25 error codes:
//  1 - When there is a file with this name. It asks "Overwrite?".
//	9 - If extract to unexistent path
    
	return ( dwExitCode == 0 );
}


// Phisically delete the file from the Bundle
/* LATER: if there is only one File in the Bundle, delete the Bundle; 
		  otherwise, delete the File from the Bundle. */
//==============================================================================
bool CZipBundle::deleteFileFromBundle( CFileCopy* pFileCopy )
{
// Today it is the same as parent class's method
  UNREFERENCED_PARAMETER(pFileCopy);
	bool bSuccess=true;
	CString fname = getFullName();	// It is the name of the Bundle file

	bSuccess = DeleteReadOnlyFile( fname );
	if( ! bSuccess )
		g_TheArchive.m_LogFile.AddRecord( getFullPath(), m_strName, 
                                      L"Can't delete Bundle" );

	return bSuccess;
}
