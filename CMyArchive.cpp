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
//                  See the modification history on Github.
//==============================================================================

#include "stdafx.h"
#include "CMyArchive.h"
#include <afx.h>
#include "CFileToArc.h"
#include "CRoom.h"
#include "CBundle.h"
#include "CArchiveDB.h"
#include "CFileCopy.h" // M
#include "MainFrm.h" // M
#include "CInsertDiskDlg.h" // M
#include "MArc2.h"
#include "CRoomsFrame.h"
#include "CFolderToArc.h"
#include "CZipBundle.h"
#include "CFileCompressor.h"
#include "CNewFilesLocator.h"       // (14)
#include "CNewFilesLocatorFrame.h"  // (14)
#include "CProgressDialog.h"
#include "Miscelaneous.h"	// (2)


CArchiveDB* g_pTheDB;	// M	The Database to store and get info
// ZZZZ 2014: I DON'T LIKE THE FACT THAT THERE IS A GLOBAL VARIABLE


// Constructor
//===========================
CMyArchive::CMyArchive()
{
  m_nDefaultCopies = 5; // If there is the respective option in DB,
                        //  it will be gotten from DB later
  m_bIsWorking = false; // (9)
  m_LogFile.m_pArchive = this;
  m_pProgressDlg = NULL;
  m_pCompressor = NULL; // (13)
  m_pLocator = NULL;    // (14)
}


// Destructor
//===========================
CMyArchive::~CMyArchive()
{
// Clear File list
	POSITION pos;
	for( pos = m_FilesToArc.GetHeadPosition(); pos != NULL; )
	{
		CFileToArc *pCurFile = m_FilesToArc.GetNext( pos );
		delete pCurFile;
	}

// Clear Room list
	for( pos = m_Rooms.GetHeadPosition(); pos != NULL; )
	{
		CRoom *pCurRoom = m_Rooms.GetNext( pos );
		delete pCurRoom;
	}

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

	delete m_pCompressor;	// (13)
}


// Open the Archive
//===========================
bool CMyArchive::open()
{
	bool bSuccess = true;	// (4) false;


// Check is there a database within the program directory.
// If there is not, create it
//---------------------------------------------------------

// Let's know the program directory
	TCHAR szExePathName[_MAX_PATH];
	VERIFY( ::GetModuleFileName( AfxGetApp()->m_hInstance, szExePathName, _MAX_PATH ));
// Construct database filename. The directory is the same as the program's directory.
// So, take exe-fullname and replace filename.
	CFileOnDisk dbFile( szExePathName );
	dbFile.m_strName = MyDBFilename;

	g_pTheDB = new CArchiveDB;	// (10)
	m_pDB = g_pTheDB;
	g_pTheDB->m_strDBFilename = dbFile.getFullName();
	g_pTheDB->m_pArchive = this;

	if( ! dbFile.checkExistence() )
	{
		bSuccess = ( AfxMessageBox( _T("Database not found!\nIf it is the first running of the program, it's Ok.\n")
								    _T("Do you want to create a new database?"), 
								    MB_YESNO | MB_ICONEXCLAMATION )==IDYES );
// TODO: Ask DB ID string, store it in m_strArchiveID and somewhere in the database
		if( bSuccess )
		// Create the database
		//---------------------------------------------
			bSuccess = g_pTheDB->Create();
	}
	else
	// (13) Create a local copy of the database
		// LATER: if( previous session wasn't terminated suddenly )
			bSuccess = g_pTheDB->PreserveDB();
		// LATER: else: AfxMessageBox( Restore the database? )

	
// Open the database and Load program data from the database
//-----------------------------------------------------------------------------
	
	if( bSuccess ) // (4) Was just "else"
	{
	// (9) Create the CProgressCtrl as a child of the status bar positioned
	//	   over the first pane.
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		RECT rc;
		pMainFrame->m_wndStatusBar.GetItemRect (0, &rc);
		rc.top += 2;	rc.bottom -= 2;
		CProgressCtrl wndProgress;
		VERIFY( wndProgress.Create( WS_CHILD | WS_VISIBLE, rc,
                                    &(pMainFrame->m_wndStatusBar), 1 ));
		wndProgress.SetRange( 0, 8 );	// Yes, we have eight steps at the moment
// zz		was wndProgress.SetRange( 0, 7 );	// Yes, we have seven steps at the moment
		wndProgress.SetStep(1);


//		theDB.m_pArchive = this;  Moved up
		bSuccess = g_pTheDB->Open();
		wndProgress.StepIt();	// (9) 1
		bool bDBOpen =  bSuccess;	// (4)
		if( bSuccess )
			bSuccess = g_pTheDB->CopiesLoad();
		wndProgress.StepIt();	// (9) 2
		if( bSuccess )
			bSuccess = g_pTheDB->FilesLoad();
		wndProgress.StepIt();	// (9) 3
		if( bSuccess )
			bSuccess = m_FoldersToArc.Load();
		wndProgress.StepIt();	// (9) 4
		if( bSuccess )
			m_nFilesToUpdate = m_FilesToArc.FilesGetStatus();
          // m_nFilesToUpdate here is preliminary
/* (6)	Moved after BundlesLoad because we need Bundles to calculate Room's Free Space
		if( bSuccess )
			bSuccess = m_Rooms.RoomsLoad(); */
		wndProgress.StepIt();	// (9) 5
		if( bSuccess )
			bSuccess = g_pTheDB->BundlesLoad();
		wndProgress.StepIt();	// (9) 6
		if( bSuccess )
			bSuccess = m_Rooms.RoomsLoad();
		wndProgress.StepIt();	// (9) 7

		if( bSuccess )	// (13)
		{
			m_pCompressor = new CFileCompressor();

			bSuccess = loadOptions();
			CZipBundle::m_pCompressor = m_pCompressor;
		}
		wndProgress.StepIt();	// Eighth step

	// (1) Write to Log	
		if( bDBOpen )	// (4)
		{
			CString mess = bSuccess ? "Archive opened successfully" : "Archive opening error";
			m_LogFile.AddRecord( "===========", "===========", mess );
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

// (1) Write to Log
	if( g_pTheDB->IsOpen() )	// (4)
		m_LogFile.AddRecord( "", "", "Closing the Archive" );

	bSuccess = g_pTheDB->Close();
	if( bSuccess )		// (10)
		delete g_pTheDB;	// (10)
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
OpResult/*(16) Was int */ CMyArchive::update()
{
	m_bIsWorking = true;	// (9)
	m_bStopWorking = false;	// (9)
	OpResult nResult = OPR_SUCCESSFUL; // (16) Was: bool bContinue = true;
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();

// Some prepations and preliminary checks...
//---------------------------------------------

// Check is there any file for archive
	if( m_FilesToArc.GetCount() <= 1 ) // (4) Was  = 0
	{
		AfxMessageBox( _T("Please select files to archive first.\n")
			             _T("To do this just drag files from Windows Explorer\n")
			             _T("and drop them into the program window.") );
		nResult = OPR_USER_STOP; // (16) Was: bContinue = false;
	}

// Check is there any room
	if( nResult == OPR_SUCCESSFUL )  // (16) Was: if( bContinue )
		if( m_Rooms.GetCount() == 0 )
		{
			AfxMessageBox( _T("Please create Archive Rooms first.\n")
				_T("To do this press \"Show Rooms\" button on the toolbar,\n")
				_T("then press \"Create Room\" button.") );
		  nResult = OPR_USER_STOP; // (16) Was: bContinue = false;
		}

// Check is there any unavailable room and calculate free space
	if( nResult == OPR_SUCCESSFUL )  // (16) Was: if( bContinue )
	{
		m_Rooms.RoomsUpdate();
			// (6) RoomsUpdate(); // 29.12.2001
		POSITION pos;
		for( pos = m_Rooms.GetHeadPosition(); pos != NULL; )
		{
			CRoom *pCurRoom = m_Rooms.GetNext( pos );
			if( pCurRoom->m_nDiskSpaceFree == -1 )  // -1 - the Room is unavailable
			{
				pMainFrame->ShowRooms();
				CRoomsFrame* pRoomsFrame = pMainFrame->m_pRoomsFrame;
				pRoomsFrame->UpdateList();
				int nYesNo = AfxMessageBox( _T("Not all Archive Rooms are available now.\n")
							                      _T("Continue?"), MB_YESNO );
        nResult = ( nYesNo == IDYES ) ? OPR_WARNINGS : OPR_USER_STOP;
	      /* (16) Was: 
        bContinue = ( AfxMessageBox( "Not all Archive Rooms are available now.\n"
							   "Continue?", MB_YESNO /* | MB_ICONEXCLAMATION* / )==IDYES );*/
				if( nResult == OPR_WARNINGS )  // (16) Was: if( bContinue )
				{
					CString mess;
					mess.Format( _T("Archive Room #%d is unavailable"), pCurRoom->m_nRoomID );
					m_LogFile.AddRecord( "", "", mess );
				}
				break;
			}
		}
	}


// Progress Dialog Init
//--------------------------------------------------------------------------------------
	if( nResult <= OPR_WARNINGS )  // (16) Was: if( bContinue )
	{
		try	// (11)
		{
			m_pProgressDlg = new CProgressDialog();
				// It will be deleted in its DestroyWindow().

			if ( ! m_pProgressDlg->Create( IDD_PROGRESS_DIALOG ) )
				nResult = OPR_FATAL_STOP;  // (16) Was: bContinue = false;
		}
		catch(...)				// (11)
		{
			nResult = OPR_FATAL_STOP;  // (16) Was: bContinue = false;	// (11)
		}
		if( nResult == OPR_FATAL_STOP ) // (16) Was: if( ! bContinue )		// (11)
			AfxMessageBox( _T("Error creating copying process dialog.") );		// (11)
	}

	if( nResult <= OPR_WARNINGS )  // (16) Was: if( bContinue )
	{
		m_pProgressDlg->ResetAndShow();
		// (5) int maxProgress = m_FilesToArc.GetCount() * 30 + m_FilesToArc.GetCount();
			// * 30 - Copying is 30 times longer operation than Analysing + for Analyze
		/* (5) Was: short maxProgress = (short)( m_FilesToArc.GetCount() + m_FilesToArc.GetCount() / 10 );
			// + 10% for changing analyse and rooms assignment */
		// (5) progressDlg.SetMaxRange( maxProgress ); 


	// Analyse file changes
	//---------------------------------

		HCURSOR hc = ::LoadCursor( NULL, IDC_APPSTARTING );	// Cursor: Arrow with Glass
		/*HCURSOR prevC =*/ ::SetCursor( hc );

	// Check is it interrupted by user (13).
	// It also prevents the dialog to stick in left upper corner
		if( m_pProgressDlg->IsAborted()	// (13)
		 || m_bStopWorking )			// (13)
			nResult = OPR_USER_STOP; // (16) Was: bContinue = false;			// (13)
		else							// (13)
		{								// (13)
    //.. Read files' time stamps again ...................
      m_nFilesToUpdate = m_FilesToArc.FilesGetStatus();
    //....................................................
			if( m_pProgressDlg->IsAborted() 	// Interrupted by user (13)
			 || m_bStopWorking )				// (13)
  			nResult = OPR_USER_STOP; // (16) Was: bContinue = false;			// (13)
		}

    if( nResult <= OPR_WARNINGS )
      if( m_nFilesToUpdate == 0 || m_nFilesToUpdate == 1 ) // "One" because of the Database - it changes always
                                                          // TODO: Not sure it changes really always - try repetive 'Go'
      {
        AfxMessageBox( _T("Nothing to do:\nThe Archive is up-to-date") );
        nResult = OPR_USER_STOP; // (16) Was: bContinue = false;
      }
  }


// Assign a Room, a Bundle for every file, if it is necessary
//------------------------------------------------------------
  if( nResult <= OPR_NONFATAL_ERRORS )
	{
		POSITION pos;
		m_pProgressDlg->SetMaxRange( m_FilesToArc.GetCount() ); 
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
			m_pProgressDlg->Advance( 1 );
		}
		m_pProgressDlg->m_CopyingLabel.EnableWindow();
		m_pProgressDlg->m_AnalysingBmp.ShowWindow(SW_NORMAL);
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
//	if( bContinue )	- Unconditional !!! 
//			Because there could be copied some files before user break
//	{
	// Delete all them from memory first 
		POSITION pos;
		for( pos = m_Copies.GetHeadPosition(); pos != NULL; )
		{
			CFileCopy *pCurCopy = m_Copies.GetNext( pos );
			delete pCurCopy;
		}
		m_Copies.RemoveAll();
		/*bContinue = - Continue anyway */ g_pTheDB->CopiesLoad();
//	}

#ifdef _DEBUG
	if( m_pProgressDlg )	// (12)
		if( m_pProgressDlg->m_hWnd )
			/*int poz =*/ m_pProgressDlg->m_Progress.GetPos();
#endif


// Progress Dialog Status Update
//------------------------------------------------------------------------------
  if( nResult <= OPR_NONFATAL_ERRORS )  // (16) Was: if( bContinue )
	{
		m_pProgressDlg->m_CopyingBmp.ShowWindow(SW_NORMAL);
		m_pProgressDlg->m_StopButton.EnableWindow( FALSE );
		m_pProgressDlg->m_OkButton.EnableWindow();
		m_pProgressDlg->m_OkButton.SetFocus();
		if( m_bStopWorking || m_pProgressDlg->m_bIsAborted )	// (12)
			m_pProgressDlg->m_SuccessLabel.SetWindowText( _T("User break") );//(12)
		if( nResult == OPR_WARNINGS ) // (16)
			m_pProgressDlg->m_SuccessLabel.SetWindowText( _T("There were some warnings") );
		if( nResult == OPR_NONFATAL_ERRORS ) // (16)
			m_pProgressDlg->m_SuccessLabel.SetWindowText(
                                           _T("There were errors. Check the Log") );
		m_pProgressDlg->m_SuccessLabel.ShowWindow(SW_NORMAL);
	}
	if( nResult >= OPR_FATAL_STOP ) // (16) Was: if( ! bContinue )
		if( m_pProgressDlg )	// (12)
			m_pProgressDlg->DestroyWindow();
    else                                                      // (17)
   		pMainFrame->EnableControls( true ); // (17)
        // Otherwise it wiil be called in m_pProgressDlg->DestroyWindow()


//	RoomsUpdate(); // 29.12.2001   Free space is updated in CRoom::DoCopying()
	if( pMainFrame->m_pRoomsFrame != NULL )
		pMainFrame->m_pRoomsFrame->UpdateList();
	pMainFrame->CopiesWindowUpdate();	// (4)

// m_pProgressDlg can't be deleted here because it is opened at the moment
//		and it waits for user reaction
	m_bIsWorking = false;	// (9)
	
	return nResult; // (16) Was: bContinue;

} // End of update()


// Update file copies in Archive.
//==============================================================================
OpResult /*(16) bool*/ CMyArchive::doCopying()
{
	OpResult nResult = OPR_SUCCESSFUL; // (16)  Was: bool bContinue = true;
	bool bCheckedOk = true;
	
	CInsertDiskDlg insDlg;

	m_pProgressDlg->SetPos( 0 );
	m_pProgressDlg->SetMaxRange( m_nFilesToUpdate );


// Removable Rooms first
//-----------------------
	POSITION roomPos;
	CRoom *pCurRoom;
	for( roomPos = m_Rooms.GetHeadPosition(); roomPos != NULL; )
	{
		if(   m_pProgressDlg->IsAborted() 	// Interrupted by user
			 || m_bStopWorking/*(9)*/ )
				// Their order is important! Becase m_bStopWorking is changed
				//	during IsAborted() !
		{
   		nResult = OPR_USER_STOP; // (16) Was: bContinue = false;
			break;
		}

		pCurRoom = m_Rooms.GetNext( roomPos );

repeat:
		if( pCurRoom->m_bRemovable )
			if( pCurRoom->CountFilesBeingCopied() > 0 )
			// If there is any copy to add or replace in the Room,
			//	then show the "Insert Disk" dialog
			//======================================================
			{
				CString mess;
				mess.Format( _T("Please insert the disk labeled as\n\"Archive Room #%d\"")
						         _T("\ninto drive %s, then press \"OK\""), 
    								 pCurRoom->m_nRoomID, pCurRoom->m_strDrive );
				insDlg.m_InsDiskLabel = mess;
				int nRet = insDlg.DoModal();
				AfxGetMainWnd()->UpdateWindow();
				m_pProgressDlg->IsAborted(); // Just to process messages
        int nYesNo;
        switch ( nRet )
				{
				case -1: 
					AfxMessageBox( _T("'Insert Disk' dialog box could not be created!") );
	        nResult = OPR_FATAL_STOP; // (16) Was: bContinue = false;
					break;

				case IDABORT:
				case IDCANCEL:	// CANCEL - for pressing a cross in the window's
                        //   right top corner
					nYesNo = AfxMessageBox( _T("Abort the archiving process?"), MB_YESNO );
          if( nYesNo == IDYES )
             nResult = OPR_USER_STOP;
						// (16) Was: bContinue = ! ( AfxMessageBox(
            //            "Abort the archiving process?", MB_YESNO ) == IDYES );
					break;

				case IDOK:
					// Ok, continue archiving
					break;

        case ID_SKIP_DISK: // Skip this Room, proceed with next Room
          mess.Format( _T("Archive Room #%d skipped"), pCurRoom->m_nRoomID );
          m_LogFile.AddRecord( "", "", mess );
          m_pProgressDlg->Advance( pCurRoom->CountAllFiles() );
          nResult = max( OPR_WARNINGS, nResult );
          continue;
          break;

				default:
					AfxMessageBox( _T("Sudden Error in 'DoCopying'") );
	        nResult = OPR_FATAL_STOP; // (16) Added
					break;
				}

			// Check is it Room #N
        if( nResult <= OPR_NONFATAL_ERRORS )  // (16) Was: if( bContinue )
				{
					bCheckedOk = pCurRoom->CheckLabel();
					if( ! bCheckedOk )
						goto repeat;	// Repeat with the same Room
				}

        if( nResult <= OPR_NONFATAL_ERRORS )  // (16) Was: if( bContinue )
        {
				// Write copies to this Room
				//=========================================================
          OpResult nCurResult = pCurRoom->doCopying();
						    // (16) Was: bContinue = pCurRoom->DoCopying();
          nResult = max( nCurResult, nResult );
				//=========================================================
        }
				else
					if( ( nRet == IDABORT ) || ( nRet == IDCANCEL ) )	// 29.12.2001
						break;	// Stop Archiving

#ifdef _DEBUG
					/*int poz =*/ m_pProgressDlg->m_Progress.GetPos();
#endif
			} // if Count > 0
			else
				m_pProgressDlg->Advance( pCurRoom->CountAllFiles() );

#ifdef _DEBUG
	  /*int poz =*/ m_pProgressDlg->m_Progress.GetPos();
#endif
	} // for Rooms


// Then Non-Removable Rooms
//--------------------------
  if( nResult <= OPR_NONFATAL_ERRORS )  // (16) Was: if( bContinue )
	{
		for( roomPos = m_Rooms.GetHeadPosition(); roomPos != NULL; )
		{
			if( m_pProgressDlg->IsAborted() )	// Interrupted by user
			{
			// (1) Write to Log	
				m_LogFile.AddRecord( "", "", "User break" );

				nResult = OPR_USER_STOP; // (16) Was: bContinue = false;
				break;
			}
			CRoom *pCurRoom = m_Rooms.GetNext( roomPos );
			if( ! pCurRoom->m_bRemovable )
			{
				if( pCurRoom->m_nDiskSpaceFree != -1 )  // -1 - the Room is unavailable
				{
				// Check is it Room #N
					bCheckedOk = pCurRoom->CheckLabel();
					if( bCheckedOk )

					// Write copies to this Room. Write the results to DB or to log
					//=============================================================
          {
            OpResult nCurResult = pCurRoom->doCopying();
            nResult = max( nCurResult, nResult );
						    // (16) Was: bContinue = pCurRoom->DoCopying();
          }
					//=============================================================

					else
					{
						CString mess;
						mess.Format( _T("Archive Room #%d skipped because it has a bad label"), 
									 pCurRoom->m_nRoomID );
// TODO: Counter - "There were N errors!" 
						m_LogFile.AddRecord( "", "", mess );
					}
#ifdef _DEBUG
					/*int poz =*/ m_pProgressDlg->m_Progress.GetPos();
#endif
				}
				else	// The Room is unavailable - move the Progress Bar
					m_pProgressDlg->Advance( pCurRoom->CountAllFiles() );
			}
#ifdef _DEBUG
			/*int poz =*/ m_pProgressDlg->m_Progress.GetPos();
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
      pRoomDeleteFrom->m_nPrognosisFree += pCopy->m_nPackedSize;
  }
  return bSuccess;
}


// Analyse the file's status and decide what to do with it - add a copy or replace...
// TODO:
//	From network drive - to my local disk. From local disk - to network Room...
//		Then among local phisycal drives. Then among local logical drives.
//==============================================================================
OpResult CMyArchive::decideAboutFile( CFileToArc* const i_pFile )
{
  OpResult nSuccess = OPR_SUCCESSFUL;


  if ( i_pFile->m_nStatus == fsNotFound )
  //----------------------------------------------------------------
  {
    i_pFile->m_nCommand = CFileToArc::fcNothing;
    m_LogFile.AddRecord( i_pFile->getFullPath(), i_pFile->m_strName,
                         "Could not find the file" );
    nSuccess = OPR_WARNINGS;
  }


  int nCopiesToHave = i_pFile->getRequiredCopiesNum();
  int nFileCopies = m_Copies.GetCopiesCount( i_pFile );


  if ( i_pFile->m_nStatus == fsUpToDate ) // The file has not changed
  //----------------------------------------------------------------------
  {
    if( nFileCopies == nCopiesToHave ) // Just enough copies
    {
      i_pFile->m_nCommand = CFileToArc::fcNothing; 
    }
    else
    { 
      // Only one of the two following 'for's will actually work

      // Add as many copies as needed
      //.................................................
      for( int i = nFileCopies; i < nCopiesToHave; i++ )
      {
        // Add new copies
        OpResult nCurResult = addCopyOfFile( i_pFile );
        nSuccess = max( nSuccess, nCurResult );
      }

      // Remove all excessive copies (mutually exclusive with the previous 'for')
      for( int i=nFileCopies; i > nCopiesToHave; i-- )
      {
        deleteOldestCopyOfFile( i_pFile );
      }
    }
  } // endif fsUpToDate


  if( i_pFile->m_nStatus == fsNew || i_pFile->m_nStatus == fsChanged )
  //------------------------------------------------------------------
  {
    i_pFile->m_nCommand = CFileToArc::fcAddCopy;

    // There were enough copies or even more (for example, when the user decreases number of file copies).
    // We are deleting the oldest copies, then search where should we place new copies.
    //.........................................................................................................
    // This way copies will be re-spread among the Rooms, especially if the Rooms list changes.
    for( int i=nFileCopies; i >= nCopiesToHave; // >= means that we always need to replace at least one copy
         i-- )
    {
      deleteOldestCopyOfFile( i_pFile );
    }
    
    // Add new copies
    //.............................................
    for( int i = nFileCopies; i < nCopiesToHave; i++ )
    {
      OpResult nCurResult = addCopyOfFile( i_pFile );
      nSuccess = max( nSuccess, nCurResult );
    }

  }	// endIf new or changed


  if ( i_pFile->m_nStatus == fsOlder )
  //----------------------------------------------------------------
  {
    m_LogFile.AddRecord( i_pFile->getFullPath(), i_pFile->m_strName,
                         "Warning: The file on disk is older than in Archive" );
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
//	 for the file and which has the least number of this File's Copies
  CRoom *pRoomTo = NULL;
  int leastCopies = 999999;
  POSITION pos;
  for( pos = m_Rooms.GetHeadPosition(); pos != NULL; )
  {
    CRoom *pCurRoom = m_Rooms.GetNext( pos );
    if( pCurRoom->m_nDiskSpaceFree != -1 )	// Is the Room accessible?
    { 
		// To compress or not to compress (for this very Room)?
		//		Refer to the project documentation.
			if( isCompressorDefined() )									// (13)
			{
				if(	   ( pCurRoom->m_nCompressionMode == rcmAlways )	// (13)
		  			|| ( pCurRoom->m_nCompressionMode == rcmAllowed &&  // (13)
			  			   i_pFile->m_bCompressIt ))						// (13)
				{
					if( ! i_pFile->IsPreCompressed() )		// (13)
					// The file could be compressed during negotiation 
					//	with one of previous Rooms
					{
						if( ! i_pFile->PreCompress() )	// (13) Pre-compress file
            {
						  nSuccess = OPR_NONFATAL_ERRORS; // TODO: Is it Non-Fatal?
							break;							// (13)
            }
					}
				}
				else	// Don't compress
				  i_pFile->m_nPredictedCompressedSize = i_pFile->m_nSize;
			}
			else	// Don't compress
				i_pFile->m_nPredictedCompressedSize = i_pFile->m_nSize;

			if( pCurRoom->m_nPrognosisFree > 
				i_pFile->m_nPredictedCompressedSize + 65536/*(15)Was:10240* /
                            /*zzzReserve space for Room's contents and so on*/ )
				// (13) Was: if( pCurRoom->m_nPrognosisFree > pFile->m_nSize + 5000 )
			// Is there enough space? -------------------------------------
			{
				int curCopiesCount = m_Copies.GetCopiesCount( i_pFile, pCurRoom->m_nRoomID );
        curCopiesCount += i_pFile->CountCopies( pCurRoom );
				if( curCopiesCount < leastCopies )
				// Yes, this Room has least number of Copies
				{
					leastCopies = curCopiesCount;
					pRoomTo = pCurRoom;
				}
			}
		}
	} // End of for - Selecting Room

	if( nSuccess <= OPR_NONFATAL_ERRORS ) // (16) Was: if( bSuccess )
		if( pRoomTo == NULL )
		{
			m_LogFile.AddRecord( i_pFile->getFullPath(), i_pFile->m_strName,
		             "Could not find an appropriate Room for a Copy of this File" );
      nSuccess = max( nSuccess, OPR_NONFATAL_ERRORS );  // (16)
			i_pFile->m_nCommand = CFileToArc::fcNothing;
		}
		else
		{
			i_pFile->m_nCommand = CFileToArc::fcAddCopy;
			i_pFile->m_CopyToRooms.push_back( pRoomTo );
			pRoomTo->m_nPrognosisFree -= i_pFile->m_nPredictedCompressedSize;
		}

  return nSuccess;  // (16) Was: bSuccess;

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
		wchar_t* select = _T("SELECT * FROM ProgramOptions")
						 _T(" WHERE SectionName=\"Archive\"");
		hr = rsOptions->Open( select, g_pTheDB->m_pConnection, adOpenStatic,
							            adLockReadOnly, adCmdText );
    TESTHR( hr );
		while( ! rsOptions->adoEOF )
		{
			_bstr_t  bstrTmp; // Temporary string for type conversion
			_variant_t vtTmp;
			
      bstrTmp = rsOptions->Fields->Item["OptionName"]->Value;
			CString strOption = (LPCSTR)bstrTmp;
			strOption.TrimRight();

      bstrTmp = rsOptions->Fields->Item["OptionValue"]->Value;
			CString strValue = (LPCSTR)bstrTmp;
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
	// Notify the user of errors if any
		ShowADOErrors( e, g_pTheDB->m_pConnection );
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
	tmp.Format( _T("%d"), m_nDefaultCopies );
	bSuccess = g_pTheDB->optionSave( "Archive", "nDefaultCopies", tmp );

// Compression utility name
	if( bSuccess )
		bSuccess = g_pTheDB->optionSave( "Archive", "CompressorName",
									                   m_pCompressor->m_strName );

// Compression utility path
	if( bSuccess )
		bSuccess = g_pTheDB->optionSave( "Archive", "CompressorPath",
									                   m_pCompressor->m_strExePath );

	return bSuccess;
}


// (13)
//==============================================================================
bool CMyArchive::isCompressorDefined()
{
	return ( m_pCompressor != NULL && m_pCompressor->m_strExePath != "" );
}


// Events
//==============================================================================

// (14)
void CMyArchive::onLocatorStart( void )
{
  if( m_pLocator == NULL )
  {
    m_pLocator = new CNewFilesLocator();
 	  m_pLocator->Init();
	  ((CMainFrame*)AfxGetMainWnd())->EnableControls( false );
  }
  else
    if( m_pLocator->m_pFrame != NULL )

	m_pLocator->m_pFrame->ActivateFrame();
}
