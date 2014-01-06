// DiskArcher.
// MArc2.cpp : Defines the class behaviors for the application.
// (C) Marat Mirgaleev, 2001-2002.
// Modifications:
//	(1) 05.03.2002. AddFiles() - Check is it directory.
//					Enable drag'n'drop.
//	(2) 01.05.2002. Changes in About box.
//	                CoInitialize() and CoUnitialize() moved here.
//	(3) 10.05.2002. CAboutDlg moved into a separate file.
//					CheckRegDataConformity() and CheckRegistration() added.
//					Tip of the Day added.
//	(4) 02.09.2002. Locator added.
//	(5) 02.10.2002. try block.
//	(6) 12.01.2003. "Excluded files" template.
//	(7) 02.12.2003. Processing of a return code of CFilesToArc::Add().
//	(8) 03.03.2004. GetPassedDays() added.
//  (9) 24.05.2006. theArchive renamed into g_TheArchive.
//                  Copy protection changed.
//==============================================================================

#include "stdafx.h"
#include "MArc2.h"

#include "MainFrm.h"
#include "CFilesToArcFrame.h"
#include "CFilesToArcDoc.h"
#include "CFileToArc.h"		// (1)
#include "LeftView.h"

#include "CMyArchive.h"		// M
#include "CArchiveDB.h"
#include "CRoomsDoc.h"		// M
#include "CRoomsFrame.h"	// M
#include "CRoomsView.h"		// M

#include "CCopiesDoc.h"		// M
#include "CCopiesFrame.h"	// M
#include "CCopiesView.h"	// M

#include "CLogDoc.h"		// M
#include "CLogFrame.h"		// M
#include "CLogView.h"		// M

#include "CNewFilesLocatorDoc.h"	// (4)
#include "CNewFilesLocatorFrame.h"
#include "CNewFilesLocatorView.h"

#include "CLocatorExcludedFrame.h"	// (6)
#include "CLocatorExcludedDoc.h"
#include "CLocatorExcludedView.h"

#include "CAboutDlg.h"	// (3)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//= GLOBALS ====================================================================
/*const Не получается*/ wchar_t* strMyComp = _T("My Computer");
                                                // LATER: Или в ресурс ее?
CMArc2App theApp; // The one and only CMArc2App object
CMyArchive g_TheArchive; // (9) Was theArchive


//==============================================================================
BEGIN_MESSAGE_MAP(CMArc2App, CWinApp)
//	ON_COMMAND(CG_IDS_TIPOFTHEDAY, ShowTipOfTheDay)	// (3)
	//{{AFX_MSG_MAP(CMArc2App)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_UPDATE_COMMAND_UI(ID_HELP_TIPOFTHEDAY, OnUpdateHelpTipOfTheDay)
	ON_COMMAND(ID_HELP_TIPOFTHEDAY, OnHelpTipOfTheDay)
	//}}AFX_MSG_MAP
	// Standard file based document commands
END_MESSAGE_MAP()


//==============================================================================
CMArc2App::CMArc2App()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// CMArc2App initialization
//==============================================================================
BOOL CMArc2App::InitInstance()
{
	if(FAILED(CoInitialize(NULL)))	// (2)
		return FALSE;
/*
#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
*/
	SetRegistryKey(_T("DiskArcher"));
//(3)	LoadStdProfileSettings(0);  // Load standard INI file options (including MRU)


// Register the application's document templates.  Document templates
//  serve as the connection between documents, frame windows and views.
//--------------------------------------------------------------------------

// FilesToArc window
	m_pFilesToArcTemplate = new CMultiDocTemplate( IDR_FILESTOARC,
		RUNTIME_CLASS( CFilesToArcDoc ), RUNTIME_CLASS( CFilesToArcFrame ),
		RUNTIME_CLASS( CLeftView ));
	AddDocTemplate( m_pFilesToArcTemplate );

// Archive Rooms window
	m_pRoomsTemplate = new CMultiDocTemplate( IDR_ROOMS,
		RUNTIME_CLASS( CRoomsDoc ), RUNTIME_CLASS( CRoomsFrame ),
		RUNTIME_CLASS( CRoomsView ) );
	AddDocTemplate( m_pRoomsTemplate );

// File Copies window
	m_pCopiesTemplate = new CMultiDocTemplate( IDR_COPIES,
		RUNTIME_CLASS( CCopiesDoc ), RUNTIME_CLASS( CCopiesFrame ),
		RUNTIME_CLASS( CCopiesView ) );
	AddDocTemplate( m_pCopiesTemplate );

// Log window
	m_pLogTemplate = new CMultiDocTemplate( IDR_LOG,
		RUNTIME_CLASS( CLogDoc ), RUNTIME_CLASS( CLogFrame ),
		RUNTIME_CLASS( CLogView ) );
	AddDocTemplate( m_pLogTemplate );

// (4) Locator window
	m_pLocatorTemplate = new CMultiDocTemplate( IDR_LOCATOR,
		RUNTIME_CLASS( CNewFilesLocatorDoc ), RUNTIME_CLASS( CNewFilesLocatorFrame ),
		RUNTIME_CLASS( CNewFilesLocatorView ) );
	AddDocTemplate( m_pLocatorTemplate );

// (6) "Excluded files" window
	m_pLocatorExclTemplate = new CMultiDocTemplate( IDR_LOCATOR_EXCL,
		RUNTIME_CLASS( CLocatorExcludedDoc ), 
		RUNTIME_CLASS( CLocatorExcludedFrame ),
		RUNTIME_CLASS( CLocatorExcludedView ) );
	AddDocTemplate( m_pLocatorExclTemplate );

// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame( IDR_MAINFRAME ))
		return FALSE;
	m_pMainWnd = pMainFrame;

// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;	// M
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	/*pMainFrame->m_pFilesToArcDoc =*/ m_pFilesToArcTemplate->OpenDocumentFile(NULL);

	// M   Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();	// (1)

// (3) CG: This line inserted by 'Tip of the Day' component.
	ShowTipAtStartup();

	return TRUE;
}


// App command to run the dialog
//==============================================================================
void CMArc2App::OnAppAbout()
{
	CAboutDlg aboutDlg;
	CString strName = GetProfileString( _T("Settings"), _T("User") );
	if( strName.IsEmpty() )
		aboutDlg.m_strLicensedTo = "Unregistered Copy";
	else
		aboutDlg.m_strLicensedTo = "Licensed to: " + strName;	// (2)
	aboutDlg.DoModal();
}


//==============================================================================
void CMArc2App::OnFileOpen() 
{
	if( AddFiles() )
		UpdateTreeAndList();
}


//==============================================================================
bool CMArc2App::AddFiles()
{
	bool success=true;
	CFileDialog fileDialog( TRUE/*OpenDlg*/, NULL/*NoDefaultExtensionAdded*/,
		_T("*.*")/*InitialFilename*/, OFN_ALLOWMULTISELECT );
// Replace default buffer for filenames with a larger one
	wchar_t buffer[ 8192 ] = _T("*.*");	
	fileDialog.m_ofn.lpstrFile = buffer;
	fileDialog.m_ofn.nMaxFile = 8191;

	int result = fileDialog.DoModal();
	if (result==IDOK)
	{
	// "Open" has been pressed - make a list of selected files
		POSITION ps=fileDialog.GetStartPosition();
		while (ps)	// with each selected file
		{
		// Create an object for the current file or folder
			CString curFilename = fileDialog.GetNextPathName(ps);

		// (2) Check is it directory		
			CFileToArc newFile( curFilename );
			newFile.getInfo();
			if( newFile.getType() == CDiskItem::DI_FOLDER )
				g_TheArchive.m_FoldersToArc.AddFolder( curFilename );
			else
				if( ! g_TheArchive.m_FilesToArc.FileAdd( curFilename ) )
				// (7) condition
					break;	// (7)
		}
	}	// end of if (result==IDOK)
	else 
	// User canceled selection
		success = false;

	return success;
}


//==============================================================================
void CMArc2App::FilesGetStatus()
{
	g_TheArchive.m_FilesToArc.FilesGetStatus();
	UpdateTreeAndList();
}


//==============================================================================
void CMArc2App::UpdateTreeAndList()
{
	if( ((CMainFrame*)AfxGetMainWnd())->m_pFilesToArcFrame != NULL )
	{
		((CMainFrame*)AfxGetMainWnd())->m_pFilesToArcFrame->UpdateList();
		((CMainFrame*)AfxGetMainWnd())->m_pFilesToArcFrame->UpdateTree();
	}
}


// (2)
//==============================================================================
int CMArc2App::ExitInstance() 
{
	int retCode = 0;	// (5)
	try	// (5)
	{
	    ::CoUninitialize();
		retCode = CWinApp::ExitInstance();
	}
	catch(...)
	{
		Beep( 1000, 100 );	// (5)
		retCode = -1;
	}
	return retCode; // (5) Was: CWinApp::ExitInstance();
}


// (3) CG: This function added by 'Tip of the Day' component.
//==============================================================================
void CMArc2App::ShowTipAtStartup(void)
{
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	if (cmdInfo.m_bShowSplash)
	{
		CTipDlg dlg;
		if (dlg.m_bStartup)
			dlg.DoModal();
	}
}

/*
// (3) CG: This function added by 'Tip of the Day' component.
void CMArc2App::ShowTipOfTheDay(void)
{
}
*/

// (3)
//==============================================================================
void CMArc2App::OnUpdateHelpTipOfTheDay(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
}


// (3)
//==============================================================================
void CMArc2App::OnHelpTipOfTheDay() 
{
	CTipDlg dlg;
	dlg.DoModal();
}
