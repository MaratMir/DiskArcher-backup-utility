// DiskArcher.
// CFilesToArcFrame.cpp : implementation of the CFilesToArcFrame class.
// (C) Marat Mirgaleev, 2001-2002.
// Modifications:
//	(1) 04.02.2002. OnClose() corrections.
//	(2) 05.03.2002. OnDropFiles() - Check is it directory.
//					DoDropFiles() added.
//	(3) 10.04.2002. OnFileOpen() - changed to update screen after adding.
//	(4) 14.05.2002. Status bar correction.
//	(5) 03.07.2002. Status bar correction again.
//	(6) 30.11.2003. Stop files adding on error.
//  (7) 03.09.2006. A bug with dropping folders fixed (I hope).
//========================================================================

#include "stdafx.h"
#include "MArc2.h"

#include "MArcCore/CMyArchive.h"
#include "MArcCore/CFileToArc.h"
#include "MArcCore/Miscelaneous.h"

#include "resource.h"
#include "CFilesToArcFrame.h"
#include "LeftView.h"
#include "CFilesToArcView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CFilesToArcFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CFilesToArcFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CFilesToArcFrame)
	ON_WM_CREATE()
	ON_WM_DROPFILES()
	ON_WM_CLOSE()
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_UPDATE_COMMAND_UI(ID_UPDATE_FILES, OnUpdateUpdateFiles)
	ON_COMMAND(ID_UPDATE_FILES, OnUpdateFiles)
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI_RANGE(AFX_ID_VIEW_MINIMUM, AFX_ID_VIEW_MAXIMUM, OnUpdateViewStyles)
	ON_COMMAND_RANGE(AFX_ID_VIEW_MINIMUM, AFX_ID_VIEW_MAXIMUM, OnViewStyle)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_FILES, OnUpdateFileCount)	// M
END_MESSAGE_MAP()


CFilesToArcFrame::CFilesToArcFrame()
{
//	bShowTree = true;	
}


CFilesToArcFrame::~CFilesToArcFrame()
{
}


BOOL CFilesToArcFrame::OnCreateClient( LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
	// create splitter window
	if (!m_wndSplitter.CreateStatic(this, 1, 2))
		return FALSE;

	if (!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CLeftView), CSize(170, 100), pContext) ||
		!m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CFilesToArcView), CSize(100, 100), pContext))
	{
		m_wndSplitter.DestroyWindow();
		return FALSE;
	}

	return TRUE;
}


BOOL CFilesToArcFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CFilesToArcFrame diagnostics

#ifdef _DEBUG
void CFilesToArcFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CFilesToArcFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG



CFilesToArcView* CFilesToArcFrame::GetRightPane()
{
	CWnd* pWnd = m_wndSplitter.GetPane(0, 1);
	CFilesToArcView* pView = DYNAMIC_DOWNCAST(CFilesToArcView, pWnd);
	return pView;
}


CLeftView* CFilesToArcFrame::GetLeftPane() // M
{
	CWnd* pWnd = m_wndSplitter.GetPane(0, 0);
	CLeftView* pView = DYNAMIC_DOWNCAST(CLeftView, pWnd);
	return pView;
}


/////////////////////////////////////////////////////////////////////////////
// CFilesToArcFrame message handlers

void CFilesToArcFrame::OnUpdateViewStyles(CCmdUI* pCmdUI)
{
	// TODO: customize or extend this code to handle choices on the
	// View menu.

	CFilesToArcView* pView = GetRightPane(); 

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
}


void CFilesToArcFrame::OnViewStyle(UINT nCommandID)
{
	// TODO: customize or extend this code to handle choices on the
	// View menu.
	CFilesToArcView* pView = GetRightPane();

	// if the right-hand pane has been created and is a CFilesToArcView,
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
}


int CFilesToArcFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) // M
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	if( !m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC ) ||
		!m_wndToolBar.LoadToolBar(IDR_FILESTOARC) )
			return -1;
/* M. Use it if you want another one toolbar	
	if (!m_wndToolBar2.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC ) ||
		!m_wndToolBar2.LoadToolBar(IDR_FILESTOARC))
			return -1;
*/
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
//	m_wndToolBar2.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking( CBRS_ALIGN_ANY );
	DockControlBar(&m_wndToolBar);
//	DockControlBar(&m_wndToolBar2);

// M. Enable drag/drop open
	DragAcceptFiles();

  CProgressBar PrBar;
  g_TheArchive.open( &PrBar );

	UpdateTree();
	UpdateList();

	((CMainFrame*)AfxGetMainWnd())->m_pFilesToArcFrame = this;
	
	return 0;
}


void CFilesToArcFrame::OnDropFiles(HDROP hDropInfo) 
{
// M. This code was copied from CFrameWnd::OnDropFiles() from file MFC\Src\WinFrm.Cpp

  SetActiveWindow();      // activate us first !
  UINT nFiles = ::DragQueryFile(hDropInfo, (UINT)-1, NULL, 0);

  CWinApp* pApp = AfxGetApp();
  ASSERT(pApp != NULL);
  for (UINT iFile = 0; iFile < nFiles; iFile++)
  {
    TCHAR szFileName[_MAX_PATH];
    ::DragQueryFile(hDropInfo, iFile, szFileName, _MAX_PATH);

    const MArcLib::error* result = g_TheArchive.addFile( szFileName );
    if( result->getSeverity() > MArcLib::error::severity::everythingIsFine )
    {
      AfxMessageBox( result->getMessage() );
      delete result;
      break;  /* 2014. Was:
    // Check if it is a directory
    CFileToArc newFile( szFileName );
    newFile.getInfo();
    if( newFile.CDiskItem::getType() == CDiskItem::DI_FOLDER )
      g_TheArchive.m_FoldersToArc.AddFolder( szFileName );
    else
      if( ! g_TheArchive.m_FilesToArc.FileAdd( szFileName ) )
        break;*/
    }
  }
  ::DragFinish(hDropInfo);

  UpdateTree();
  UpdateList();
}


void CFilesToArcFrame::EraseList()
{
	CFilesToArcView* pView = GetRightPane();
	pView->EraseList();
}


void CFilesToArcFrame::EraseTree()
{
	CLeftView* pView = GetLeftPane(); 
	pView->EraseTree();
}


void CFilesToArcFrame::UpdateList()
{
	CFilesToArcView* pView = GetRightPane();
	pView->ShowFileList();
}


// There can be many computers in the Tree - in the network!
void CFilesToArcFrame::UpdateTree()	// M
{
	CLeftView* pLeftView = GetLeftPane();
	pLeftView->UpdateTree();
}


CString CFilesToArcFrame::GetTreeSelPath()
{
	CLeftView* pLeftView = GetLeftPane();
	CTreeCtrl& ctlTree = pLeftView->GetTreeCtrl();
	HTREEITEM hItemSel = ctlTree.GetSelectedItem();
	return pLeftView->GetFullPath( hItemSel );
}

/*
void CFilesToArcFrame::OnShowTree() 
{
	bShowTree = !bShowTree;
}
*/
/*
void CFilesToArcFrame::OnUpdateShowTree(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( bShowTree );	
}
*/

void CFilesToArcFrame::OnUpdateFileCount(CCmdUI *pCmdUI)
{
//(5)    CString strFiles((LPCSTR)ID_INDICATOR_FILES);
//(5)	strFiles.TrimRight( '*' );
	CString tmp;
	tmp.Format( _T("Files in Archive: %d")/*(5)Was:"%d"*/, 
				      g_TheArchive.m_FilesToArc.GetCount() );
//(5)	strFiles += tmp;
/* (4) Was:
   CString strFiles;
    strFiles.Format( ID_INDICATOR_FILES, theArchive.m_FilesToArc.GetCount() ); 
*/
    pCmdUI->SetText( tmp/*(5) Was strFiles*/ ); 
    pCmdUI->Enable();
}


void CFilesToArcFrame::OnClose()
{
//	M It was CMDIChildWnd::OnClose();
	MyWindowClose( this );	// (1)
};


void CFilesToArcFrame::OnFileOpen() 
{
	theApp.OnFileOpen();	
		// (3) Was: theApp.AddFiles();
}


void CFilesToArcFrame::OnUpdateUpdateFiles(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
}


void CFilesToArcFrame::OnUpdateFiles() 
{
	theApp.FilesGetStatus();
}


// (2) Just to open access to protected member
void CFilesToArcFrame::DoDropFiles( HDROP hDropInfo )
{
	OnDropFiles( hDropInfo );
}
