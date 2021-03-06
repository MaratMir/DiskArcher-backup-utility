// DiskArcher.
// CRooms class - collection of Rooms.
// (C) Marat Mirgaleev, 2001-2014.
// Created 16.02.2002.
// Modifications:
// (1) 06.04.2002. Corrections in RoomCreate().
// (2) 10.04.2002. Delete() - Freeing memory.
// (3) 26.04.2002. Corrections in RoomCreate() again.
// (4) 01.11.2002. theDB changed to *pTheDB.
// (5) 05.01.2003. "Label this disk..." message for CD too.
// (6) 19-26.10.2003. Compression.
//=================================================================

#include "stdafx.h"
#include <afx.h>
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
bool CRooms::createRoom()
{
  bool bSuccess = true;
  CString strFolder;
  CRoom *pNewRoom = NULL;

  bSuccess = BrowseForFolder( L"Select a folder where the Archive Room will be created:", strFolder );


// TO DO: Check is there any Room on this disk 
//   "This is not reliable to store several Archive Rooms onto the same disk"


  if( bSuccess )
  {
    pNewRoom = new CRoom( strFolder + L"MArcRoom" );
    pNewRoom->m_sizeLimit = 0; // This means 'has not been set'
    pNewRoom->m_nCompressionMode = CRoom::roomCompressionMode::rcmAllowed;	// (6)

    // Get a new Room ID: it will be maxID+1
    //==========================================
    int nMax = g_TheArchive.m_pDB->RoomGetMaxID();
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
  std::wstring driveName = L"";
  if( bSuccess )
  {
    driveName = pNewRoom->m_strComputer + pNewRoom->m_strDrive;
      // m_strComputer is empty when it is "My Computer"
  }
  MArcLib::CDrive drive( driveName );  // TODO: Shouldn't create it if not bSuccess,
                                       //       but how to make it available at all the method's scope?

  if( bSuccess )
  {
    if( drive.m_driveType == DRIVE_FIXED || drive.m_driveType == DRIVE_REMOTE )
    {
      // Check is this directory already in the Archive
      CRoom* foundRoom = find( pNewRoom->getFullName() );
      if( foundRoom != NULL )
      { AfxMessageBox( _T("There is already an Archive Room at given path.") );
          // TODO: May be, in this case create another one Room?
        bSuccess = false;
      }
      pNewRoom->m_bRemovable = false;
    }
    else if( drive.m_driveType == DRIVE_REMOVABLE )
    {
      pNewRoom->m_bRemovable = true;
      if( AfxMessageBox( _T("Please insert the disk and press Ok."), MB_OKCANCEL ) != IDOK )
        bSuccess = false;
    }
    else if( drive.m_driveType == DRIVE_CDROM )
    {
      pNewRoom->m_bRemovable = true;
      if( AfxMessageBox( _T("Are you sure that this CD-drive is recordable\n")
                 _T("and can be operated like a usual diskette?"), MB_YESNO ) != IDYES )
        bSuccess = false;
    }
    else
    {
      CString tmp;
      tmp.Format( L"Error: Cannot get the drive's type.\n"
                  L"DriveName: <%s>. DriveType: %d", 
                  driveName, drive.m_driveType );
      AfxMessageBox( tmp );
      bSuccess = false;
    }
  }
  if( bSuccess )
  {
    if( pNewRoom->checkExistence() )
    {
      AfxMessageBox( _T("There is already an Archive Room at given path.") );
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
        mess.Format( _T("Folder creation error:\nCode: %d.\nDescription:%s"), err, (LPCSTR)lpMsgBuf );
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
    bSuccess = g_TheArchive.m_pDB->RoomAdd( pNewRoom );
  }

  if( bSuccess )
  {
    // Add the Room to the list in memory
    m_rooms.push_back( pNewRoom );

    if( drive.m_driveType == DRIVE_REMOVABLE
     || drive.m_driveType == DRIVE_CDROM ) // (5)
    {
      CString mess;
      mess.Format( _T("Please label the disk as \"Archive Room #%d\""), 
             pNewRoom->m_nRoomID );
      AfxMessageBox( mess );
    }
  }
  else
  {
    AfxMessageBox( _T("The Room has not been created") );
    delete pNewRoom;
  }
  return bSuccess;
}


// Find the Room in Archive by ID
//===================================
CRoom* CRooms::find(ID roomID)
{
  CRoom* found = nullptr;
  for( auto curRoom : m_rooms )
  {
    if( curRoom->m_nRoomID == roomID )
    {
      found = curRoom;
      break;
    }
  }
  return found;
}


// Find the Room in Archive by filename
//========================================
CRoom* CRooms::find(CString filename)
{
  CRoom* found = nullptr;
  for( auto curRoom : m_rooms )
    if( curRoom->getFullName() == filename )
    {
      found = curRoom;
      break;
    }
  return found;
}


// Find the Room in Archive by one of its Copies
//================================================
CRoom* CRooms::find(CFileCopy* pCopy)
{
  CRoom *pFound = NULL;
  CBundle *pBundle = g_TheArchive.m_Bundles.BundleFind( pCopy->m_nBundleID );
  if( pBundle != NULL )
    pFound = find( pBundle->m_nRoomID );
  return pFound;
}


// Load the Room list from the DB into the memory
//================================================
bool CRooms::load()
{
  // Load from DB
  bool bSuccess = g_TheArchive.m_pDB->RoomsLoad();

  // Get free space (for non-removable only)
  if( bSuccess )
    update();

  return bSuccess;
}


// Update all Rooms info.
// 29.12.2001
//==================================================================================
void CRooms::update()
{
  for( auto curRoom : m_rooms )
  {
    if( ! curRoom->m_bRemovable )
      curRoom->GetDiskSpaceFree();
    curRoom->m_pArchive = &g_TheArchive; // TODO: Looks ugly
    curRoom->GetPrognosis();
  }
}


// Get rid of the Room - delete copies etc.
// TODO: Check how the class diagram helps to understand this method
//===================================================================
bool CRooms::erase( CRoom* pRoom )
{
  bool bSuccess = true;

  // Delete all Copies from this Room
  CFilesCopies *copies = pRoom->GetCopies();
  POSITION copyPos;
  for( copyPos = copies->GetHeadPosition(); copyPos != NULL; )
  {
    CFileCopy *pCurCopy = copies->GetNext( copyPos );

    // Delete the Copy from DB
    bSuccess &= g_TheArchive.m_pDB->CopyDelete( pCurCopy );

    // Delete the Copy from memory (from the list)
    POSITION pos = g_TheArchive.m_Copies.Find( pCurCopy );
    ASSERT( pos );
    g_TheArchive.m_Copies.RemoveAt( pos );

    delete pCurCopy;  // (2)
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
    /*POSITION pos = Find( pRoom );
    ASSERT( pos );
    RemoveAt( pos );*/
    auto it = std::find( m_rooms.begin(), m_rooms.end(), pRoom );
    ASSERT( it == m_rooms.end() );
    m_rooms.erase( it );
  }

  if( bSuccess )
  {
    AfxMessageBox( L"This Room has been removed from the database.\n"
                   L"But you should manualy delete copy files at path\n"
                  + pRoom->getFullName() );
    delete pRoom;
  }
  else
    AfxMessageBox( L"There were some errors during Room deletion.\n" );

  return (bool)0;
}


void CRooms::free()
{
  for( auto curRoom : m_rooms )
    delete curRoom;
  m_rooms.clear();
}