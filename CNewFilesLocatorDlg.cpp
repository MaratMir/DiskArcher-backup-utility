// DiskArcher.
// CNewFilesLocatorDlg class - a dialog for set Locator's properties up and
//    to start and stop files search.
// (C) Marat Mirgaleev, 2002.
// Created in Aug 2002.
// Modifications:
//	(1) 26.10.02. From CNewFilesLocator class extracted new CLocatorFolder.
//	(2) 12.01.03. "Files..." and "Folders..." buttons added.
//				  Destructor added.
//	(3) 08.04.03. "Skip useless files..." checkbox disabled during the search.
//  (4) 24.07.04. Reconstruction.
//=============================================================================

#include "stdafx.h"
#include "MArc2.h"
#include "CLocatorExcludedDoc.h"
#include "CLocatorExcludedView.h"
#include "CNewFilesLocator.h"
#include "CNewFilesLocatorDlg.h"
#include "CNewFilesLocatorView.h"
#include "MainFrm.h"
#include "CMyArchive.h"
#include "Miscelaneous.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CNewFilesLocatorDlg::CNewFilesLocatorDlg( CNewFilesLocator* pLocator,
                                          // (4) CNewFilesLocatorView* pView, 
										 CWnd* pParent /*=NULL*/ )
	: CDialog( CNewFilesLocatorDlg::IDD, pParent )
{
	//{{AFX_DATA_INIT(CNewFilesLocatorDlg)
	m_strCurrentFolder = _T("");
	m_strCounter = _T("");
	m_StartPath = _T("C:\\");
	m_nDays = 15;
	m_bSkipSomeFiles = FALSE;
	//}}AFX_DATA_INIT

	m_bRunning   = false;
	m_bIsAborted = false;
    // (4)	m_pView    = pView;
	m_nCounter = 0;
	m_pLocator = pLocator;
}


CNewFilesLocatorDlg::~CNewFilesLocatorDlg()	// (2)
{
	if( m_pLocator )
        if( m_pLocator->m_pView )  // (4)
        {
        // delete m_pLocator; - No, Locator will be deleted on the View close
        // m_pLocator = NULL;     // (4)
        }
}


void CNewFilesLocatorDlg::Finalize()	// (2)
{
	m_pLocator/*(4)Was: m_pView*/->m_pDlg = NULL;
	delete this;
}


void CNewFilesLocatorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewFilesLocatorDlg)
	DDX_Control(pDX, IDC_EXCL_FILE_TYPES_BUTTON, m_ExclFileTypesButton);
	DDX_Control(pDX, IDC_EXCL_FOLDERS_BUTTON, m_ExclFoldersButton);
	DDX_Control(pDX, IDC_LCTR_BROWSE, m_BrowseBtn);
	DDX_Control(pDX, IDC_CHECKED_FILES, m_CheckedFiles);
	DDX_Control(pDX, IDABORT, m_CancelBtn);
	DDX_Control(pDX, IDC_LOCATOR_DAYS_SPIN, m_Spin);
	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Text(pDX, IDC_LOCATOR_CURRENT_FOLDE, m_strCurrentFolder);
	DDX_Text(pDX, IDC_COUNTER, m_strCounter);
	DDX_Text(pDX, IDC_LOCATOR_START_PATH, m_StartPath);
	DDX_Text(pDX, IDC_LOCATOR_DAYS, m_nDays);
	DDV_MinMaxUInt(pDX, m_nDays, 1, 500);
	DDX_Check(pDX, IDC_SKIP_SOME_FILES, m_bSkipSomeFiles);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewFilesLocatorDlg, CDialog)
	//{{AFX_MSG_MAP(CNewFilesLocatorDlg)
	ON_BN_CLICKED(IDABORT, OnAbort)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_LCTR_BROWSE, OnLctrBrowse)
	ON_BN_CLICKED(IDC_SKIP_SOME_FILES, OnSkipSomeFilesClick)
	ON_BN_CLICKED(IDC_EXCL_FILE_TYPES_BUTTON, OnExclFileTypesButton)
	ON_BN_CLICKED(IDC_EXCL_FOLDERS_BUTTON, OnExclFoldersButton)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CNewFilesLocatorDlg::EnableControls( bool bOnOff )
{
	if( ! m_bIsAborted )
	{
		m_BrowseBtn.EnableWindow( bOnOff );
		m_ExclFileTypesButton.EnableWindow( bOnOff );	// (2)
		m_ExclFoldersButton.EnableWindow( bOnOff );		// (2)
		CWnd* pSkipSomeFilesCheckBox = GetDlgItem( IDC_SKIP_SOME_FILES );// (3)
		pSkipSomeFilesCheckBox->EnableWindow( bOnOff);					 // (3)
	}
}


// Checks is "Cancel" pressed. Also lets MFC do its idle processing.
// Returns true if "Cancel" pressed, otherwise returns false.
// This code got from MSDN Q&A: C++ Periodicals 1998.
bool CNewFilesLocatorDlg::IsAborted()
{
	MSG msg;
	while ( ! m_bIsAborted &&
			::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE)) 
	{	
		if( ! AfxGetThread()->PumpMessage() )
			m_bIsAborted = true;
	}
	return m_bIsAborted;
}


void CNewFilesLocatorDlg::OnOK() 
{
	m_bRunning = true;	// (2)
	m_OkButton.EnableWindow( FALSE );
	m_CheckedFiles.ShowWindow( SW_NORMAL );
	m_CancelBtn.SetFocus();
	UpdateData();	

	EnableControls( false );

// (1)
// (4) Was:  m_pLocator = new CNewFilesLocator( m_StartPath, m_pView, this );
// (2)	if( m_pLocator->Init() )
	m_pLocator->Analyze( m_StartPath );	// (1)
/* (1) Was too complicated:
	if( ! m_pLocator->Init() )
	{
		delete m_pLocator;
		m_pLocator = NULL;
	}
	else
		m_pLocator->Analyze();
*/

//	EnableControls( true ); It's useless because the dialog
//							will be closed any way
	((CMainFrame*)AfxGetMainWnd())->EnableControls( true );
	if( ( ! IsAborted() ) 	// Not interrupted by user
	 && ( ! g_TheArchive.m_bStopWorking ))
		CDialog::OnOK();
			// If Cancel was pressed, OnCancel has been invoked already
	m_bRunning = false;	// (2)
	Finalize();
}


void CNewFilesLocatorDlg::OnAbort() 
{
	((CMainFrame*)AfxGetMainWnd())->EnableControls( true );
	m_bIsAborted = true;	// Set "cancel operation" flag

	if( !m_bRunning )
		Finalize();
}


void CNewFilesLocatorDlg::OnClose() 
{
	OnAbort();
}


// It is how it described in MSDN CDialog for modeless dialogs
void CNewFilesLocatorDlg::OnCancel()
{
	OnAbort();
}


void CNewFilesLocatorDlg::CountFile()
{
	m_nCounter++;
	m_strCounter.Format( "%d", m_nCounter );
}


void CNewFilesLocatorDlg::ShowProgress( CString strFName )
									// (1) Was: CDiskItem *pItem )
{
	m_strCurrentFolder = strFName; // (1) Was: Item->GetFullPath();
	CountFile();
	UpdateData( FALSE );
	UpdateWindow();
}


void CNewFilesLocatorDlg::OnLctrBrowse() 
{
	UpdateData();
	CString strFolder;
	if( BrowseForFolder( "Select a folder to start from", strFolder ))
	{
		m_StartPath = strFolder;
		UpdateData( FALSE );
		SetFocus();	// To select the dialog window again
	}
}


BOOL CNewFilesLocatorDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
// Add extra initialization here
	m_Spin.SetRange( 1, 100 );
	m_Spin.SetPos( 5 );

	m_bSkipSomeFiles = true;	// (2)
	UpdateData( FALSE );		// (2)	
	OnSkipSomeFilesClick();		// (2)

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CNewFilesLocatorDlg::OnSkipSomeFilesClick()	// (2)
{
	UpdateData();
	m_ExclFileTypesButton.EnableWindow( m_bSkipSomeFiles );
	m_ExclFoldersButton.EnableWindow( m_bSkipSomeFiles );
}


void CNewFilesLocatorDlg::OnExclFileTypesButton() 
{
	ShowExcluded( LocExclFileTypes );
}


void CNewFilesLocatorDlg::OnExclFoldersButton() 
{
	ShowExcluded( LocExclFolders );
}


// (2)
int CNewFilesLocatorDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
// Add your specialized creation code here
// (4)	/*bSuccess = */ CNewFilesLocator::Init();
	
	return 0;
}


void CNewFilesLocatorDlg::ShowExcluded( enum LocatorWhatToExclude nShowType )
{
	ShowWindow( SW_HIDE );
	CLocatorExcludedDoc *pDoc 
		= (CLocatorExcludedDoc*)theApp.m_pLocatorExclTemplate->
													OpenDocumentFile(NULL);
	if( pDoc )
	{
		pDoc->m_pParentWindow = this;
		pDoc->m_pLocator = m_pLocator;
		POSITION pos = pDoc->GetFirstViewPosition();
        CLocatorExcludedView* pView 
				= (CLocatorExcludedView*)(pDoc->GetNextView(pos));
						// We have only one view
		pView->ShowList( nShowType );
	}
}
