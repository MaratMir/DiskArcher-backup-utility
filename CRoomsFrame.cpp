// DiskArcher.
// CRoomsFrame.cpp : implementation of the CRoomsFrame class
// (C) Marat Mirgaleev, 2001-2002.
// Modifications:
//		(1) 04.02.2002. OnClose() correction.
//==================================================================

#include "stdafx.h"
#include "MArc2.h"

#include "CRoomsFrame.h"

#include "CRoomsView.h"
#include "MainFrm.h"		// M
#include "CFileOnDisk.h"	// M
#include "Miscelaneous.h"	// M
#include "CMyArchive.h"		// M

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CRoomsFrame

IMPLEMENT_DYNCREATE(CRoomsFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CRoomsFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CRoomsFrame)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI_RANGE(AFX_ID_VIEW_MINIMUM, AFX_ID_VIEW_MAXIMUM, OnUpdateViewStyles)
	ON_COMMAND_RANGE(AFX_ID_VIEW_MINIMUM, AFX_ID_VIEW_MAXIMUM, OnViewStyle)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRoomsFrame construction/destruction

CRoomsFrame::CRoomsFrame()
{
}


CRoomsFrame::~CRoomsFrame()
{
}


BOOL CRoomsFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CRoomsFrame diagnostics

#ifdef _DEBUG
void CRoomsFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CRoomsFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CRoomsFrame message handlers

/*
CRoomsView* CRoomsFrame::GetRightPane()
{
	CWnd* pWnd = m_wndSplitter.GetPane(0, 1);
	CRoomsView* pView = DYNAMIC_DOWNCAST(CRoomsView, pWnd);
	return pView;
}
*/

void CRoomsFrame::OnUpdateViewStyles(CCmdUI* pCmdUI)
{
/*
	// TODO: customize or extend this code to handle choices on the
	// View menu.

	CRoomsView* pView = GetRightPane(); 

	// if the right-hand pane hasn't been created or isn't a view,
	// disable commands in our range

	if (pView == NULL)
		pCmdUI->Enable(FALSE);
	else
	{
		DWORD dwStyle = pView->GetStyle() & LVS_TYPEMASK;

		// if the command is ID_VIEW_LINEUP, only enable command
		// when we're in LVS_ICON or LVS_SMALLICON mode

		if (pCmdUI->m_nID == ID_VIEW_LINEUP)
		{
			if (dwStyle == LVS_ICON || dwStyle == LVS_SMALLICON)
				pCmdUI->Enable();
			else
				pCmdUI->Enable(FALSE);
		}
		else
		{
			// otherwise, use dots to reflect the style of the view
			pCmdUI->Enable();
			BOOL bChecked = FALSE;

			switch (pCmdUI->m_nID)
			{
			case ID_VIEW_DETAILS:
				bChecked = (dwStyle == LVS_REPORT);
				break;

			case ID_VIEW_SMALLICON:
				bChecked = (dwStyle == LVS_SMALLICON);
				break;

			case ID_VIEW_LARGEICON:
				bChecked = (dwStyle == LVS_ICON);
				break;

			case ID_VIEW_LIST:
				bChecked = (dwStyle == LVS_LIST);
				break;

			default:
				bChecked = FALSE;
				break;
			}

			pCmdUI->SetRadio(bChecked ? 1 : 0);
		}
	}
*/
}


void CRoomsFrame::OnViewStyle(UINT nCommandID)
{
/*
	// TODO: customize or extend this code to handle choices on the
	// View menu.
	CRoomsView* pView = GetRightPane();

	// if the right-hand pane has been created and is a CRoomsView,
	// process the menu commands...
	if (pView != NULL)
	{
		DWORD dwStyle = -1; // Это, оказывается, 0xffffffff

		switch (nCommandID)
		{
		case ID_VIEW_LINEUP:
			{
				// ask the list control to snap to grid
				CListCtrl& refListCtrl = pView->GetListCtrl();
				refListCtrl.Arrange(LVA_SNAPTOGRID);
			}
			break;

		// other commands change the style on the list control
		case ID_VIEW_DETAILS:
			dwStyle = LVS_REPORT;
			break;

		case ID_VIEW_SMALLICON:
			dwStyle = LVS_SMALLICON;
			break;

		case ID_VIEW_LARGEICON:
			dwStyle = LVS_ICON;
			break;

		case ID_VIEW_LIST:
			dwStyle = LVS_LIST;
			break;
		}

		// change the style; window will repaint automatically
		if (dwStyle != -1)
			pView->ModifyStyle(LVS_TYPEMASK, dwStyle);
	}
*/
}


int CRoomsFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) // M
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
// TODO: Add your specialized creation code here
	if( !m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC ) ||
		!m_wndToolBar.LoadToolBar(IDR_ROOMS) )
			return -1;
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking( CBRS_ALIGN_ANY );
	DockControlBar(&m_wndToolBar);

	UpdateList();

	((CMainFrame*)AfxGetMainWnd())->m_pRoomsFrame = this;
	
	return 0;
}


void CRoomsFrame::EraseList()
{
//	CRoomsView* pView = GetRightPane();
//	pView->EraseList();
}


void CRoomsFrame::UpdateList()
{
	CRoomsView* pView = (CRoomsView*)GetActiveView();	// We have just one View
	if( pView != NULL )
		pView->ShowRoomList();
}


void CRoomsFrame::OnClose() 
{
//	M It was CMDIChildWnd::OnClose();
	MyWindowClose( this );	// (1)
}
