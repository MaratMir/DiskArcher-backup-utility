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
#include "MArcCore/CMyArchive.h"
#include "MArcCore/enums.h"
#include "MainFrm.h"
#include "CProgressDialog.h"
#include "CRoomsFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


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
//ON_BN_CLICKED(IDOK, &CProgressDialog::OnBnClickedOk)
		// 2014-01-05: Removing. It came somehow during migration to VS 2012
END_MESSAGE_MAP()



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
bool CProgressDialog::isAborted()
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
  g_TheArchive.setProgressDlg( NULL ); // TODO: Ugly hack using a global variable!
	return bResult;
}


void CProgressDialog::resetAndShow()
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
void CProgressDialog::setMaxRange(int nMax)
{
	m_Progress.SetRange32( 0, nMax ); 
}


// (3)
void CProgressDialog::advance( int nAmount )
{
	m_Progress.OffsetPos( nAmount );
	UpdateWindow();
}


// (3)
void CProgressDialog::setMessage( CString strMessage )
{
	m_CurrentFilename = strMessage;
	UpdateData( FALSE );
	UpdateWindow();
}


// (3)
void CProgressDialog::setPos(int newPos)
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

void CProgressDialog::analysisDone()
{
  m_CopyingLabel.EnableWindow();
  m_AnalysingBmp.ShowWindow(SW_NORMAL);
}


void CProgressDialog::finished( bool i_stopWorking, OpResult i_result )
{
  if( i_result <= OPR_NONFATAL_ERRORS )
  {
    m_CopyingBmp.ShowWindow(SW_NORMAL);
    m_StopButton.EnableWindow( FALSE );
    m_OkButton.EnableWindow();
    m_OkButton.SetFocus();
    if( i_stopWorking || m_bIsAborted )
      m_SuccessLabel.SetWindowText( L"User break" );
    if( i_result == OPR_WARNINGS )
      m_SuccessLabel.SetWindowText( L"There were some warnings" );
    if( i_result == OPR_NONFATAL_ERRORS )
      m_SuccessLabel.SetWindowText( L"There were errors. Check the Log" );
    m_SuccessLabel.ShowWindow(SW_NORMAL);
  }

  CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();

  if( i_result >= OPR_FATAL_STOP )
    if( this ) // ???
      DestroyWindow();
    else // TODO: Never reached, because if is always true
      pMainFrame->EnableControls( true );
        // Otherwise it wiil be called in m_pProgressDlg->DestroyWindow()

  // RoomsUpdate(); // 29.12.2001   Free space is updated in CRoom::DoCopying()
  if( pMainFrame->m_pRoomsFrame != NULL )
    pMainFrame->m_pRoomsFrame->UpdateList();
  pMainFrame->CopiesWindowUpdate();

  // 'this' (Progress Dlg) can't be deleted here because it is opened at the moment and it waits for user reaction
}
