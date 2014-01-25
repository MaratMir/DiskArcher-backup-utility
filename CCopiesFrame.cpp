// DiskArcher.
// CCopiesFrame.cpp : implementation of the CCopiesFrame class
// (C) Marat Mirgaleev, 2001-2002.
// Modifications:
//		(1) 04.02.2002. OnClose() corrections.
//==================================================================

#include "stdafx.h"
#include "MArc2.h"

#include "CCopiesFrame.h"
#include "resource.h"
#include "CCopiesView.h"
#include "MainFrm.h"
#include "CFileOnDisk.h"
#include "MArcCore/Miscelaneous.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CCopiesFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CCopiesFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CCopiesFrame)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_COMMAND(ID_COPY_DELETE, OnCopyDelete)
	ON_UPDATE_COMMAND_UI(ID_COPY_DELETE, OnUpdateCopyDelete)
	ON_COMMAND(ID_COPY_PROPERTIES, OnCopyProperties)
	ON_UPDATE_COMMAND_UI(ID_COPY_PROPERTIES, OnUpdateCopyProperties)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CCopiesFrame::CCopiesFrame()
{
}


CCopiesFrame::~CCopiesFrame()
{
}


BOOL CCopiesFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}


#ifdef _DEBUG
void CCopiesFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CCopiesFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG


int CCopiesFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) // M
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
// TODO: Add your specialized creation code here
	if( !m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC ) ||
		!m_wndToolBar.LoadToolBar(IDR_COPIES) )
			return -1;
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking( CBRS_ALIGN_ANY );
	DockControlBar(&m_wndToolBar);

	UpdateList();

	((CMainFrame*)AfxGetMainWnd())->m_pCopiesFrame = this;
	
	return 0;
}

/*
void CCopiesFrame::EraseList()
{
//	CCopiesView* pView = GetRightPane();
//	pView->EraseList();
}
*/

void CCopiesFrame::UpdateList()
{
	CCopiesView* pView = (CCopiesView*)GetActiveView();	// We have just one View
	if( pView != NULL )
		pView->ShowCopyList();
}


void CCopiesFrame::OnClose() 
{
//	M It was CMDIChildWnd::OnClose();
	MyWindowClose( this );	// (1)
}


void CCopiesFrame::OnCopyDelete() 
{
	// TODO: Add your command handler code here
}


void CCopiesFrame::OnUpdateCopyDelete(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
}


void CCopiesFrame::OnCopyProperties() 
{
	// TODO: Add your command handler code here
}


void CCopiesFrame::OnUpdateCopyProperties(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
}
