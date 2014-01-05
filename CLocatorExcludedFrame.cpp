// DiskArcher.
// Implementation of the CLocatorExcludedFrame class.
// (C) Marat Mirgaleev, 2003.
// Created 12.01.2003.
// Modifications:
//==================================================================

#include "stdafx.h"
#include "resource.h"
#include "CNewFilesLocator.h"
#include "CLocatorExcludedDoc.h"
#include "CLocatorExcludedFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CLocatorExcludedFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CLocatorExcludedFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CLocatorExcludedFrame)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CLocatorExcludedFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}



// CLocatorExcludedFrame diagnostics
//==================================================================

#ifdef _DEBUG
void CLocatorExcludedFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CLocatorExcludedFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG



// CLocatorExcludedFrame message handlers
//==================================================================


int CLocatorExcludedFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) // M
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if( !m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC ) ||
		!m_wndToolBar.LoadToolBar( IDR_LOCATOR_EXCL ) )
			return -1;
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking( CBRS_ALIGN_ANY );
	// DockControlBar(&m_wndToolBar); Похоже, без этого лучше.

	return 0;
}

