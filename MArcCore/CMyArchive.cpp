// DiskArcher.
// CMyArchive - the main class of the application business logic.
// (C) Marat Mirgaleev, 2001-2014.
// Modifications:
//  (1) 19.01.2002. Log file.
//  (2) 20.01.2002. "Miscelaneous.h" added.
//  (3) 21.01.2002. FileAdd() - Check is it directory.
//  (4) 09.02.2002. Empty database - not when 0 files, but when 1 file!
//                  Open/Create database - changes in logic.
//  (5) 13.02.2002. Changes in Progress Dialog.
//  (6) 16.02.2002. Classes CFilesToArc, CBundles and CRooms created 
//                  and some methods were moved there.
//  (7) 19.02.2002. Copies replacing logic changed.
//  (8) 04.03.2002. Folders added.
//  (9) 03.07.2002. Flags m_bIsWorking and m_bStopWorking added.
//                  Loading process bar.
// (10) 01.11.2002. theDB changed to *pTheDB.
// (11) 19.11.2002. Public progressDlg changed to a member m_pProgressDlg.
// (12) 05.01.2003. Bug in (11) fixing.
// (13) 21.04-22.11.2003. Compression utilities. m_pCompressor added.
// (14) 24.07.2004. Locator reconstruction.
// (15) 11.12.2004. Increased reserve space on disks.
// (16) 23.05.2006. Error tracking improved.
// (17) 20.08.2006. Bug fixed: All controls are disabled after the "Create Rooms
//                  first" message.
// (19) 23.12.2013. Migration to VS 2012 and Unicode.
//                  See the latest modification history on Github.
//==============================================================================

#include "stdafx.h"
#include "CMyArchive.h"
#include <afx.h>
#include "CFileToArc.h"
#include "CRoom.h"
#include "CBundle.h"
#include "CArchiveDB.h"
#include "CFileCopy.h"
#include "IInsertDisk.h"
#include "CFolderToArc.h"
#include "CZipBundle.h"
#include "CFileCompressor.h"
#include "IProgressIndicator.h"
#include "IProgressCtrl.h"
#include "Miscelaneous.h"
#include "MyException.h"

CMyArchive g_TheArchive; // 2014 Moved from MArc2.cpp
// TODO: 2014: I DON'T LIKE THE FACT THAT THERE IS A GLOBAL VARIABLE


// Constructor
//===========================
CMyArchive::CMyArchive()
{
  m_nDefaultCopies = 5; // If there is the respective option in DB,
                        //  it will be gotten from DB later
  m_bIsWorking = false; // (9)
  m_LogFile.m_pArchive = this;
  m_pProgressDlg = NULL;
  m_insertDiskDlg = NULL;
  m_pCompressor = NULL; // (13)
  m_pLocator = NULL;    // (14)
}


// Destructor
//===========================
CMyArchive::~CMyArchive()
{

// TODO: It's very strange that it's done this way
// WHERE IS INCAPSULATION?

// Clear File list
  POSITION pos;
  for( pos = m_FilesToArc.GetHeadPosition(); pos != NULL; )
  {
    CFileToArc *pCurFile = m_FilesToArc.GetNext( pos );
    delete pCurFile;
  }

// Clear the Rooms list
  m_Rooms.free();

// Clear Copies list
  for( pos = m_Copies.GetHeadPosition(); pos != NULL; )
  {
    CFileCopy *pCurCopy = m_Copies.GetNext( pos );
    delete pCurCopy;
  }

// Clear Bundles list
  for( pos = m_Bundles.GetHeadPosition(); pos != NULL; )
  {
    CBundle *pCurBundle = m_Bundles.GetNext( pos );
    delete pCurBundle;
  }

// Clear Folders list
  for( pos = m_FoldersToArc.GetHeadPosition(); pos != NULL; )
  {
    CFolderToArc *pCurFolder = m_FoldersToArc.GetNext( pos );
    delete pCurFolder;
  }

  delete m_pCompressor;
  delete m_insertDiskDlg;
}


// Open the Archive
//===========================
bool CMyArchive::open( IProgressIndicator* i_pProgressIndicator )
{
  bool bSuccess = true;
  m_pProgressIndicator = i_pProgressIndicator;

// Check is there a database within the program directory.
// If there is not, create it
//---------------------------------------------------------

// Let's know the program directory
  TCHAR szExePathName[_MAX_PATH];
  VERIFY( ::GetModuleFileName( AfxGetApp()->m_hInstance, szExePathName, _MAX_PATH ));
// Construct database filename. The directory is the same as the program's directory.
// So, take exe-fullname and replace filename.
  CFileOnDisk dbFile( szExePathName );
  dbFile.m_strName = g_TheArchive.m_pDB->MyDBFilename;

  m_pDB = new CArchiveDB( dbFile.getFullName() );
  m_pDB->m_pArchive = this;

  if( ! dbFile.checkExistence() )
  {
    bSuccess = ( AfxMessageBox( L"Database not found!\nIf it is the first running of the program, it's Ok.\n"
                                L"Do you want to create a new database?",
                                MB_YESNO | MB_ICONEXCLAMATION )==IDYES );
// TODO: Ask DB ID string, store it in m_strArchiveID and somewhere in the database
    if( bSuccess )
    // Create the database
    //---------------------------------------------
      bSuccess = m_pDB->Create();
  }
  else
  // (13) Create a local copy of the database
    // LATER: if( previous session wasn't terminated suddenly )
      bSuccess = m_pDB->preserveDB();
    // LATER: else: AfxMessageBox( Restore the database? )


// Open the database and Load program data from the database
//-----------------------------------------------------------------------------

  if( bSuccess ) // (4) Was just "else"
  {
    ASSERT( m_pProgressIndicator );
    m_pProgressIndicator->init();


    bSuccess = m_pDB->Open();
    m_pProgressIndicator->stepIt(); // Let's count them: 1
    bool bDBOpen =  bSuccess; // (4)
    if( bSuccess )
      bSuccess = m_pDB->CopiesLoad();
    m_pProgressIndicator->stepIt(); // Let's count them: 2
    if( bSuccess )
      bSuccess = m_pDB->FilesLoad();
    m_pProgressIndicator->stepIt(); // Let's count them: 3
    if( bSuccess )
      bSuccess = m_FoldersToArc.Load();
    m_pProgressIndicator->stepIt(); // Let's count them: 4
    int updatedSysFiles;
    if( bSuccess )
      m_nFilesToUpdate = m_FilesToArc.FilesGetStatus( updatedSysFiles );
          // m_nFilesToUpdate here is preliminary
/* (6) Moved after BundlesLoad because we need Bundles to calculate Room's Free Space
    if( bSuccess )
      bSuccess = m_Rooms.RoomsLoad(); */
    m_pProgressIndicator->stepIt(); // Let's count them: 5
    if( bSuccess )
      bSuccess = m_pDB->BundlesLoad();
    m_pProgressIndicator->stepIt(); // Let's count them: 6
    if( bSuccess )
      bSuccess = m_Rooms.load();
    m_pProgressIndicator->stepIt(); // Let's count them: 7

    if( bSuccess )	// (13)
    {
      m_pCompressor = new CFileCompressor();

      bSuccess = loadOptions();
      CZipBundle::m_pCompressor = m_pCompressor;
    }
    m_pProgressIndicator->stepIt(); // Let's count them: 8
      // Eighth steps for now

    if( bDBOpen )
    {
      CString mess = bSuccess ? L"Archive opened successfully" : L"Archive opening error";
      m_LogFile.AddRecord( L"===========", L"===========", mess );
    }
  }

// (13) Get system's Temp directory
  wchar_t tmpDir[_MAX_PATH];
  /*int len =*/ ::GetTempPath( _MAX_PATH, tmpDir );
  m_sTempPath = tmpDir;
  
  return bSuccess;
}


// Close the Archive
//==============================================================================
bool CMyArchive::close()
{
  bool bSuccess = true;

  if( m_pDB->IsOpen() )
    m_LogFile.AddRecord( L"", L"", L"Closing the Archive" );

  bSuccess = m_pDB->Close();
  if( bSuccess )
    delete m_pDB; // Life is hard. But, fortunately, not so long

  return bSuccess;
}


/*
int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
  if( uMsg == BFFM_VALIDATEFAILED )
  {
    AfxMessageBox( "Invalid folder name!" );
    return 1;
  }
  return 0;
}
*/


// Update the Archive - update files copies.
// Distribute files first, don't ask anything after, only write into the Log.
//==============================================================================
OpResult CMyArchive::update()
{
  m_bIsWorking = true;
  m_bStopWorking = false;
  OpResult nResult = OPR_SUCCESSFUL;
  int updatedSysFiles = 0;

  // Analyse file changes
  //---------------------------------
  HCURSOR hc = ::LoadCursor( NULL, IDC_APPSTARTING ); // Cursor: Arrow with Glass
  /*HCURSOR prevC =*/ ::SetCursor( hc );

  // Check is it interrupted by user (13).
  // It also prevents the dialog to stick in left upper corner
  ASSERT( m_pProgressDlg );
  if( m_pProgressDlg->isAborted()
    || m_bStopWorking )  // (13)
    nResult = OPR_USER_STOP; // (16) Was: bContinue = false; // (13)
  else// (13)
  {   // (13)
  //.. Read files' time stamps again ...................
    m_nFilesToUpdate = m_FilesToArc.FilesGetStatus( updatedSysFiles );
  //....................................................
    if( m_pProgressDlg->isAborted() // Interrupted by user
      || m_bStopWorking ) // (13)
      nResult = OPR_USER_STOP; // (16) Was: bContinue = false; // (13)
  }

  if( nResult <= OPR_WARNINGS )
    if(    ( m_nFilesToUpdate == 0 )                         // Nothing changed or
        || (m_nFilesToUpdate == 1 && updatedSysFiles == 1 )) // Only the Database has changed
    {
      AfxMessageBox( _T("Nothing to do:\nThe Archive is up-to-date") );
      nResult = OPR_USER_STOP; // (16) Was: bContinue = false;
    }


// Assign a Room, a Bundle for every file, if it is necessary
//------------------------------------------------------------
  if( nResult <= OPR_NONFATAL_ERRORS )
  {
    POSITION pos;
    m_pProgressDlg->setMaxRange( m_FilesToArc.GetCount() ); 
    m_nFilesToUpdate = 0;  // Let's count it again, more carefully.
                          // For example, we will not count files
                         //  if we don't find an appropriate room for it.
    for( pos = m_FilesToArc.GetHeadPosition(); pos != NULL; )
    {
      CFileToArc *pCurFile = m_FilesToArc.GetNext( pos );

    // Assign a Room, a Bundle for the file, if necessary
    //----------------------------------------------------
      OpResult nCurResult = decideAboutFile( pCurFile );
    //....................................................
      nResult = max( nResult, nCurResult );
      if( nResult >= OPR_FATAL_STOP )
        break;
      
      m_nFilesToUpdate += pCurFile->m_CopyToRooms.size();
      m_pProgressDlg->advance( 1 );
    }
    m_pProgressDlg->analysisDone();
  }
#if 0  // for debugging
  if( m_pProgressDlg )
    if( m_pProgressDlg->m_hWnd )
      int poz = m_pProgressDlg->m_Progress.GetPos();
#endif


// Do copying
//------------------------------------------------------------------------------
  if( nResult <= OPR_NONFATAL_ERRORS )  // (16) Was: if( bContinue )
  {
    OpResult nCurResult = doCopying();  // (16) Was: bContinue = DoCopying();
    nResult = max( nResult, nCurResult );
  }

// Update and Reset data in memory.
//------------------------------------------------------------------------------

// (13) Clear non-actual info in Files, including deletion of temporary files
//..............................................................................
  this->m_FilesToArc.ResetRuntimeData();

// Reload Copies -----------------------------------
// if( bContinue ) - Unconditional !!! 
//      Because there could be copied some files before user break
//  {
  // Delete all them from memory first 
    POSITION pos;
    for( pos = m_Copies.GetHeadPosition(); pos != NULL; )
    {
      CFileCopy *pCurCopy = m_Copies.GetNext( pos );
      delete pCurCopy;
    }
    m_Copies.RemoveAll();
    /*bContinue = - Continue anyway */ m_pDB->CopiesLoad();
    // TODO: RELOAD BUNDLES?
// }

#ifdef _DEBUGGGG // Progress indicator debugging
  if( m_pProgressDlg ) // (12)
    if( m_pProgressDlg->m_hWnd )
      /*int poz =*/ m_pProgressDlg->m_Progress.GetPos();
#endif


// Progress Dialog Status Update
//------------------------------------------------------------------------------
  m_pProgressDlg->finished( m_bStopWorking, nResult );

  m_bIsWorking = false;

  return nResult;

} // End of update()


// Update file copies in Archive.
//==============================================================================
OpResult CMyArchive::doCopying()
{
  OpResult nResult = OPR_SUCCESSFUL;
  bool bCheckedOk = true;

  m_pProgressDlg->setPos( 0 );
  m_pProgressDlg->setMaxRange( m_nFilesToUpdate );


// Removable Rooms first
//-----------------------
  for( auto curRoom : m_Rooms.m_rooms )
  {
    if(    m_pProgressDlg->isAborted() // Interrupted by the user
        || m_bStopWorking )
      // Their order is important! Becase m_bStopWorking is changed during isAborted()!
    {
      nResult = OPR_USER_STOP; // (16) Was: bContinue = false;
      break;
    }

repeat:
    if( curRoom->m_bRemovable )
      if( curRoom->CountFilesBeingCopied() > 0 )
      // If there is any copy to add or replace in the Room, then show the "Insert Disk" dialog
      //=======================================================================================
      {
        int nRet = this->getInsertDlg()->askInsertDiskForCopy( curRoom );

        m_pProgressDlg->isAborted(); // Just to process messages

        const int ID_SKIP_DISK=4; // A hack! TODO
        if( nRet == ID_SKIP_DISK )
        {
          CString mess;
          mess.Format( L"Archive Room #%d skipped", curRoom->m_nRoomID );
          m_LogFile.AddRecord( L"", L"", mess );
          m_pProgressDlg->advance( curRoom->CountAllFiles() );
          nResult = max( OPR_WARNINGS, nResult );
          continue;
        }

        // Check it is Room #N
        if( nResult <= OPR_NONFATAL_ERRORS )
        {
          bCheckedOk = curRoom->CheckLabel();
          if( ! bCheckedOk )
            goto repeat;	// Repeat with the same Room
        }

        if( nResult <= OPR_NONFATAL_ERRORS )
        {
          // Write copies to this Room
          //=========================================================
          OpResult nCurResult = curRoom->doCopying();
          nResult = max( nCurResult, nResult );
          //=========================================================
        }
        else
          if( ( nRet == IDABORT ) || ( nRet == IDCANCEL ) )
            break;  // Stop Archiving

#ifdef _DEBUG
//          /*int poz =*/ m_pProgressDlg->m_Progress.GetPos();
#endif
      } // if Count > 0
      else
        m_pProgressDlg->advance( curRoom->CountAllFiles() );

#ifdef _DEBUG
//    /*int poz =*/ m_pProgressDlg->m_Progress.GetPos();
#endif
  } // for Rooms


  // Then Non-Removable Rooms
  //--------------------------
  if( nResult <= OPR_NONFATAL_ERRORS )  // (16) Was: if( bContinue )
  {
    for( auto curRoom : m_Rooms.m_rooms )
    {
      if( m_pProgressDlg->isAborted() ) // Interrupted by the user
      {
        m_LogFile.AddRecord( L"", L"", L"User break" );

        nResult = OPR_USER_STOP;
        break;
      }
      if( ! curRoom->m_bRemovable )
      {
        if( curRoom->m_nDiskSpaceFree != -1 )  // -1 - the Room is unavailable
        {
        // Check is it Room #N
          bCheckedOk = curRoom->CheckLabel();
          if( bCheckedOk )

          // Write copies to this Room. Write the results to DB or to log
          //=============================================================
          {
            OpResult nCurResult = curRoom->doCopying();
            nResult = max( nCurResult, nResult );
                // (16) Was: bContinue = pCurRoom->DoCopying();
          }
          //=============================================================

          else
          {
            CString mess;
            mess.Format( _T("Archive Room #%d skipped because it has a bad label"), curRoom->m_nRoomID );
// TODO: Counter - "There were N errors!" 
            m_LogFile.AddRecord( L"", L"", mess );
          }
#ifdef _DEBUG
//  /*int poz =*/ m_pProgressDlg->m_Progress.GetPos();
#endif
        }
        else // The Room is unavailable - move the Progress Bar
          m_pProgressDlg->advance( curRoom->CountAllFiles() );
      }
#ifdef _DEBUG
//    /*int poz =*/ m_pProgressDlg->m_Progress.GetPos();
#endif
    } // for
  }
  
  return nResult; // (16) Was: bContinue;

} // End of doCopying()


//==============================================================================
bool CMyArchive::deleteOldestCopyOfFile( CFileToArc* const i_pFile )
{
  bool bSuccess = true;
  CFileCopy* pCopy = m_Copies.GetOldestCopy( i_pFile );
  if( pCopy )
  {
    pCopy->MarkForDeletion();

  // Freeing space because of deleting an old Copy
    CRoom *pRoomDeleteFrom = pCopy->GetRoom();
    if( pRoomDeleteFrom )
      pRoomDeleteFrom->m_nPrognosisFree += pCopy->m_packedSize;
  }
  return bSuccess;
}


// Analyse the file's status and decide what to do with it - add a copy or replace...
// TODO:
//  From network drive - to my local disk. From local disk - to network Room...
//    Then among local phisycal drives. Then among local logical drives.
//==============================================================================
OpResult CMyArchive::decideAboutFile( CFileToArc* const i_pFile )
{
  OpResult nSuccess = OPR_SUCCESSFUL;


  if ( i_pFile->m_nStatus == fsNotFound )
  //----------------------------------------------------------------
  {
    i_pFile->m_addCopy = false; // 2014 Was: i_pFile->m_nCommand = CFileToArc::fcNothing;
    m_LogFile.AddRecord( i_pFile->getFullPath(), i_pFile->m_strName, L"Could not find the file" );
    nSuccess = OPR_WARNINGS;
  }


  int copiesToHave = i_pFile->getRequiredCopiesNum();
  int copiesExist  = m_Copies.GetCopiesCount( i_pFile );


  if ( i_pFile->m_nStatus == fsUpToDate ) // The file has not changed
  //----------------------------------------------------------------------
  {
    if( copiesExist == copiesToHave ) // Just enough copies
    {
      i_pFile->m_addCopy = false; // 2014. Was: i_pFile->m_nCommand = CFileToArc::fcNothing; 
    }
    else
    {
      // Only one of the two following 'for's will actually work

      // Add as many copies as needed
      //.................................................
      for( int i = copiesExist; i < copiesToHave; i++ )
      {
        // Add new copies
        OpResult nCurResult = addCopyOfFile( i_pFile );
        nSuccess = max( nSuccess, nCurResult );
      }

      // Remove all excessive copies (mutually exclusive with the previous 'for')
      for( int i=copiesExist; i > copiesToHave; i-- )
      {
        deleteOldestCopyOfFile( i_pFile );
      }
    }
  } // endif fsUpToDate


  if( i_pFile->m_nStatus == fsNew || i_pFile->m_nStatus == fsChanged )
  //------------------------------------------------------------------
  {
    i_pFile->m_addCopy = true; // 2014 Was: i_pFile->m_nCommand = CFileToArc::fcAddCopy;

    // There were enough copies or even more (for example, when the user decreases number of file copies).
    // We are deleting the oldest copies, then search where should we place new copies.
    // So, if there is 5 copies, we'll leave 4 and add 1 new later.
    //.........................................................................................................
    // This way copies will be re-spread among the Rooms, especially if the Rooms list changes.
    int copiesLeft = copiesExist;
    for( int i=copiesExist; i >= copiesToHave; // >= means that we always need to replace at least one copy
         i-- )
    {
      deleteOldestCopyOfFile( i_pFile );
      copiesLeft--;
    }
    
    // Add new copies
    //.............................................
    for( int i = copiesLeft; i < copiesToHave; i++ )
    {
      OpResult nCurResult = addCopyOfFile( i_pFile );
      nSuccess = max( nSuccess, nCurResult );
    }

  }	// endIf new or changed


  if ( i_pFile->m_nStatus == fsOlder )
  //----------------------------------------------------------------
  {
    m_LogFile.AddRecord( i_pFile->getFullPath(), i_pFile->m_strName,
                         L"Warning: The file on disk is older than in Archive" );
    nSuccess = max( nSuccess, OPR_WARNINGS );
  }

  return nSuccess;

} // decideAboutFile()


// (15) Extracted from "spaghetti" code.
//==============================================================================
OpResult CMyArchive::addCopyOfFile( CFileToArc* const i_pFile )
{
  OpResult nSuccess = OPR_SUCCESSFUL;

// Select a Room - only from accessible Rooms, which has enough space 
//   for the file and which has the least number of this File's Copies
  CRoom *pRoomTo = NULL;
  int leastCopies = 999999;
  for( auto curRoom : m_Rooms.m_rooms )
  {
    if( curRoom->m_nDiskSpaceFree != -1 ) // Is the Room accessible?
    { 
    // To compress or not to compress (for this very Room)?
    //  Refer to the project documentation.
      if( isCompressorDefined() )  // (13)
      {
        if(   ( curRoom->m_nCompressionMode == CRoom::roomCompressionMode::rcmAlways )    // (13)
           || ( curRoom->m_nCompressionMode == CRoom::roomCompressionMode::rcmAllowed &&  // (13)
                i_pFile->m_bCompressIt ))
        {
          if( ! i_pFile->IsPreCompressed() )  // (13)
          // The file could be compressed during negotiation 
          //   with one of previous Rooms
          {
            if( ! i_pFile->PreCompress() )	// (13) Pre-compress file
            {
              nSuccess = OPR_NONFATAL_ERRORS; // TODO: Is it Non-Fatal?
              break;  // (13)
            }
          }
        }
        else // Don't compress
          i_pFile->m_nPredictedCompressedSize = i_pFile->getSize();
      }
      else // Don't compress
        i_pFile->m_nPredictedCompressedSize = i_pFile->getSize();

      if( curRoom->m_nPrognosisFree > 
        i_pFile->m_nPredictedCompressedSize + 65536/*(15)Was:10240* /
                            /*Reserve space for Room's contents and so on*/ )
        // (13) Was: if( pCurRoom->m_nPrognosisFree > pFile->m_nSize + 5000 )
      // Is there enough space? -------------------------------------
      {
        int curCopiesCount = m_Copies.GetCopiesCount( i_pFile, curRoom->m_nRoomID );
        curCopiesCount += i_pFile->CountCopies( curRoom );
        if( curCopiesCount < leastCopies )
        // Yes, this Room has least number of Copies
        {
          leastCopies = curCopiesCount;
          pRoomTo = curRoom;
        }
      }
    }
  } // End of for - Selecting Room

  if( nSuccess <= OPR_NONFATAL_ERRORS )
    if( pRoomTo == NULL )
    {
      m_LogFile.AddRecord( i_pFile->getFullPath(), i_pFile->m_strName,
                           L"Could not find an appropriate Room for a Copy of this File" );
      nSuccess = max( nSuccess, OPR_NONFATAL_ERRORS );  // (16)
      i_pFile->m_addCopy = false; // 2014 Was: i_pFile->m_nCommand = CFileToArc::fcNothing;
    }
    else
    {
      i_pFile->m_addCopy = true; // 2014. Was: i_pFile->m_nCommand = CFileToArc::fcAddCopy;
      i_pFile->m_CopyToRooms.push_back( pRoomTo );
      pRoomTo->m_nPrognosisFree -= i_pFile->m_nPredictedCompressedSize;
    }

  return nSuccess;

} // End of addCopyOfFile()


// (13)
// Very similar code is in CNewFilesLocator::LocatorLoadOptions()...
//==============================================================================
bool CMyArchive::loadOptions()
{
  bool bSuccess = false;

  _RecordsetPtr rsOptions;
  rsOptions.CreateInstance(__uuidof(Recordset)); 
  HRESULT hr;
  try
  {
  // Select all Options
    wchar_t* select = L"SELECT * FROM ProgramOptions"
                      L" WHERE SectionName=\"Archive\"";
    hr = rsOptions->Open( select, m_pDB->m_pConnection, adOpenStatic, adLockReadOnly, adCmdText );
    g_TheArchive.m_pDB->TESTHR( hr );
    while( ! rsOptions->adoEOF )
    {
      _bstr_t  bstrTmp; // Temporary string for type conversion
      _variant_t vtTmp;

      bstrTmp = rsOptions->Fields->Item["OptionName"]->Value;
      CString strOption = bstrTmp;
      strOption.TrimRight();

      bstrTmp = rsOptions->Fields->Item["OptionValue"]->Value;
      CString strValue = bstrTmp;
      strValue.TrimRight();

      if ( strOption == "nDefaultCopies" )
        m_nDefaultCopies = _ttoi(strValue) ;//atoi( strValue );
      if ( strOption == "CompressorName" )
        m_pCompressor->m_strName = strValue;
      if ( strOption == "CompressorPath" )
        m_pCompressor->m_strExePath = strValue;

      rsOptions->MoveNext();
    }
    rsOptions->Close();

    bSuccess = true;
  }
  catch(_com_error &e)
  {
    g_TheArchive.m_pDB->showADOErrors( e, m_pDB->m_pConnection );
  }
  catch(...)
  {
    AfxMessageBox( _T("Some error occured in CMyArchive::LoadOptions().") );
  }

  return bSuccess;
}


//==============================================================================
bool CMyArchive::saveOptions()
{
  bool bSuccess=false;

  // Default number of copies
  CString tmp;
  tmp.Format( L"%d", m_nDefaultCopies );
  bSuccess = m_pDB->optionSave( L"Archive", L"nDefaultCopies", tmp );

  // Compression utility name
  if( bSuccess )
    bSuccess = m_pDB->optionSave( L"Archive", L"CompressorName", m_pCompressor->m_strName );

  // Compression utility path
  if( bSuccess )
    bSuccess = m_pDB->optionSave( L"Archive", L"CompressorPath", m_pCompressor->m_strExePath );

  return bSuccess;
}


// (13)
//==============================================================================
bool CMyArchive::isCompressorDefined()
{
  return ( m_pCompressor != NULL && m_pCompressor->m_strExePath != "" );
}


// 2014
//==============================================================================
const MArcLib::error* CMyArchive::addFile( CString i_fileName )
{
  const MArcLib::error* result = MArcLib::error::getDefault();

// Check if it is a directory
  CFileToArc newFile( i_fileName );
  newFile.getInfo();
  if( newFile.CDiskItem::getType() == CDiskItem::DI_FOLDER )
    result = m_FoldersToArc.AddFolder( i_fileName );
  else
    result = m_FilesToArc.FileAdd( i_fileName );

  return result;
}


// Events
//==============================================================================

/* 2014 Moved to void CMainFrame::OnStartLocator() 
void CMyArchive::onLocatorStart( void )
...
*/
