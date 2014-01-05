// DiskArcher.
// CLogFrame.cpp : implementation of the CLogFrame class
// (C) Marat Mirgaleev, 2001-2002.
// Modifications:
//		(1) 04.02.2002. OnClose() correction.
//==================================================================

#include "stdafx.h"

#include "CLogFrame.h"
#include "MainFrm.h"		// M
#include "CLogView.h"		// M
#include "Miscelaneous.h"	// M

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CLogFrame

IMPLEMENT_DYNCREATE(CLogFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CLogFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CLogFrame)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogFrame construction/destruction

CLogFrame::CLogFrame()
{
}


CLogFrame::~CLogFrame()
{
}


BOOL CLogFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CLogFrame diagnostics

#ifdef _DEBUG
void CLogFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CLogFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CLogFrame message handlers


int CLogFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) // M
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
/*
	if( !m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC ) ||
		!m_wndToolBar.LoadToolBar(IDR_COPIES) )
			return -1;
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking( CBRS_ALIGN_ANY );
	DockControlBar(&m_wndToolBar);
*/
	UpdateList();

	((CMainFrame*)AfxGetMainWnd())->m_pLogFrame = this;
	
	return 0;
}

/*
void CLogFrame::EraseList()
{
//	CCopiesView* pView = GetRightPane();
//	pView->EraseList();
}
*/

void CLogFrame::UpdateList()
{
	CLogView* pView = (CLogView*)GetActiveView();	// We have just one View
	if( pView != NULL )
		pView->ShowLog();
}


void CLogFrame::OnClose() 
{
//	M It was CMDIChildWnd::OnClose();
	MyWindowClose( this );	// (1)
}
