// DiskArcher.
// CMainFrame class - the main window of the program.
// (C) Marat Mirgaleev, 2001-2002.
// Modifications:
//	(1) 15.01.2002. Log added.
//	(2) 12.02.2002. CopiesWindowUpdate() added.
//	(3) 05.03.2002. OnDropFiles() added.
//	(4) 09.05.2002. Nag screen added.
//	(5) 03.07.2002. OnClose() - interrupt archiving process.
//	(6) 20.08.2002. Locator added.
//	(7)	19.11.2002. Menu disabled during Archiving.
//	(8) 12.01.2003. "Excluded files" window added.
//	(9) 24.09.2003. "Options" dialog.
// (10) 25.07.2004. Locator reconstructed.
// (11) 08.05.2006. "Register" added in the main menu.
//==============================================================================

#include "stdafx.h"
#include "MArc2.h"
#include "MainFrm.h"

#include "MArcCore/CMyArchive.h"
#include "MArcCore/CFileCompressor.h"
#include "MArcCore/CRoom.h"

#include "CFilesToArcView.h"
#include "CFilesToArcFrame.h"
#include "CRoomsFrame.h"
#include "CCopiesFrame.h"
#include "CLogFrame.h"	    // (1)
#include "CCopiesView.h"
#include "LeftView.h"
#include "COptionsDialog.h"	// (9)
#include "FileLocator/CNewFilesLocator.h"
#include "FileLocator/CNewFilesLocatorFrame.h"
#include "CProgressDialog.h"	// (9)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

//==============================================================================
BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_UPDATE_COMMAND_UI(ID_SHOW_WINDOW_ROOMS, OnUpdateWindowRooms)
	ON_COMMAND(ID_SHOW_WINDOW_ROOMS, OnWindowRooms)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_FILESTOARCHIVE, OnUpdateWindowFilesToArchive)
	ON_COMMAND(ID_WINDOW_FILESTOARCHIVE, OnWindowFilesToArchive)
	ON_UPDATE_COMMAND_UI(ID_ARCHIVE_UPDATE, OnUpdateArchiveUpdate)
	ON_COMMAND(ID_ARCHIVE_UPDATE, OnArchiveUpdate)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_COPIES, OnUpdateWindowCopies)
	ON_COMMAND(ID_WINDOW_COPIES, OnWindowCopies)
	ON_WM_CLOSE()
	ON_COMMAND(ID_VIEW_LOG, OnViewLog)
	ON_WM_DROPFILES()
	ON_UPDATE_COMMAND_UI(ID_START_LOCATOR, OnUpdateStartLocator)
	ON_COMMAND(ID_START_LOCATOR, OnStartLocator)
	ON_COMMAND(ID_FILE_OPTIONS, OnFileOptions)
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_FILES, OnUpdateFileCount)	// M
	// Global help commands
	ON_COMMAND(ID_HELP_FINDER, CMDIFrameWnd::OnHelpFinder)
	ON_COMMAND(ID_HELP, CMDIFrameWnd::OnHelp)
	ON_COMMAND(ID_CONTEXT_HELP, CMDIFrameWnd::OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, CMDIFrameWnd::OnHelpFinder)
END_MESSAGE_MAP()

static UINT indicators[] =
{
  ID_SEPARATOR,           // status line indicator
  ID_INDICATOR_FILES
};


//==============================================================================
CMainFrame::CMainFrame()
{
  m_pFilesToArcFrame = NULL;
  m_pRoomsFrame      = NULL;
  m_pCopiesFrame     = NULL;
  m_pLogFrame        = NULL;
  m_pLocator         = NULL;
}


//==============================================================================
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}


// CMainFrame diagnostics
//==============================================================================
#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG


//==============================================================================
void CMainFrame::OnUpdateFileCount(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(); 
// Показ будет в окне "Files To Arc"
}


//==============================================================================
void CMainFrame::OnUpdateWindowRooms(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable(); 
}


//==============================================================================
void CMainFrame::OnWindowRooms() 
{
	if( m_pRoomsFrame != NULL )
		m_pRoomsFrame->ActivateFrame();
	else
		theApp.m_pRoomsTemplate->OpenDocumentFile(NULL);
}


//==============================================================================
void CMainFrame::OnUpdateWindowFilesToArchive(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable(); 
}


//==============================================================================
void CMainFrame::OnWindowFilesToArchive() 
{
	if( m_pFilesToArcFrame != NULL )
		m_pFilesToArcFrame->ActivateFrame();
	else
		theApp.m_pFilesToArcTemplate->OpenDocumentFile(NULL);
}


//==============================================================================
void CMainFrame::OnUpdateArchiveUpdate(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable(); 
}


// TODO: Refactor it
//==============================================================================
OpResult CMainFrame::checksBeforeUpdate()
{
  OpResult nResult = OPR_SUCCESSFUL;

  // Some prepations and preliminary checks...
  //---------------------------------------------

  // Check is there any file for archive
  if( g_TheArchive.m_FilesToArc.GetCount() <= 1 )
  {
    AfxMessageBox( L"Please select files to archive first.\n"
                   L"To do this just drag files from Windows Explorer\n"
                   L"and drop them into the program window." );
    nResult = OPR_USER_STOP;
  }

  // Check is there any room
  if( nResult == OPR_SUCCESSFUL )
    if( g_TheArchive.m_Rooms.GetCount() == 0 )
    {
      AfxMessageBox( L"Please create Archive Rooms first.\n"
                     L"To do this press \"Show Rooms\" button on the toolbar,\n"
                     L"then press \"Create Room\" button." );
      nResult = OPR_USER_STOP;
    }

  // Check is there any unavailable room and calculate free space
  if( nResult == OPR_SUCCESSFUL )
  {
    g_TheArchive.m_Rooms.RoomsUpdate();
    POSITION pos;
    for( pos = g_TheArchive.m_Rooms.GetHeadPosition(); pos != NULL; )
    {
      CRoom *pCurRoom = g_TheArchive.m_Rooms.GetNext( pos );
      if( pCurRoom->m_nDiskSpaceFree == -1 )  // -1 - the Room is unavailable
      {
        ShowRooms();
        m_pRoomsFrame->UpdateList();

        int nYesNo = AfxMessageBox( L"Not all Archive Rooms are available now.\n"
                                    L"Continue?", MB_YESNO );
        nResult = ( nYesNo == IDYES ) ? OPR_WARNINGS : OPR_USER_STOP;
        if( nResult == OPR_WARNINGS )
        {
          CString mess;
          mess.Format( _T("Archive Room #%d is unavailable"), pCurRoom->m_nRoomID );
          g_TheArchive.m_LogFile.AddRecord( "", "", mess );
        }
        break;
      }
    }
  }

  return nResult;
}


// TODO: CMainFrame is not the best place for this method
// Progress Dialog Init
//--------------------------------------------------------------------------------------
OpResult CMainFrame::initProgressDialog()
{
  OpResult nResult = OPR_SUCCESSFUL;
  CProgressDialog* pProgressDlg = NULL;
  try
  {
    pProgressDlg = new CProgressDialog();
      // It will be deleted in its DestroyWindow().

    if ( ! pProgressDlg->Create( IDD_PROGRESS_DIALOG ) )
      nResult = OPR_FATAL_STOP;
  }
  catch(...)
  {
    nResult = OPR_FATAL_STOP;
  }
  if( nResult == OPR_FATAL_STOP )
    AfxMessageBox( L"Error creating copying process dialog." );

  if( nResult <= OPR_WARNINGS )
    pProgressDlg->resetAndShow();

  //...........................................
  g_TheArchive.setProgressDlg( pProgressDlg );
  //...........................................

  return nResult;
}


//==============================================================================
void CMainFrame::OnArchiveUpdate() 
{
  EnableControls( false );

  if( checksBeforeUpdate() <= OPR_WARNINGS )
    if( initProgressDialog() <= OPR_WARNINGS )
      g_TheArchive.update();  /*	(11) Was:
	if( ! g_TheArchive.update() )
	// For example, there isn't any file to archive
		EnableControls( true );	// if success, they will be enabled 
								            // when progress dialog will be closed */
	if( m_pFilesToArcFrame != NULL )
	{
		CLeftView* pView = m_pFilesToArcFrame->GetLeftPane(); 
		pView->TrackSelChange(); // OnSelChanged( NULL, NULL );
	}//		m_pFilesToArcFrame->UpdateList();
}


//==============================================================================
void CMainFrame::OnUpdateWindowCopies(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable(); 
}


//==============================================================================
void CMainFrame::OnWindowCopies() 
{
	WindowCopies();
	CCopiesView* pView = (CCopiesView*)(m_pCopiesFrame->GetActiveView());
														// We have just one View
	pView->m_strPath	 = "";	// Show all
	pView->m_strFilename = "";	//		copies
	pView->ShowCopyList();
}


//==============================================================================
void CMainFrame::OnClose() 
{
	static int bIsStopping = false;		// (5)
	if( g_TheArchive.m_bIsWorking )	// (5) All this branch added
	{
		if( ! bIsStopping	)
		{
			bIsStopping = true;
			if( AfxMessageBox( _T("Abort the archiving process?"), 
								         MB_YESNO ) == IDYES )
				g_TheArchive.m_bStopWorking = true;
			bIsStopping = false;
		}
	}
	else
	{
		g_TheArchive.close();		

		CMDIFrameWnd::OnClose();
	}
}


//==============================================================================
void CMainFrame::WindowCopies()
{
	if( m_pCopiesFrame != NULL )
		m_pCopiesFrame->ActivateFrame();
	else
		theApp.m_pCopiesTemplate->OpenDocumentFile(NULL);
}


//==============================================================================
void CMainFrame::EnableControls( bool bOnOff )
{
	CMenu *pMainMenu = GetMenu();										// (7)
	for( int i=0; i < pMainMenu->GetMenuItemCount(); i++ )				// (7)
		if( bOnOff )													// (7)
			pMainMenu->EnableMenuItem( i, MF_ENABLED | MF_BYPOSITION );	// (7)
		else															// (7)
			pMainMenu->EnableMenuItem( i, MF_BYPOSITION | MF_GRAYED );	// (7)
	if( m_pFilesToArcFrame != NULL )
		m_pFilesToArcFrame->EnableWindow( bOnOff );
	if( m_pRoomsFrame != NULL )
		m_pRoomsFrame->EnableWindow( bOnOff );
	if( m_pCopiesFrame != NULL )
		m_pCopiesFrame->EnableWindow( bOnOff );
	if( m_pLogFrame != NULL )	// (1)
		m_pLogFrame->EnableWindow( bOnOff );
	if( g_TheArchive.m_pLocator != NULL )                 // (10) Revised
    g_TheArchive.m_pLocator->EnableControls( bOnOff );
	m_wndToolBar.EnableWindow( bOnOff );
	if( bOnOff )		// (9)
		DrawMenuBar();	// (9)
}


//==============================================================================
void CMainFrame::ShowRooms()
{
	OnWindowRooms();
}


// (1)
//==============================================================================
void CMainFrame::WindowLog()
{
	if( m_pLogFrame != NULL )
	{
		m_pLogFrame->UpdateList();
		m_pLogFrame->ActivateFrame();
	}
	else
		theApp.m_pLogTemplate->OpenDocumentFile(NULL);
}


// (1)
//==============================================================================
void CMainFrame::OnViewLog() 
{
	WindowLog();	
}


// (2)
//==============================================================================
void CMainFrame::CopiesWindowUpdate()
{
	if( m_pCopiesFrame != NULL )
		m_pCopiesFrame->UpdateList();
}


// (3)
//==============================================================================
void CMainFrame::OnDropFiles(HDROP hDropInfo) 
{
	m_pFilesToArcFrame->DoDropFiles( hDropInfo );
}


// (6)
//==============================================================================
void CMainFrame::OnUpdateStartLocator(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(); 
}


// 2014 Moved here
//==============================================================================
void CMainFrame::OnStartLocator() 
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


// (9)
//==============================================================================
void CMainFrame::OnFileOptions() 
{
	COptionsDialog dlg;
	dlg.m_nDefaulNumberOfCopies = g_TheArchive.m_nDefaultCopies;
	if( g_TheArchive.m_pCompressor != NULL )
	{
		dlg.m_sCompressionUtility = g_TheArchive.m_pCompressor->m_strName;
		dlg.m_sCompressorPath = g_TheArchive.m_pCompressor->m_strExePath;
	}
//-------------------------------
	int result = dlg.DoModal();
//-------------------------------
	if (result==IDOK)
	{
		g_TheArchive.m_pCompressor->m_strName = dlg.m_sCompressionUtility;
		g_TheArchive.m_pCompressor->m_strExePath = dlg.m_sCompressorPath;
		g_TheArchive.m_nDefaultCopies = dlg.m_nDefaulNumberOfCopies;

		g_TheArchive.saveOptions();
	}
}
