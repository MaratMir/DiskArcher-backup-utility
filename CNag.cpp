// DiskArcher.
// Nag screen.
// (C) Marat Mirgaleev, 2001-2002.
// Created 06.05.2002.
// Modifications:
//	(1) 16.06.02. Web page changed.
//	(2) 05.07.02. Days counter added.
//	(3) 06.04.04. Days counting procedure moved out from here.
//==================================================================

#include "stdafx.h"
#include <math.h>
#include "marc2.h"
#include "CNag.h"
#include "CRegisterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CNag::CNag(CWnd* pParent /*=NULL*/)
	: CDialog(CNag::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNag)
	m_RegText = _T("");
	//}}AFX_DATA_INIT
}


void CNag::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNag)
	DDX_Control(pDX, IDC_DAYS_COUNTER, m_DaysCounter);
	DDX_Text(pDX, IDC_REGISTER_TEXT, m_RegText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNag, CDialog)
	//{{AFX_MSG_MAP(CNag)
	ON_WM_CREATE()
	ON_BN_CLICKED(ID_REGISTER, OnRegister)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CNag message handlers

int CNag::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_RegText = "Thank you for using DiskArcher."
		"\n\nThis is a Shareware program. You can evaluate "
		"DiskArcher during 30 days. If you want to use DiskArcher "
		"after this 30-days trial period, you MUST REGISTER "
		"it. Otherwise you must remove it from your computer."
		"\n\nTo get a registration code go "
		"http://diskarcher.mastak.com"	// (1) Was eSmartWeb
		"\n(if this page doesn't "
		"work, try to find \"DiskArcher\" on one of Internet "
		"search sites)."
		"\n\nWhen you receive the registration code, "
		"press \"Register\" button.\n";

	return 0;
}


void CNag::OnRegister() 
{
	KillTimer( m_nTimer );	// (2)
	EndDialog(0);

	CRegisterDlg dlg;
	dlg.DoModal();
}


// (2) Days Counter. Calculate the Time Span and show it.
BOOL CNag::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CString tmp;
	int nDays = theApp.GetPassedDays(); /* (3) Was:	
// It would be nice to get the MDB create date. But it is impossible in FAT
	CTime theTime = CTime::GetCurrentTime();
	CTime baseTime( 2002, 01, 01, 0, 0, 0 );	// Just a basis for calculations
	CTimeSpan tsFromBase = theTime - baseTime;
	int nElapsedDaysFromBase = tsFromBase.GetDays();
	
	CString tmp;
	int nFirstDay = theApp.GetProfileInt( "Settings", "FirstDay", -1 );
	if( nFirstDay == -1 )
		VERIFY( theApp.WriteProfileInt( "Settings", "FirstDay", 
										nElapsedDaysFromBase ) );
	else
		tmp.Format( "You have used\nDiskArcher\nfor %d days", 
					nElapsedDaysFromBase - nFirstDay );*/
	tmp.Format( "You have used\nDiskArcher\nfor %d days", nDays ); // (3)
	m_DaysCounter.SetWindowText( tmp );

// Get a font and change Counter's font to bold
	CFont *curFont = GetFont();		// Get from the Dialog
	LOGFONT lf;
	curFont->GetLogFont( &lf );
	lf.lfWeight = FW_BOLD;
	lf.lfEscapement = -30;	// It's funny!
	strcpy(lf.lfFaceName, "Arial" );
	VERIFY( m_CounterFont.CreateFontIndirect(&lf) );
	m_DaysCounter.SetFont( &m_CounterFont );	// Set for Static only

// Remember position of Counter	
	RECT wndRect;
	m_DaysCounter.GetWindowRect( &wndRect );
	ScreenToClient( &wndRect );
	m_nCounterTop = wndRect.top;

	m_nTimer = SetTimer(1, 30, 0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


// (2) Move the Counter - just for fun
void CNag::OnTimer(UINT nIDEvent) 
{
	static double timeCounter = 0.;
	RECT wndRect;
	m_DaysCounter.GetWindowRect( &wndRect );
	ScreenToClient( &wndRect );
	double movement = 50. * cos( timeCounter ) - 50;
	wndRect.top    = m_nCounterTop + (LONG)movement;
	wndRect.bottom = m_nCounterTop + 80 + (LONG)movement;
	timeCounter += 0.02;
	m_DaysCounter.MoveWindow( &wndRect );

	CDialog::OnTimer(nIDEvent);
}


// (2)
void CNag::OnDestroy() 
{
	CDialog::OnDestroy();
	
	KillTimer( m_nTimer );	// It might have been killed before,
							//	but killing it again doesn't raise an error
}
