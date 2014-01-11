// DiskArcher.
// Interface of CFilesToArc class.
// (C) Marat Mirgaleev, 2001-2002.
// Created 16.02.2002.
// Modifications:
//	(1) 18.02.2002. FileDelete() added.
//	(3) 21.01.2002. FileAdd() - Check is it directory.
//	(4) 05.03.2002. Checking is it directory moved from here.
//	(5) 08.03.2002. FilesGetStatus() changed.
//	(6) 10.04.2002. Delete - Freeing memory.
//	(7) 02.07.2002. FilesGetStatus() - Wait cursor.
//	(8) 01.11.2002. theDB changed to *pTheDB.
//  (9) 22.11.2003. ResetRuntimeData() added.
//					Files count limit for unregistered copy.
// (10) 03.03.2004. Files limit for unreg copy - only after 90 days.
// (11) 06.06.2004.   - after 45 days.
// (12) 10.07.2004. CountForCopying() eliminated because it is never used.
// (13) 24.05.2006. Copy protection changed.
//=========================================================================

#include "stdafx.h"
#include "CMyArchive.h"
#include "CFilesToArc.h"
#include "CFileToArc.h"
#include "CArchiveDB.h"
//zzz#include "MArc2.h"


// Add file by name to the Archive
//==================================
bool CFilesToArc::FileAdd(CString name)
{
	bool bResult = false;

// Check is this file already in the Archive
	CFileToArc *pFound = FileFind( name );
	if( pFound != NULL )
	{
		bResult = true;	// (9) It is not error, just warning
		AfxMessageBox( name + ":\nThis file is already in the Archive" );		
	}
	else
	{
    CFileToArc *pNewFile = new CFileToArc( name );
    pNewFile->m_nStatus = fsNew;
    pNewFile->getInfo();
    pNewFile->m_nPriority = -1;	// (1) "Not Used"   LATER
    pNewFile->m_nUpToCopies = g_TheArchive.m_nDefaultCopies;	// (1)
    pNewFile->m_nFolderID = 0;		// (4)
    pNewFile->m_bPaused = false;	// (4)

  /* (4) Out
      (3) Check is it directory
    if( pNewFile->GetType() == CDiskItem::DI_FOLDER )
      AfxMessageBox( "Backup of folders has not been implemented yet." );
    else
    {*/

  // Write to DB
    bResult = g_TheArchive.m_pDB->FileAdd( pNewFile );

  // Add to the list in memory
    AddTail( pNewFile );

  /*	}  (4) */
    bResult = true;
  }
	return bResult;
}


// Find a file by name in Archive
//==========================================================
CFileToArc* CFilesToArc::FileFind( CString fullName ) const
{
	CFileToArc* pFound = NULL;
	POSITION pos;
	for( pos = GetHeadPosition(); pos != NULL; )
	{
		CFileToArc *pCurFile = GetNext( pos );
		if( pCurFile->getFullName() == fullName )
		{
			pFound = pCurFile;
			break;
		}
	}
	return pFound;
}


// Get files info from disks
//=================================================================
int CFilesToArc::FilesGetStatus( int& o_systemFileToUpdate ) const
{
  CWaitCursor wait;
  int filesToUpdate=0, sysFilesToUpdate=0;

// FilesToArc - get file's date and time
  POSITION pos;
  for( pos = GetHeadPosition(); pos != NULL; )
  {
    CFileToArc *pCurFile = GetNext( pos );
    if( pCurFile->GetStatus() )
    {
      filesToUpdate++;
      if( pCurFile->m_bSystem )
        sysFilesToUpdate++;
    }
  }
  o_systemFileToUpdate = sysFilesToUpdate;
  return filesToUpdate;
}


// (1) Delete the File and remove it from the List.
//=================================================================
bool CFilesToArc::FileDelete( CFileToArc* pFile )
{
	bool bSuccess=true;
	bSuccess = pFile->Delete();

// Delete the File from the list and from memory 
	if( bSuccess )
	{
		POSITION pos = Find( pFile );
		ASSERT( pos );
		RemoveAt( pos );

		delete pFile;	// (6)
	}	
	return bSuccess;
}


// (9)  Clear some non-actual runtime information, including deletion
//	    of temporary files.
//===================================================================
void CFilesToArc::ResetRuntimeData()
{
	POSITION pos;
	for( pos = GetHeadPosition(); pos != NULL; )
	{
		CFileToArc *pCurFile = GetNext( pos );
		pCurFile->ResetRuntimeData();
	}
}