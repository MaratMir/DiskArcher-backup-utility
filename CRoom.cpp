// DiskArcher.
// CRoom.cpp - Implementation of CRoom class.
// (C) Marat Mirgaleev, 2001-2002.
// Modifications:
//	(1) 19.01.2002. Log added.
//	(2) 20.01.2002. "Miscelaneous.h" added.
//	(3) 11.02.2002. m_nPrognosisFree, GetOccupiedSpace(),
//						DeleteMarkedCopies() added.
//					GetSpaceFree() renamed to GetDiskSpaceFree().
//	(4) 13.02.2002. Changes in Progress Dialog.
//					Changes in CountFiles().
//	(5) 17.02.2002. Memorizing file size.
//	(6) 20.02.2002. Delete(), DeleteLabel() and IsAvailable() added.
//	(7) 04.04.2002. "Stop" button press handling.
//	(8) 03.07.2002. Flag archive->m_bStopWorking handled.
//	(9) 11.11.2002. Deleting of read-only copies.
// (10) 19.11.2002. progressDlg changed into m_pArchive->m_pProgressDlg.
// (11) 21.04-26.10.2003. Compression.
// (12) 22.11.2003. GetOccupiedSpace() - now counts only copies not marked
//					for deletion.
// (13) 24.05.2006. Error tracking improved.
//==============================================================================

#include "stdafx.h"
#include "CRoom.h"
#include <afx.h>
#include "CMyArchive.h"
#include "CBundle.h"
#include "MArc2.h"				// (1)
#include "CArchiveDB.h"			// M
#include "CFileToArc.h"			// M
#include "CFileCopy.h"			// M
#include "CProgressDialog.h"	// M
#include "CRoomLabel.h"			// M
#include "Miscelaneous.h"		// (2)

// (10) extern CProgressDialog progressDlg;


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
OpResult /*(13) Was bool*/ CRoom::doCopying()
{
  OpResult nResult = OPR_SUCCESSFUL; // (13) Was: bool bSuccess=false;
	bool bFileSuccess=true;


// (3) At first - Delete all Copies marked for deletion from this Room
//---------------------------------------------------------------------
	if( ! DeleteMarkedCopies() )
    nResult = OPR_NONFATAL_ERRORS;
	  // (13) Was: bSuccess = DeleteMarkedCopies();


// Thru all Files To Arc - select files that must be copied into this Room
//-------------------------------------------------------------------------
// (9) Copy files even we couldn't delete previous copies
  POSITION filesPos;
  for( filesPos = m_pArchive->m_FilesToArc.GetHeadPosition(); filesPos != NULL; )
  {
    if(   m_pArchive->m_pProgressDlg->IsAborted() 	// Interrupted by user
		   || m_pArchive->m_bStopWorking/*(8)*/ )
				// Their order is important! Becase m_bStopWorking is changed
				//	during IsAborted() checking!
				// (8) Was just	if( progressDlg.IsAborted() )
		{
		// (1) Write to Log	
			g_TheArchive.m_LogFile.AddRecord( "", "", "User break" );

			nResult = OPR_USER_STOP; // (13) Was: bSuccess = false;
			break;
		}

		CFileToArc *pCurFile = m_pArchive->m_FilesToArc.GetNext( filesPos );
		if( pCurFile->m_pRoom != NULL && pCurFile->m_pRoom->m_nRoomID == m_nRoomID )
    // This file is assigned to this Room
		{
			if( pCurFile->m_nCommand != fcNothing )
				m_pArchive->m_pProgressDlg->SetMessage( pCurFile->getFullName() );

			if( pCurFile->m_nCommand == fcAddCopy )
			// Add a Copy
			//===================================
			{
				CFileCopy *pNewCopy	= new CFileCopy();
				m_pArchive->m_Copies.AddTail( pNewCopy );// And there it will be deleted
				pNewCopy->m_strPath		= pCurFile->getFullPath();
				pNewCopy->m_strFilename = pCurFile->m_strName;
				pNewCopy->m_nSize		= pCurFile->m_nSize;	// (5)
				pNewCopy->m_nPackedSize = pCurFile->m_nPredictedCompressedSize;//(11)

 			// Send the Copy to a Bundle where isn't any copy of this file
			// LATER: But now every file is placed in a separated bundle
			//		  And is it needed to do in different way?
			// (11) Check the File and the Rooms settings and choose type 
			//		of the bundle
				bundleType theBundleType = btSingleFile;
				if( m_pArchive->isCompressorDefined() )	// (13)
					if( ! pCurFile->m_bCompressIt )
					{
						if( m_nCompressionMode == rcmAlways )
							theBundleType = btZipFile;
					}
					else
						if(    m_nCompressionMode == rcmAllowed
  							|| m_nCompressionMode == rcmAlways )
							theBundleType = btZipFile;
			// endOf (11)
				CBundle* pNewBundle = 
					    m_pArchive->m_Bundles.BundleCreate( pCurFile->m_pRoom,
						    									                theBundleType );					
				pCurFile->m_pBundle = pNewBundle;
				
				pNewCopy->m_nBundleID	= pCurFile->m_pBundle->m_nBundleID;
				pCurFile->m_pCopyToReplace = pNewCopy;

				bFileSuccess = pNewBundle->addCopy( pCurFile );
										// (11) Be careful! Polymorphismus is here!

			} // End of Add a Copy

			if( pCurFile->m_nCommand == fcReplaceCopy )
			// zz Эта ветка никогда не срабатывает !!!! ?
			// Replace a Copy
			//===================================
			{
				pCurFile->m_pCopyToReplace->m_nSize		  = pCurFile->m_nSize;	// (5)
				pCurFile->m_pCopyToReplace->m_nPackedSize = pCurFile->m_nSize;	// (5)
				bFileSuccess = pCurFile->m_pBundle->addCopy( pCurFile );
			}

		// Write the result to DB or to log
		//==================================
			if( ! bFileSuccess )
				;
			else
			{
				if( pCurFile->m_nCommand == fcAddCopy )
				{
					pCurFile->m_pCopyToReplace->m_FileDateTime = pCurFile->m_LastWriteTime;
					bFileSuccess = g_pTheDB->CopyAdd( pCurFile->m_pCopyToReplace );
				}
				if( pCurFile->m_nCommand == fcReplaceCopy )
				// zz Эта ветка никогда не срабатывает !!!! ?
				{
					pCurFile->m_pCopyToReplace->m_FileDateTime = pCurFile->m_LastWriteTime;
					bFileSuccess = g_pTheDB->CopyUpdate( pCurFile->m_pCopyToReplace );
				}

				if( bFileSuccess )
					if( pCurFile->m_nStatus != fsNotFound )
						pCurFile->m_nStatus = fsUpToDate;
			}

			m_pArchive->m_pProgressDlg->SetMessage( "" );
			m_pArchive->m_pProgressDlg->Advance( 1 );
#ifdef _DEBUG
	/*int poz =*/ m_pArchive->m_pProgressDlg->m_Progress.GetPos();
#endif
		} // End of This file is assigned to this Room
	} // End of Thru all Files To Arc

// Write Room's contents file
  if( ! writeContents() )           // (13)
    nResult = OPR_NONFATAL_ERRORS;  /* (13) Was:
		CStdioFile contFile;
		try
		{
			m_pArchive->m_pProgressDlg->SetMessage( "Writing Room's contents..." );
			contFile.Open( 
				GetFullName() + "\\Contents.txt",
				CFile::modeCreate | CFile::modeWrite | CFile::typeText );
			contFile.WriteString( "Archive Room Contents\n---------------------\n" );
		// TO DO: Error processing

			POSITION copiesPos;
			for( copiesPos = m_pArchive->m_Copies.GetHeadPosition(); copiesPos != NULL; )
			{
				CFileCopy* pCurCopy = m_pArchive->m_Copies.GetNext( copiesPos );
				CBundle* pCurBundle = 
					m_pArchive->m_Bundles.BundleFind( pCurCopy->m_nBundleID );
				if( pCurBundle == NULL )
					AfxMessageBox( "CRoom::DoCopying: Can't find the bundle" );
				else
					if( pCurBundle->m_nRoomID == m_nRoomID )
					{
						contFile.WriteString( pCurBundle->m_strName + ": "
							+ pCurCopy->m_strPath + pCurCopy->m_strFilename + "\n" );
// zz And write the type of the bundle? - There will be the extension (ZIP), it's enough
						bSuccess = true;
					}
			}
		}
		catch( CFileException e )
		{
			AfxMessageBox( "The contents file could not be written:\n" + e.m_cause );
		}
	
		try
		{
			contFile.Close();
		}
		catch( CFileException e )
		{
			AfxMessageBox( "The contents file could not be closed:\n" + e.m_cause );
		}
// (9)}	// if( bSuccess )
*/

// Update info about the Room's free space.
//		Even if it is a removable disk, because the disk is inserted now!
	GetDiskSpaceFree();	
	g_pTheDB->RoomUpdate( this );

	m_pArchive->m_pProgressDlg->SetMessage( "" );
	
	return nResult; // (13) Was: bSuccess;
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
int CRoom::CountFiles( enum countType nType )
// (4) Was int CRoom::CountFilesForCopying()
{
	int nCount=0;
	POSITION filesPos;
	for( filesPos = m_pArchive->m_FilesToArc.GetHeadPosition(); filesPos != NULL; )
	{
		CFileToArc *pCurFile = m_pArchive->m_FilesToArc.GetNext( filesPos );
		if( pCurFile->m_pRoom != NULL && pCurFile->m_pRoom->m_nRoomID == m_nRoomID )
		{
			switch( nType )
			{
			case countAll:
				nCount++;
				break;
			case countForCopying:
				if( pCurFile->m_nCommand == fcAddCopy || 
					pCurFile->m_nCommand == fcReplaceCopy )
						nCount++;
				break;
			}
		}
	}
	return nCount;
}


// (3) Delete all Copies marked for deletion from this Room.
//===========================================================
bool CRoom::DeleteMarkedCopies()
{
	bool bSuccess = true;

	m_pArchive->m_pProgressDlg->SetMessage( "Deleting excessive copies..." );

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
_int64 CRoom::GetOccupiedSpace() const
{
	_int64 nOccupiedSpace = 0;

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
			if( pCurBundle->m_nRoomID == m_nRoomID )
			// Is this Copy from this Room?
				nOccupiedSpace += pCurCopy->m_nSize;
		}
	}
	return nOccupiedSpace;
}


// (3) Returns prognosis of free space in the Room
//		or -1 if the Room is unavailable.
// Sets m_nPrognosisFree member variable.
//===========================================================
_int64 CRoom::GetPrognosis()
{
	if(		m_nSizeLimit != 0		// Is the Limit has been set
		&&	m_nDiskSpaceFree != -1 )// And the Room is available
	{
		_int64 nOccupiedSpace = GetOccupiedSpace();
		m_nPrognosisFree = min( m_nDiskSpaceFree, m_nSizeLimit - nOccupiedSpace );
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

    return g_pTheDB->RoomDelete( this );
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
		m_pArchive->m_pProgressDlg->SetMessage( "Writing Room's contents..." );
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
		g_TheArchive.m_LogFile.AddRecord( sRoomNum, "", mess );
    bSuccess = false;
	}

  return bSuccess;

} // End of writeContents()