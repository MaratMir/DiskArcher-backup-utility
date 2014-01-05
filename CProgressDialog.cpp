// DiskArcher.
// CProgressDialog class implementation file.
// (C) Marat Mirgaleev, 2001-2002.
// Modifications:
//	(1) 15.01.2002. Log added.
//	(2) 15.01.2002. OnClose() behaviour corrected.
//	(3) 13.02.2002. Some functions added.
//	(4) 03.07.2002. IsAborted() changed.
//	(5) 01.11.2002. Fixed: EnableControls() lost when Esc pressed.
//	(6) 18.03.2003. (5) doesn't always work, fixing it totally.
//==================================================================

#include "stdafx.h"
#include "MArc2.h"
#include "CMyArchive.h"
#include "CProgressDialog.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProgressDialog dialog

CProgressDialog::CProgressDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CProgressDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProgressDialog)
	m_CurrentFilename = _T("");
	//}}AFX_DATA_INIT

	m_bIsAborted = false;
}


void CProgressDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProgressDialog)
	DDX_Control(pDX, IDC_COPYING_SUCCESS, m_SuccessLabel);
	DDX_Control(pDX, IDABORT, m_StopButton);
	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDC_ANALYSING_BMP, m_AnalysingBmp);
	DDX_Control(pDX, IDC_COPYING_BMP, m_CopyingBmp);
	DDX_Control(pDX, IDC_COPYING_LABEL, m_CopyingLabel);
	DDX_Control(pDX, IDC_PROGRESS, m_Progress);
	DDX_Text(pDX, IDC_CURRENT_FILENAME, m_CurrentFilename);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProgressDialog, CDialog)
	//{{AFX_MSG_MAP(CProgressDialog)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDABORT, OnAbort)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CProgressDialog message handlers


// "Ok" means "View Log" button!
// This button is unavailable during archiving process, so this event handler
//	 can be invoked only when archiving is finished.
void CProgressDialog::OnOK() 
{
//(6)((CMainFrame*)AfxGetMainWnd())->EnableControls( true );	// M
	CDialog::OnOK();
	((CMainFrame*)AfxGetMainWnd())->WindowLog();	// (1)
	DestroyWindow(); // (6)
}


// Invokes when "Stop" button is pressed.
// If archiving is in progress, just send a "Stop" message.
// If archiving has been completed, then close the dialog.
void CProgressDialog::OnAbort() 
{
// TODO: Ask "Interrupt?"
// if( MessageBox... )
// (6)	((CMainFrame*)AfxGetMainWnd())->EnableControls( true );	// M
	if( g_TheArchive.m_bIsWorking )
		m_bIsAborted = true;	// Set "cancel operation" flag
	else
		DestroyWindow();
}


// Invoked when user presses "X" in right left window's angle
void CProgressDialog::OnClose() 
{
	OnAbort();	// (2)
}


// Invoked when user presses Esc button.
void CProgressDialog::OnCancel()
{
	OnAbort();
}


// Checks is "Cancel" pressed. Also lets MFC do its idle processing.
// Returns true if "Cancel" pressed, otherwise returns false.
// This code got from MSDN Q&A: C++ Periodicals 1998.
bool CProgressDialog::IsAborted()
{
	MSG msg;
	while ( ! m_bIsAborted &&
			::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE)) 
	{	
		if( ! AfxGetThread()->PumpMessage() )
			m_bIsAborted = true;
	}
	/* (4) Was:	
	while (::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE)) 
	{	AfxGetThread()->PumpMessage();
	}*/
	return m_bIsAborted;
}


BOOL CProgressDialog::DestroyWindow() 
{
	BOOL bResult;
	// TODO: Add your specialized code here and/or call the base class
	((CMainFrame*)AfxGetMainWnd())->EnableControls( true );
	bResult = CDialog::DestroyWindow();
	delete this;
	g_TheArchive.m_pProgressDlg = NULL;
	return bResult;
}


void CProgressDialog::ResetAndShow()
{
	m_CopyingLabel.EnableWindow( FALSE );
	m_CopyingBmp.ShowWindow(SW_HIDE);
	m_AnalysingBmp.ShowWindow(SW_HIDE);
	m_StopButton.EnableWindow();
	m_OkButton.EnableWindow(FALSE);
	m_SuccessLabel.ShowWindow(SW_HIDE);
	m_Progress.SetPos( 0 ); 
	ShowWindow( SW_NORMAL );
	CenterWindow();
	UpdateWindow();

	m_bIsAborted = false;
	m_Progress.SetStep( 1 );
}


// (3)
void CProgressDialog::SetMaxRange(int nMax)
{
	m_Progress.SetRange32( 0, nMax ); 
}


// (3)
void CProgressDialog::Advance( int nAmount )
{
	m_Progress.OffsetPos( nAmount );
	UpdateWindow();
}


// (3)
void CProgressDialog::SetMessage( CString strMessage )
{
	m_CurrentFilename = strMessage;
	UpdateData( FALSE );
	UpdateWindow();
}


// (3)
void CProgressDialog::SetPos(int newPos)
{
	m_Progress.SetPos( newPos ); 
}

/* 
// (5) It doesn't do what we need because the focus is lost all the time
//	   when user clicks anywhere outside the dialog.
void CProgressDialog::OnKillFocus(CWnd* pNewWnd) 
{
	CDialog::OnKillFocus(pNewWnd);
	
	((CMainFrame*)AfxGetMainWnd())->EnableControls( true );
}

*/
