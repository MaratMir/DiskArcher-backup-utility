// DiskArcher.
// CFilesCopies.cpp - Implementation of CFilesCopies class.
// (C) Marat Mirgaleev, 2001-2002.
// Moved from CMyArchive 10.02.2002.
// Modifications:
//	(1) GetFileCopies() added.
//	(2) 10.04.2002. Delete() - Freeing memory.
//	(3) 01.11.2002. theDB changed to *pTheDB.
//  (4) 04.05.2006. Deleted copies skipped.
//==========================================================================

#include "stdafx.h"
#include "CFilesCopies.h"
#include "CFileCopy.h"
#include "CFileToArc.h"
#include "MArc2.h"
#include "CMyArchive.h"
#include "CRoom.h"
#include "CBundle.h"
#include "CArchiveDB.h"


// Returns even copies marked for deletion!
//==============================================================================
CFileCopy* CFilesCopies::FindCopy(ID nID) const
{
	CFileCopy *pFound = NULL;
	POSITION pos;
	for( pos = GetHeadPosition(); pos != NULL; )
	{
		CFileCopy *pCurCopy = GetNext( pos );
		if( pCurCopy->m_nCopyID == nID )
		{
			pFound = pCurCopy;
			break;
		}
	}
	return pFound;
}


//==============================================================================
bool CFilesCopies::DeleteCopy(CFileCopy* pFileCopy)
{
	bool bSuccess=true;
	CBundle* pCurBundle = g_TheArchive.m_Bundles.BundleFind( pFileCopy->m_nBundleID );
	ASSERT( pCurBundle );

// Phisically delete the Copy from the Bundle
/* TO DO: if there is only one File in the Bundle, delete the Bundle; 
		  otherwise, delete the File from the Bundle. */
	bSuccess = pCurBundle->deleteFileFromBundle( pFileCopy );
		// It can be a problem if we can't delete the file because how can we replace
		//	it in this case?

// Delete the Copy from DB
	if( bSuccess )
		bSuccess = g_pTheDB->CopyDelete( pFileCopy );

// Delete the Copy from memory (from the list)
	if( bSuccess )
	{
		POSITION pos = Find( pFileCopy );
		ASSERT( pos );
		RemoveAt( pos );

		delete pFileCopy;	// (2)
	}	

// Increase Room's Free size? Probably, no, because we will get the free space
//    directly from the disk at the end of the backup process.

	return bSuccess;
}


// Skips copies marked for deletion.
// LATER: Use GetFileCopies()?
//==============================================================================
CFileCopy* CFilesCopies::GetLatestCopy(CFileToArc* pFile) const
{
	CFileCopy* pLatestCopy = NULL;
	POSITION pos;
	for( pos = GetHeadPosition(); pos != NULL; )
	{
		CFileCopy *pCurCopy = GetNext( pos );
    if( pCurCopy->m_bDeleteIt )	// (4) added
      continue;                 // (4) added
		if( ( pFile->getFullPath() == pCurCopy->m_strPath ) && 
			( pFile->m_strName == pCurCopy->m_strFilename ) )
		{
			if( pLatestCopy == NULL )
				pLatestCopy = pCurCopy;
			else
				if( pCurCopy->m_FileDateTime > pLatestCopy->m_FileDateTime )
					pLatestCopy = pCurCopy;
		}
	}
	return pLatestCopy;
}


// Skips copies marked for deletion.
// LATER: Use GetFileCopies()?
//==============================================================================
CFileCopy* CFilesCopies::GetOldestCopy(CFileToArc* pFile) const
{
	CFileCopy* pOldestCopy = NULL;
	POSITION pos;
	for( pos = GetHeadPosition(); pos != NULL; )
	{
		CFileCopy *pCurCopy = GetNext( pos );
    if( pCurCopy->m_bDeleteIt )	// (4) added
      continue;                 // (4) added
		if( ( pFile->getFullPath() == pCurCopy->m_strPath ) && 
			( pFile->m_strName == pCurCopy->m_strFilename ) )
		{
			if( pOldestCopy == NULL )
				pOldestCopy = pCurCopy;
			else
				if( pCurCopy->m_FileDateTime < pOldestCopy->m_FileDateTime )
					pOldestCopy = pCurCopy;
		}
	}
	return pOldestCopy;
}


// Skips copies marked for deletion.
//==============================================================================
int CFilesCopies::GetCopiesCount( CFileToArc* pFile, ID nRoomID ) const
{
	int nCount = 0;
	POSITION pos;
	for( pos = GetHeadPosition(); pos != NULL; )
	{
		CFileCopy *pCurCopy = GetNext( pos );
    if( pCurCopy->m_bDeleteIt )	// (4) added
      continue;                 // (4) added
		if(		( pFile->getFullPath() == pCurCopy->m_strPath ) && 
				( pFile->m_strName == pCurCopy->m_strFilename ) )
			if( nRoomID == -1 ) // Default - don't take in account Room ID
				nCount++;
			else
			{
				CRoom *pRoom;
				if( ( pRoom = g_TheArchive.m_Rooms.RoomFind( pCurCopy ) ) == NULL )
					AfxMessageBox( "Can not find the Room!" );	// TO DO
				else
					if( pRoom->m_nRoomID == nRoomID )
						nCount++;
			}
	}
	return nCount;
}


// (1) Creates a new List with Copies of the given file and returns
//	a pointer to it.
//	You should call "delete" for it after use!
//===================================================================
CFilesCopies* CFilesCopies::GetFileCopies( CFileToArc* pFile ) const
{
	CFilesCopies* copiesList = new CFilesCopies;
	POSITION pos;
	for( pos = GetHeadPosition(); pos != NULL; )
	{
		CFileCopy *pCurCopy = GetNext( pos );
    if( pCurCopy->m_bDeleteIt )	// (4) added
      continue;                 // (4) added
		if(		( pFile->getFullPath() == pCurCopy->m_strPath ) && 
				( pFile->m_strName == pCurCopy->m_strFilename ) )
			copiesList->AddTail( pCurCopy );
	}
	return copiesList;
}
