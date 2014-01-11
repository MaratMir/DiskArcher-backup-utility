// DiskArcher.
// Implementation of CBundle class.
// (C) Marat Mirgaleev, 2001-2002.
// Modifications:
//	(1) 12.02.2002. AddCopy() and DeleteFileFromBundle() added.
//	(2) 20.02.2002. Delete() added.
//	(3) 26.05.2002. Error message added in DeleteFileFromBundle().
//	(4) 02.11.2002. More informative message when deletion fails.
//	(5) 11.11.2002. Deleting read-only copies.
//	(6) 26.10.2003. ExtractFile() parameters changed.
//	(7) 19.11.2003. DeleteFileFromBundle() became virtual.
//==============================================================================

#include "stdafx.h"
#include "CBundle.h"
#include "CFileToArc.h"
//zzz#include "MArc2.h"
#include "CMyArchive.h"
#include "CArchiveDB.h"
#include "../MArcLib/Misc.h"
#include "Miscelaneous.h"


//==============================================================================
CBundle::CBundle(CString strFilename) 
	: CFileOnDisk( strFilename )	// M
{
}


// (2)
//==============================================================================
bool CBundle::deleteThis()
{
  return g_TheArchive.m_pDB->BundleDelete( this );
}


// (1) Write a copy of a given file into the Bundle
//==============================================================================
bool CBundle::addCopy( CFileToArc* pFile )
{
	bool bSuccess=true;
	if( m_nType == btSingleFile )
	{
		CWaitCursor wait;	// (7)
		BOOL bOk = CopyFile( pFile->getFullName(), 
							           pFile->m_pBundle->getFullName(), 
							           FALSE/*Overwrite existing*/ );
		if( ! bOk )
			g_TheArchive.m_LogFile.AddRecord( pFile->getFullPath(), 
											                  pFile->m_strName,
											                  GetErrDescription( true ) );
		bSuccess = ( bOk != 0 );
	}
	else
		bSuccess = false;	// Use an appropriate overriden method instead!
	return bSuccess;
}


// (1) Phisically delete the file from the Bundle
/* LATER: if there is only one File in the Bundle, delete the Bundle; 
		  otherwise, delete the File from the Bundle. */
//==============================================================================
bool CBundle::deleteFileFromBundle( CFileCopy* pFileCopy )
{
  UNREFERENCED_PARAMETER( pFileCopy );
	bool bSuccess=true;
	if( m_nType == btSingleFile )
	{
		CString fname = getFullName();	// It is the name of the Bundle file

		bSuccess = DeleteReadOnlyFile( fname );
	/* (5) Was:
		bSuccess = ( DeleteFile( fname ) != 0 ); // Delete file from the disk
		if( ! bSuccess )	// (3)
		{	// (4)
			CString msg;
			msg.Format( "DiskArcher can't delete a file from a Room.\n"
						"File: %s\n"
						"%s",		fname, GetErrDescription() );
			AfxMessageBox( msg );
		}*/	/* (4)	Was:
			AfxMessageBox( "DiskArcher can't delete a file from a Room:\n"
							+ GetErrDescription() );	// (3) */
	}
  else
  {
    g_TheArchive.m_LogFile.AddRecord( getFullPath(), m_strName, L"Program logic error: Can't delete this type of Bundle" );
    bSuccess = false; // Use an appropriate overriden method instead!
  }
  return bSuccess;
}


//==============================================================================
bool CBundle::extractFile( const CString strGetFile, 
                           const CString strWriteToPath,
                           const CString strWriteToFilename ) const
{
  BOOL bOk = CopyFile( getFullName(), strWriteToPath + strWriteToFilename, FALSE/*Overwrite existing*/ );
  if( bOk == 0 )
  {
    CString msg = L"Can't extract the file from \n"
                + getFullName()
                + L":\n" + GetErrDescription();
    AfxMessageBox( msg );
  }
  return ( bOk != 0 );
}


//==============================================================================
bool CBundle::extractAll(const CString strToPath) const
{
    return (bool)0;
}
