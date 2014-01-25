// DiskArcher.
// Implementation of CFoldersToArc class.
// (C) Marat Mirgaleev, 2002.
// Created 03.03.2002.
//	(1) 01.11.2002. theDB changed to *pTheDB.
//=================================================================

#include "stdafx.h"
#include "CFoldersToArc.h"
#include "CFolderToArc.h"

#include "CMyArchive.h"
#include "CArchiveDB.h"

using namespace MArcLib;


const error* CFoldersToArc::AddFolder( CString name )
{
  const error* result = error::getDefault();

  result = new error( error::unsuccessfulOperation, 
                      L"You cannot add an entire folder.\n"
                      L"Please use the Files Locator to scan a folder.\n"
                      L"You can also select and add several files at once." );
// TODO: I could start Locator here...

/*
// Check is this Folder already in the Archive
  CFolderToArc *pFound = FindFolder( name );
  if( pFound != NULL )
    AfxMessageBox( name + ":\nThis Folder is already in the Archive" );		
  else
  {
    CFolderToArc *pNewFolder = new CFolderToArc( name );
    pNewFolder->m_nUpToCopies = theArchive.m_nDefaultCopies;
    pNewFolder->m_nFolderID = ...
    // LATER: pNewFolder->m_bPaused = false;

    // Write to DB
      bResult = theDB.FolderAdd( pNewFolder );

    // Add to the list in memory
      AddTail( pNewFolder );

    bResult = true;
  }
*/
  return result;
}


// Load Folders list from DB into memory.
//========================================
bool CFoldersToArc::Load()
{
// Load from DB
  bool bSuccess = g_TheArchive.m_pDB->FoldersLoad();

	return bSuccess;
}


bool CFoldersToArc::DeleteFolder(CFolderToArc* pFolder)
{
  UNREFERENCED_PARAMETER( pFolder );
  return (bool)0;
}


CFolderToArc* CFoldersToArc::FindFolder( CString strFolderName ) const
{
	CFolderToArc* pFound = NULL;
	POSITION pos;
	for( pos = GetHeadPosition(); pos != NULL; )
	{
		CFolderToArc *pCurFolder = GetNext( pos );
		if( pCurFolder->getFullName() == strFolderName )
		{
			pFound = pCurFolder;
			break;
		}
	}
	return pFound;
}
