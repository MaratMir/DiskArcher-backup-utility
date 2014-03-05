// DiskArcher.
// CRoom class - a folder to place several files of copies.
// (C) Marat Mirgaleev, 2001-2002.
// Modifications:
//  (1) 19.01.2002. Log added.
//  (2) 20.01.2002. "Miscelaneous.h" added.
//  (3) 11.02.2002. m_nPrognosisFree, GetOccupiedSpace(),
//                  DeleteMarkedCopies() added.
//                  GetSpaceFree() renamed to GetDiskSpaceFree().
//  (4) 13.02.2002. Changes in Progress Dialog.
//                  Changes in CountFiles().
//  (5) 17.02.2002. Memorizing file size.
//  (6) 20.02.2002. Delete(), DeleteLabel() and IsAvailable() added.
//  (7) 04.04.2002. "Stop" button press handling.
//  (8) 03.07.2002. Flag archive->m_bStopWorking handled.
//  (9) 11.11.2002. Deleting of read-only copies.
// (10) 19.11.2002. progressDlg changed into m_pArchive->m_pProgressDlg.
// (11) 21.04-26.10.2003. Compression.
// (12) 22.11.2003. GetOccupiedSpace() - now counts only copies not marked for deletion.
// (13) 24.05.2006. Error tracking improved.
//==============================================================================

#include "stdafx.h"
#include <algorithm>

#include "CRoom.h"
#include "CMyArchive.h"
#include "CFileToArc.h"
#include "CBundle.h"
#include "CArchiveDB.h"
#include "CRoomLabel.h"
#include "IProgressCtrl.h"

//==============================================================================
bool CRoom::GetDiskSpaceFree()
{
	bool bSuccess;

	ULARGE_INTEGER i64FreeBytesToCaller, i64TotalBytes;
	BOOL Ok = GetDiskFreeSpaceEx( getFullName(),
		(PULARGE_INTEGER)&i64FreeBytesToCaller, (PULARGE_INTEGER)&i64TotalBytes, NULL );
	bSuccess = ( Ok != 0 );
	if( bSuccess )
		m_nDiskSpaceFree = i64FreeBytesToCaller.QuadPart;
	else
		m_nDiskSpaceFree = -1;

	return bSuccess;
}


//==============================================================================
CRoom::CRoom(const CString& strName)
	 : CFolder( strName ) // M
{
}


//==============================================================================
OpResult CRoom::doCopying()
{
  OpResult nResult = OPR_SUCCESSFUL; // (13) Was: bool bSuccess=false;
  bool bFileSuccess=true;


// First of all - Delete all Copies marked for deletion from this Room
//---------------------------------------------------------------------
  if( ! DeleteMarkedCopies() )
    nResult = OPR_NONFATAL_ERRORS;


// Thru all Files To Arc - select files that must be copied into this Room
//-------------------------------------------------------------------------
// Copy files even we couldn't delete previous copies
  POSITION filesPos;
  for( filesPos = m_pArchive->m_FilesToArc.GetHeadPosition(); filesPos != NULL; )
  {
    if(   m_pArchive->getProgressDlg()->isAborted() // Interrupted by user
       || m_pArchive->m_bStopWorking )
        // Their order is important! Becase m_bStopWorking is changed during IsAborted() checking!
    {
    // Write to the Log
      g_TheArchive.m_LogFile.AddRecord( L"", L"", L"User break" );

      nResult = OPR_USER_STOP;
      break;
    }

    CFileToArc *pCurFile = m_pArchive->m_FilesToArc.GetNext( filesPos );

    // Find and process all the copies of this file assigned to this Room
    auto copyRoomIter = std::find( pCurFile->m_CopyToRooms.begin(), pCurFile->m_CopyToRooms.end(), this );
    for( ; copyRoomIter != pCurFile->m_CopyToRooms.end(); copyRoomIter++ )
     // 2014. Was: if( pCurFile->m_pRoom != NULL && pCurFile->m_pRoom->m_nRoomID == m_nRoomID )
    {
      if( pCurFile->m_addCopy )
        // 2014. Was: if( pCurFile->m_nCommand != CFileToArc::fcNothing )
      {
        m_pArchive->getProgressDlg()->setMessage( pCurFile->getFullName() );

        // Add all Copies of this file, assigned to this Room
        //====================================================
        CFileCopy *pNewCopy = new CFileCopy();
        m_pArchive->m_Copies.AddTail( pNewCopy );// And there it will be deleted
        pNewCopy->m_strPath = pCurFile->getFullPath();
        pNewCopy->m_strFilename = pCurFile->m_strName;
        pNewCopy->m_size = pCurFile->getSize();
        pNewCopy->m_packedSize = pCurFile->m_nPredictedCompressedSize;

      // Send the Copy to a Bundle where isn't any copy of this file
      // LATER: But now every file is placed in a separated bundle
      // Check the File and the Rooms settings and choose type of the bundle
        CBundle::bundleType theBundleType = CBundle::btSingleFile;
        if( m_pArchive->isCompressorDefined() )	// (13)
          if( ! pCurFile->m_bCompressIt )
          {
            if( m_nCompressionMode == rcmAlways )
              theBundleType = CBundle::btZipFile;
          }
          else
            if( m_nCompressionMode == rcmAllowed || m_nCompressionMode == rcmAlways )
              theBundleType = CBundle::btZipFile;

        CBundle* pNewBundle = m_pArchive->m_Bundles.BundleCreate( *copyRoomIter, theBundleType );
        pCurFile->m_pBundle = pNewBundle;
        pNewCopy->m_nBundleID = pCurFile->m_pBundle->m_nBundleID;

        bFileSuccess = pNewBundle->addCopy( pCurFile ); // Be careful! Polymorphismus is here!


      // Write the result to DB or to log
      //==================================
        if( ! bFileSuccess )
          ;
        else
        {
          pNewCopy->m_FileDateTime = pCurFile->m_LastWriteTime;
          bFileSuccess = g_TheArchive.m_pDB->CopyAdd( pNewCopy );
        }

        if( bFileSuccess )
          if( pCurFile->m_nStatus != fsNotFound )
            pCurFile->m_nStatus = fsUpToDate;

      } // End of Add a Copy

      m_pArchive->getProgressDlg()->setMessage( L"" );
      m_pArchive->getProgressDlg()->advance( 1 );
#ifdef _DEBUGGGG // Progress indicator debugging
      /*int poz =*/ m_pArchive->m_pProgressDlg->m_Progress.GetPos();
#endif
    } // End of This file is assigned to this Room
  } // End of Thru all Files To Arc


// Write the Room's contents file
//================================
  if( ! writeContents() )
    nResult = OPR_NONFATAL_ERRORS;


// Update info about the Room's free space.
//  Even if it is a removable disk, because the disk is inserted now!
  GetDiskSpaceFree();
  g_TheArchive.m_pDB->RoomUpdate( this );

  m_pArchive->getProgressDlg()->setMessage( L"" );

  return nResult;
}


//==============================================================================
bool CRoom::CheckLabel()
{
	CRoomLabel lbl;
	lbl.m_pRoom = this;
	bool bSuccess = lbl.Check();
	return bSuccess;
}


//==============================================================================
unsigned CRoom::CountAllFiles() const
{
  unsigned int nCount=0;
  POSITION filesPos;
  for( filesPos = m_pArchive->m_FilesToArc.GetHeadPosition(); filesPos != NULL; )
  {
    CFileToArc *pCurFile = m_pArchive->m_FilesToArc.GetNext( filesPos );
    nCount += pCurFile->CountCopies( this );
  }
  return nCount;
}


//==============================================================================
unsigned CRoom::CountFilesBeingCopied() const
{
  unsigned int nCount=0;
  POSITION filesPos;
  for( filesPos = m_pArchive->m_FilesToArc.GetHeadPosition(); filesPos != NULL; )
  {
    CFileToArc *pCurFile = m_pArchive->m_FilesToArc.GetNext( filesPos );
    auto copyRoomIter = std::find( pCurFile->m_CopyToRooms.begin(), pCurFile->m_CopyToRooms.end(), this );
    for( ; copyRoomIter != pCurFile->m_CopyToRooms.end(); copyRoomIter++ )
      // 2014. Was: if( pCurFile->m_pRoom != NULL && pCurFile->m_pRoom->m_nRoomID == m_nRoomID )
    {
      if( pCurFile->m_addCopy ) // 2014 Was: if( pCurFile->m_nCommand == CFileToArc::fcAddCopy )
        nCount++;
    }
  }
  return nCount;
}


// (3) Delete all Copies marked for deletion from this Room.
//===========================================================
bool CRoom::DeleteMarkedCopies()
{
	bool bSuccess = true;

  m_pArchive->getProgressDlg()->setMessage( L"Deleting excessive copies..." );

// Select all this Room's Copies
	POSITION copiesPos;
	for( copiesPos = m_pArchive->m_Copies.GetHeadPosition(); copiesPos != NULL; )
	{
		CFileCopy *pCurCopy = m_pArchive->m_Copies.GetNext( copiesPos );
		if( pCurCopy->m_bDeleteIt )
		{
			CBundle* pCurBundle = m_pArchive->m_Bundles.BundleFind( pCurCopy->m_nBundleID );
			ASSERT( pCurBundle );
			if( pCurBundle->m_nRoomID == m_nRoomID )
			// Is this Copy from this Room?
				bSuccess &=  m_pArchive->m_Copies.DeleteCopy( pCurCopy );
		}
// LATER: progressDlg.IsAborted() ?
	}
	return bSuccess;
}


//  (3) Counts and returns sizes of all NON-DELETED Copies in the Room.
// (12) Now counts only copies that are not marked for deletion!
//=====================================================================
__int64 CRoom::GetOccupiedSpace() const
{
  __int64 nOccupiedSpace = 0;

// Select all this Room's Copies
	POSITION copiesPos;
	for( copiesPos = m_pArchive->m_Copies.GetHeadPosition(); copiesPos != NULL; )
	{
		CFileCopy *pCurCopy = m_pArchive->m_Copies.GetNext( copiesPos );
		if( ! pCurCopy->m_bDeleteIt )	// (12) "if" added
		{
			CBundle* pCurBundle = 
					m_pArchive->m_Bundles.BundleFind( pCurCopy->m_nBundleID );
			ASSERT( pCurBundle );
      if( pCurBundle->m_nRoomID == m_nRoomID ) // Is this Copy from this Room?
        nOccupiedSpace += pCurCopy->m_size;
		}
	}
	return nOccupiedSpace;
}


// (3) Returns prognosis of free space in the Room
//		or -1 if the Room is unavailable.
// Sets m_nPrognosisFree member variable.
//===========================================================
__int64 CRoom::GetPrognosis()
{
	if(		m_sizeLimit != 0		// Is the Limit has been set
		&&	m_nDiskSpaceFree != -1 )// And the Room is available
	{
    __int64 nOccupiedSpace = GetOccupiedSpace();
		m_nPrognosisFree = min( m_nDiskSpaceFree, m_sizeLimit - nOccupiedSpace );
	}
	else
		m_nPrognosisFree = m_nDiskSpaceFree;
	return m_nPrognosisFree;
}


// Returns a list of Copies which are stored in this Room.
// Don't forget to call "delete" for this list.
//=========================================================
CFilesCopies* CRoom::GetCopies() const
{
	CFilesCopies* roomCopies = new CFilesCopies;
	POSITION pos;
	for( pos = g_TheArchive.m_Copies.GetHeadPosition(); pos != NULL; )
	{
		CFileCopy *pCurCopy = g_TheArchive.m_Copies.GetNext( pos );
		if( pCurCopy->GetRoom()->m_nRoomID == this->m_nRoomID )
			roomCopies->AddTail( pCurCopy );
	}
    return roomCopies;
}


//==============================================================================
CBundles* CRoom::GetBundles() const
{
	CBundles* roomBundles = new CBundles;
	POSITION pos;
	for( pos = g_TheArchive.m_Bundles.GetHeadPosition(); pos != NULL; )
	{
		CBundle *pCurBundle = g_TheArchive.m_Bundles.GetNext( pos );
		if( pCurBundle->m_nRoomID == this->m_nRoomID )
			roomBundles->AddTail( pCurBundle );
	}
    return roomBundles;
}


// (6)
//==============================================================================
bool CRoom::Delete()
{
  if( IsAvailable() )
    DeleteLabel();

  return g_TheArchive.m_pDB->RoomDelete( this );
}


//==============================================================================
bool CRoom::IsAvailable()
{
	return ( m_nDiskSpaceFree != -1 );
}


//==============================================================================
bool CRoom::DeleteLabel()
{
	CRoomLabel lbl;
	lbl.m_pRoom = this;
	bool bSuccess = lbl.Erase();
	return bSuccess;
}


// (13) Exracted from "spaghetti" code.
//==============================================================================
bool CRoom::writeContents()
{
  bool bSuccess = true;
	CStdioFile contFile;
  CString sRoomNum;
	sRoomNum.Format( _T("Room #%d"), m_nRoomID ); // (13)
  enum steps { eWriting, eClosing };
  steps nStep = eWriting;
	try
	{
    m_pArchive->getProgressDlg()->setMessage( L"Writing Room's contents..." );
		contFile.Open( getFullName() + "\\Contents.txt",
				           CFile::modeCreate | CFile::modeWrite | CFile::typeText );
		contFile.WriteString( _T("Archive Room Contents\n---------------------\n") );

		POSITION copiesPos;
		for( copiesPos = m_pArchive->m_Copies.GetHeadPosition(); copiesPos != NULL; )
		{
			CFileCopy* pCurCopy = m_pArchive->m_Copies.GetNext( copiesPos );
			CBundle* pCurBundle = 
				            	m_pArchive->m_Bundles.BundleFind( pCurCopy->m_nBundleID );
			if( pCurBundle == NULL )
				AfxMessageBox( _T("CRoom::DoCopying: Can't find the bundle") );
			else
				if( pCurBundle->m_nRoomID == m_nRoomID )
				{
					contFile.WriteString( pCurBundle->m_strName + ": "
			        				+ pCurCopy->m_strPath + pCurCopy->m_strFilename + "\n" );
        // And write the type of the bundle? - There will be the extension (ZIP),
        //    it's enough
				}
		}

    nStep = eClosing;
		contFile.Close();

	}
	catch(...) // (13) Was: ( CFileException e )
  {
    CString mess;
    if( nStep == eWriting )
      mess = "The Room Contents file could not be written.";
    else
      mess = "The Room Contents file could not be closed.";
    AfxMessageBox( mess );
    g_TheArchive.m_LogFile.AddRecord( sRoomNum, L"", mess );
    bSuccess = false;
  }

  return bSuccess;

} // End of writeContents()