// DiskArcher.
// Implementation of CRooms class.
// (C) Marat Mirgaleev, 2001-2002.
// Created 16.02.2002.
// Modifications:
//	(1) 06.04.2002. Corrections in RoomCreate().
//	(2) 10.04.2002. Delete() - Freeing memory.
//  (3) 26.04.2002. Corrections in RoomCreate() again.
//	(4) 01.11.2002. theDB changed to *pTheDB.
//  (5) 05.01.2003. "Label this disk..." message for CD too.
//  (6) 19-26.10.2003. Compression.
//=================================================================

#include "stdafx.h"
#include <afx.h>
#include "MArc2.h"
#include "CMyArchive.h"
#include "CArchiveDB.h"
#include "CFileCopy.h"
#include "Miscelaneous.h"
#include "CRooms.h"
#include "CRoom.h"
#include "CDrive.h"
#include "CRoomLabel.h"
#include "CBundle.h"
#include "CFilesCopies.h"
#include "CBundles.h"


// Create a new Archive Room
//===========================
bool CRooms::RoomCreate()
{
	bool bSuccess = true;
	CString strFolder;
	CRoom *pNewRoom = NULL;

	bSuccess = BrowseForFolder( "Select a folder where the Archive Room will be created:",
								strFolder );
	


// TO DO: Check is there any Room on this disk 
//		"This is not reliable to store several Archive Rooms onto the same disk"


	if( bSuccess )
	{
		pNewRoom = new CRoom( strFolder + "MArcRoom" );
		pNewRoom->m_nSizeLimit = 0;
		pNewRoom->m_nCompressionMode = rcmAllowed;	// (6)

	// Get a new Room ID: it will be maxID+1
	//==========================================
		int nMax = g_pTheDB->RoomGetMaxID();
		if( nMax == -1 )
			bSuccess = false;
		else
		{
			pNewRoom->m_pArchive = &g_TheArchive;
				// pNewRoom->m_pArchive = this;	// 19.11.01
			pNewRoom->m_nRoomID = nMax + 1;
		}
	}


// Create the Archive Room at selected path	
//==========================================

// Get selected drive's type
	CString driveName = "";
	if( bSuccess )
	{
		driveName = pNewRoom->m_strComputer + pNewRoom->m_strDrive;
		if( driveName.Right(1) != "\\" )	// (3) Prevent double slash
			driveName = driveName + "\\";
					/*(1) Without this slash drive type works in NT, 
							  but in 98 it says "DRIVE_NO_ROOT_DIR 1" */;
			// m_strComputer is empty when it is "My Computer"
	}
	CDrive drive( driveName );

	if( bSuccess )
	{
		if( drive.m_nDriveType == DRIVE_FIXED || drive.m_nDriveType == DRIVE_REMOTE )
		{
		// Check is this directory already in the Archive
			CRoom* foundRoom = RoomFind( pNewRoom->getFullName() );
			if( foundRoom != NULL )
			{	AfxMessageBox( "There is already an Archive Room at given path." );
					// TO DO: May be, in this case create another one Room?
				bSuccess = false;
			}
			pNewRoom->m_bRemovable = false;
		}
		else if( drive.m_nDriveType == DRIVE_REMOVABLE )
		{
			pNewRoom->m_bRemovable = true;
			if( AfxMessageBox( "Please insert the disk and press Ok.", MB_OKCANCEL ) != IDOK )
				bSuccess = false;
		}
		else if( drive.m_nDriveType == DRIVE_CDROM )
		{
			pNewRoom->m_bRemovable = true;
			if( AfxMessageBox( "Are you sure that this CD-drive is recordable\n"
							   "and can be operated like a usual diskette?", MB_YESNO ) != IDYES )
				bSuccess = false;
		}
		else
		{
		/*  Was:
			AfxMessageBox( "Error: Can not get drive's type.\nDriveName: <" + driveName + ">" );
			bSuccess = false;
		*/
			CString tmp;
			tmp.Format( "Error: Can not get drive's type.\n"
						"DriveName: <%s>. DriveType: %d", 
						driveName, drive.m_nDriveType );
			AfxMessageBox( tmp );
			bSuccess = false;
		}
	}
	if( bSuccess )
	{
		if( pNewRoom->checkExistence() )
		{
			AfxMessageBox( "There is already an Archive Room at given path." );
			bSuccess = false;
		}
		else
		{
			if( !CreateDirectory( pNewRoom->getFullName(), NULL ) )
			{
// TO DO: Use my function CString GetErrDescription()
				LPVOID lpMsgBuf;
				FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | 
					FORMAT_MESSAGE_IGNORE_INSERTS, NULL,
					GetLastError(),
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
					(LPTSTR) &lpMsgBuf, 0, NULL );
			
				DWORD err = GetLastError();
				CString mess;
				mess.Format( "Folder creation error:\nCode: %d.\nDescription:%s", err, (LPCSTR)lpMsgBuf );
					// TO DO: Retry?
				AfxMessageBox( mess );

				bSuccess = false;
			}
		}
	}

// Write Room's Label
	if( bSuccess )
	{
		CRoomLabel lbl;
		lbl.m_pRoom = pNewRoom;
		bSuccess = lbl.Write();
	}

// Get Free space 
	if( bSuccess )
	{
		bSuccess = pNewRoom->GetDiskSpaceFree();
	}

// Add the Room to the Database
	if( bSuccess )
	{
		bSuccess = g_pTheDB->RoomAdd( pNewRoom );
	}

	if( bSuccess )
	{
	// Add the Room to the list in memory
		AddTail( pNewRoom );

		if( drive.m_nDriveType == DRIVE_REMOVABLE
		 ||	drive.m_nDriveType == DRIVE_CDROM )	// (5)
		{
			CString mess;
			mess.Format( "Please label the disk as \"Archive Room #%d\"", 
						 pNewRoom->m_nRoomID );
			AfxMessageBox( mess );
		}
	}
	else
	{
		AfxMessageBox( "The Room has not been created" );
		delete pNewRoom;
	}
	return bSuccess;
}


// Find the Room in Archive by ID
//===================================
CRoom* CRooms::RoomFind(ID roomID)
{
	CRoom* pFound = NULL;
	POSITION pos;
	for( pos = GetHeadPosition(); pos != NULL; )
	{
		CRoom *pCurRoom = GetNext( pos );
		if( pCurRoom->m_nRoomID == roomID )
		{
			pFound = pCurRoom;
			break;
		}
	}
	return pFound;
}


// Find the Room in Archive by filename
//========================================
CRoom* CRooms::RoomFind(CString filename)
{
	CRoom* pFound = NULL;
	POSITION pos;
	for( pos = GetHeadPosition(); pos != NULL; )
	{
		CRoom *pCurRoom = GetNext( pos );
		if( pCurRoom->getFullName() == filename )
		{
			pFound = pCurRoom;
			break;
		}
	}
	return pFound;
}


// Find the Room in Archive by one of its Copies
//================================================
CRoom* CRooms::RoomFind(CFileCopy* pCopy)
{
	CRoom *pFound = NULL;
	CBundle *pBundle = g_TheArchive.m_Bundles.BundleFind( pCopy->m_nBundleID );
	if( pBundle != NULL )
		pFound = RoomFind( pBundle->m_nRoomID );
	return pFound;
}


// Load Room list from DB into memory.
//====================================
bool CRooms::RoomsLoad()
{
// Load from DB
	bool bSuccess = g_pTheDB->RoomsLoad();

// Get free space (for non-removable only)
	if( bSuccess )
		RoomsUpdate();

	return bSuccess;
}


// Update all Rooms info.
// 29.12.2001
//==================================================================================
void CRooms::RoomsUpdate()
{
	POSITION pos;
	for( pos = GetHeadPosition(); pos != NULL; )
	{
		CRoom *pCurRoom = GetNext( pos );
		if( ! pCurRoom->m_bRemovable )
			pCurRoom->GetDiskSpaceFree();
		pCurRoom->m_pArchive = &g_TheArchive;
		pCurRoom->GetPrognosis();
	}
}


bool CRooms::Delete( CRoom* pRoom )
{
	bool bSuccess = true;

// Delete all Copies from this Room
	CFilesCopies *copies = pRoom->GetCopies();
	POSITION copyPos;
	for( copyPos = copies->GetHeadPosition(); copyPos != NULL; )
	{
		CFileCopy *pCurCopy = copies->GetNext( copyPos );

	// Delete the Copy from DB
		bSuccess &= g_pTheDB->CopyDelete( pCurCopy );

	// Delete the Copy from memory (from the list)
		POSITION pos = g_TheArchive.m_Copies.Find( pCurCopy );
		ASSERT( pos );
		g_TheArchive.m_Copies.RemoveAt( pos );

		delete pCurCopy;	// (2)
	}
	delete copies;


// Delete all Bundles from this Room
	CBundles *bundles = pRoom->GetBundles();
	POSITION bunPos;
	for( bunPos = bundles->GetHeadPosition(); bunPos != NULL; )
	{
		CBundle *pCurBundle = bundles->GetNext( bunPos );
		bSuccess &= g_TheArchive.m_Bundles.DeleteBundle( pCurBundle );
	}
	delete bundles;


// Delete the Room from DB
	if( bSuccess )
		bSuccess = pRoom->Delete();

// Delete the Room from memory and from the list
	if( bSuccess )
	{
		POSITION pos = Find( pRoom );
		ASSERT( pos );
		RemoveAt( pos );
	}	

	if( bSuccess )
	{
		AfxMessageBox(	"This Room has been removed from the database.\n"
						        "But you should manualy delete copy files at path\n"
						      + pRoom->getFullName() );
		delete pRoom;	// (2)
	}	
	else
		AfxMessageBox(	"There were some errors during Room deletion.\n" );

    return (bool)0;
}
