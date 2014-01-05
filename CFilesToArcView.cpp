// DiskArcher.
// CFilesToArcView.cpp - Implementation of the CFilesToArcView class.
// (C) Marat Mirgaleev, 2001-2002.
// Modifications:
//	(1) 09.02.2002. "System" icon added.
//	                GetSelectedFile() added.
//	(2) 18.02.2002. Deletion.
//	(3) 06.03.2002. "Go" and "Pause" buttons added.
//					SetSubItems() added.
//	(4) 28.03.2002. "Pause" button disabled for system files.
//	(5) 07.04.2002. SetSubItems(): Error correction.
//	(6) 10.04.2002. OnFileOpen() removed - it is handled in Frame.
//	(7) 23.05.2002. Corrections in ShowFileList().
//	(8) 01.11.2002. theDB changed to *pTheDB.
//	(9) 16.01.2003. EraseList() moved into the base class.
// (10) 16.11.2003. File Properties button and menu item.
//==============================================================================

#include "stdafx.h"
#include "MArc2.h"

#include "CFilesToArcDoc.h"
#include "CFilesToArcView.h"

#include "CMyArchive.h"   // M
#include "LeftView.h"	  // M
#include "CFileToArc.h"   // M
#include "MainFrm.h"	  // M
#include "CCopiesFrame.h" // M
#include "CCopiesView.h"  // M
#include "CFilesToArcFrame.h"
#include "CArchiveDB.h"
#include "CFileToArcPropertiesDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CFilesToArcView, CListView)

//==============================================================================
BEGIN_MESSAGE_MAP(CFilesToArcView, CListView)
	//{{AFX_MSG_MAP(CFilesToArcView)
	ON_WM_CREATE()
	ON_COMMAND(ID_FILETOARC_SHOWCOPIES, OnFileToArcShowCopies)
	ON_UPDATE_COMMAND_UI(ID_FILETOARC_SHOWCOPIES, OnUpdateShowCopies)
	ON_NOTIFY_REFLECT(LVN_KEYDOWN, OnKeyDown)
	ON_UPDATE_COMMAND_UI(ID_UPDATE_FILES, OnUpdateUpdateFiles)
	ON_COMMAND(ID_UPDATE_FILES, OnUpdateFiles)
	ON_COMMAND(ID_FILETOARC_DELETE, OnFileToArcDelete)
	ON_UPDATE_COMMAND_UI(ID_FILETOARC_DELETE, OnUpdateFileToArcDelete)
	ON_UPDATE_COMMAND_UI(ID_FILETOARC_GO, OnUpdateFileToArcGo)
	ON_COMMAND(ID_FILETOARC_GO, OnFileToArcGo)
	ON_UPDATE_COMMAND_UI(ID_FILETOARC_PAUSE, OnUpdateFileToArcPause)
	ON_COMMAND(ID_FILETOARC_PAUSE, OnFileToArcPause)
	ON_UPDATE_COMMAND_UI(ID_FILETOARC_PROPERTIES, OnUpdateFileToArcProperties)
	ON_COMMAND(ID_FILETOARC_PROPERTIES, OnFileToArcProperties)
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRightClick)
END_MESSAGE_MAP()



//==============================================================================
CFilesToArcView::CFilesToArcView()
{
	m_nIDColumn		  = 0;	// TO DO: Change when columns dragged and dropped
	m_nSizeColumn	  = 1;
	m_nModifiedColumn = 2;
	m_nStatusColumn   = 3;
}


//==============================================================================
BOOL CFilesToArcView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CListView::PreCreateWindow(cs);
}


//==============================================================================
void CFilesToArcView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();


	// TODO: You may populate your ListView with items by directly accessing
	//  its list control through a call to GetListCtrl().
}


// CFilesToArcView diagnostics
//==============================================================================
#ifdef _DEBUG
void CFilesToArcView::AssertValid() const
{
	CListView::AssertValid();
}

void CFilesToArcView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CFilesToArcDoc* CFilesToArcView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFilesToArcDoc)));
	return (CFilesToArcDoc*)m_pDocument;
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CFilesToArcView message handlers


//==============================================================================
void CFilesToArcView::OnRightClick(NMHDR *pNotifyStruct, LRESULT *result)
{
	MyOnRightClick( pNotifyStruct );
	*result = 0;
}


//==============================================================================
int CFilesToArcView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	lpCreateStruct->style |= LVS_REPORT | /*LVS_NOSORTHEADER |*/ LVS_SINGLESEL;
//	lpCreateStruct->dwExStyle |= ; // It doesn't work, but SetExtendedStyle works
	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
// Give the document a pointer to this view
	GetDocument()->m_pListView = this;
	
	CListCtrl& ctlList = (CListCtrl&) GetListCtrl();
	ctlList.SetExtendedStyle( /*TO DO LVS_EX_HEADERDRAGDROP*/ /*| LVS_EX_CHECKBOXES|*/
							  LVS_EX_FULLROWSELECT | 
							  LVS_EX_GRIDLINES | LVS_EX_SUBITEMIMAGES );

// Create the Image List
	BOOL crRes = m_ctlImage.Create( IDB_FTOA_IMAGELIST, 16, 0, RGB(255,0,255));
	if( !crRes )
		AfxMessageBox( "CFilesToArcView::OnCreate(): Can't create Image List." );
//m_ctlImage.SetBkColor(GetSysColor(COLOR_WINDOW));	// TO DO
//	ctlList.SetImageList( &m_ctlImage, LVSIL_NORMAL );
//	ASSERT( ctlList.GetImageList( LVSIL_NORMAL ) == &m_ctlImage );
	ctlList.SetImageList( &m_ctlImage, LVSIL_SMALL );
//	ctlList.SetImageList( &m_ctlImage, LVSIL_STATE );
	
	int res;
	VERIFY( (res=ctlList.InsertColumn( 0, _T("Name"), LVCFMT_LEFT, 210 )) >= 0 );
	res=ctlList.InsertColumn( 1, _T("Size"), LVCFMT_RIGHT, 60 );
	res=ctlList.InsertColumn( 2, _T("Modified"), LVCFMT_LEFT, 120 );
	res=ctlList.InsertColumn( 3, _T("Status"), LVCFMT_LEFT, 100 );

	return 0;
}


// Show files in the List Control.
// Parameters:	- if is not empty, show files of given folder.
//==============================================================================
void CFilesToArcView::ShowFileList( CString showPath )
{
	if( showPath.Right( 1 ) == "\\" )						// (7)
		showPath = showPath.Left( showPath.GetLength()-1 );	// (7)
	CListCtrl& ctlList = GetListCtrl();

	EraseList();

	extern CMyArchive g_TheArchive;
// TO DO: The same with Folders
	for( POSITION pos=g_TheArchive.m_FilesToArc.GetHeadPosition(); pos != NULL; )
	{
		CFileToArc *pCurFile = g_TheArchive.m_FilesToArc.GetNext(pos);
// TO DO: 		if( ! showPath.IsEmpty() )  Is it useless?
// TO DO: Add another one parameter: Show All
		// Otherwise show given directory's files
		{
			CString curFPath = pCurFile->getFullPath();
			if( curFPath.Right( 1 ) == "\\" )						// (7)
				curFPath = curFPath.Left( curFPath.GetLength()-1 );	// (7)
			if( curFPath != showPath )
				continue;	// Don't show this file, go to next file
		}

		int nImageIndex;
		if( pCurFile->m_bSystem )
			nImageIndex = fsSystem;
		else
			nImageIndex = 77;	// Just without icon
int newItem;			// TO DO

		LV_ITEM lvItem;
		lvItem.mask = LVIF_TEXT;
		lvItem.iItem = ctlList.GetItemCount();	// Insert at the end
		lvItem.iSubItem = 0;
#ifdef _DEBUG
		CString tmp = pCurFile->getFullName();
#else
		CString tmp = pCurFile->m_strName;
#endif
		LPCTSTR strItem = tmp;
		lvItem.pszText = (LPTSTR)strItem;
		if(nImageIndex != -1)
		{
			lvItem.mask |= LVIF_IMAGE;
			lvItem.iImage = nImageIndex; //|= LVIF_IMAGE; It was, but what for?
		}

	// Store the pointer to the file
		lvItem.mask |= LVIF_PARAM;
		lvItem.lParam = (LPARAM)pCurFile;

/* Didn't work, but SetItemState works
	// State M
		lvItem.mask |= LVIF_STATE;
		lvItem.stateMask = LVIS_STATEIMAGEMASK;	// Bits 12-15
		lvItem.state = INDEXTOSTATEIMAGEMASK(nState);
*/
		newItem = ctlList.InsertItem(&lvItem);

		BOOL res = ctlList.SetItemState( newItem, INDEXTOSTATEIMAGEMASK( 11/*TO DO nState*/),
			LVIS_STATEIMAGEMASK );//INDEXTOSTATEIMAGEMASK(nState) );

		SetSubItems( newItem, pCurFile ); /* (3) Was:
		if( pCurFile->m_nStatus != fsNotFound && pCurFile->m_nStatus != fsPaused )
		{
			lvItem.iSubItem = m_nSizeColumn;
			lvItem.mask = LVIF_TEXT;
			tmp.Format( "%d", pCurFile->m_nSize );
			strItem = tmp;
			lvItem.pszText = (LPTSTR)strItem;
			ctlList.SetItem( &lvItem );

			lvItem.iSubItem = m_nModifiedColumn;
			lvItem.mask = LVIF_TEXT;
			tmp = pCurFile->m_LastWriteTime.Format();
			strItem = tmp;
			lvItem.pszText = (LPTSTR)strItem;
			ctlList.SetItem( &lvItem );
		}

		lvItem.iSubItem = 3;
		char * const strStatus[] = { "Not Found", "Up-to-date", "New", "Changed", 
									 "Older", "System", "Paused" };
		lvItem.mask = LVIF_IMAGE | LVIF_TEXT;
		lvItem.iImage = pCurFile->m_nStatus;
		lvItem.pszText = strStatus[ pCurFile->m_nStatus ];
		ctlList.SetItem( &lvItem );
*/
	}
}

/* (9)
// Erase all rows from the List Control
//=======================================
void CFilesToArcView::EraseList()
{
	CListCtrl& ctlList = GetListCtrl();
	ctlList.DeleteAllItems();
//	while( ctlList.DeleteColumn(0) );
}
*/

// Process the "Show Copies" command
//==============================================================================
void CFilesToArcView::OnFileToArcShowCopies() 
{
	CFileToArc* pSelFile = GetSelectedFile();
		// Get selected File
	
	CMainFrame *mainFrm = (CMainFrame*)AfxGetMainWnd();
	mainFrm->WindowCopies();
	CCopiesView* pView = (CCopiesView*)(mainFrm->m_pCopiesFrame->GetActiveView());
														// We have just one View
	pView->m_strPath	 = pSelFile->getFullPath();
	pView->m_strFilename = pSelFile->m_strName;
	pView->ShowCopyList();
}


//==============================================================================
void CFilesToArcView::OnUpdateShowCopies(CCmdUI* pCmdUI) 
{
	CListCtrl& ctlList = GetListCtrl();
	POSITION pos = ctlList.GetFirstSelectedItemPosition();
	pCmdUI->Enable( pos != NULL );
}


//==============================================================================
void CFilesToArcView::OnKeyDown(NMHDR* pNMHDR, LRESULT* pResult) 
{
	MyOnKeyDown( pNMHDR );
	*pResult = 0;
}


// Show Popup menu: If any file is selected   - IDR_FILETOARC_POPUP,
//					If any folder is selected - IDR_FOLDERTOARC,
//					If nothing is selected	  - IDR_FILESTOARC_POPUP
//==============================================================================
void CFilesToArcView::ShowPopupMenu( int nItemSel, CPoint curPoint )
{
	if( nItemSel == -1 )
	// Nothing is selected
		DoPopupMenu( IDR_FILESTOARC_POPUP, curPoint );
	else
		DoPopupMenu( IDR_FILETOARC_POPUP, curPoint );
}


/* (6)
void CFilesToArcView::OnFileOpen() 
{
	theApp.AddFiles();	
}
*/


//==============================================================================
void CFilesToArcView::OnUpdateUpdateFiles(CCmdUI* pCmdUI) 
{
 	pCmdUI->Enable();
}


//==============================================================================
void CFilesToArcView::OnUpdateFiles() 
{
	theApp.FilesGetStatus();
}


//==============================================================================
void CFilesToArcView::OnFileToArcDelete() 
{
	CFileToArc* pSelFile = GetSelectedFile();
		// Get selected File
	if( pSelFile->m_bSystem )	
		AfxMessageBox( "This file is for system purposes.\n"
					   "You can not delete it." );
	else
	{
		if( AfxMessageBox(	"Are you sure to remove this File\n"
							"from the Archiving list\n"
							"and to delete all its Copies?", MB_YESNO ) == IDYES )	// (2)
		{
			g_TheArchive.m_FilesToArc.FileDelete( pSelFile );
		// Update display
			UpdateList();	/* (3) Was:
			CLeftView* pView = ((CMainFrame*)AfxGetMainWnd())->m_pFilesToArcFrame->GetLeftPane(); 
			pView->TrackSelChange();
			//ShowFileList();
			*/
			((CMainFrame*)AfxGetMainWnd())->CopiesWindowUpdate();
		}
	}
}


//==============================================================================
void CFilesToArcView::OnUpdateFileToArcDelete(CCmdUI* pCmdUI) 
{
	CListCtrl& ctlList = GetListCtrl();
	POSITION pos = ctlList.GetFirstSelectedItemPosition();
	pCmdUI->Enable( pos != NULL );
}


// (1) Get the file, selected in the List.
//==============================================================================
CFileToArc* CFilesToArcView::GetSelectedFile()
{
	CListCtrl& ctlList = GetListCtrl();
	int nItem;
	POSITION pos = ctlList.GetFirstSelectedItemPosition();
	CFileToArc *pSelFile = NULL;
	if (pos != NULL)
	//	while (pos)	 - TO DO: multiselection
		{
			nItem = ctlList.GetNextSelectedItem(pos);

			LV_ITEM lvItem;
			lvItem.iItem = nItem;
			lvItem.iSubItem = 0;
			lvItem.mask = LVIF_PARAM;
			BOOL bSuccess = ctlList.GetItem( &lvItem );
			if( !bSuccess )
				AfxMessageBox( "CFilesToArcView::GetSelectedFile():\n GetItem() Error!" );
			pSelFile = (CFileToArc*)lvItem.lParam;
		}
	return pSelFile;
}


// (3)
//==============================================================================
void CFilesToArcView::OnUpdateFileToArcGo(CCmdUI* pCmdUI) 
{
	CFileToArc* pSelFile = GetSelectedFile();
	bool bEnable = pSelFile && pSelFile->m_bPaused;
	pCmdUI->Enable( bEnable );
}


// (3)
//==============================================================================
void CFilesToArcView::OnFileToArcGo() 
{
	CFileToArc* pSelFile = GetSelectedFile();
	pSelFile->m_bPaused = false;
	pSelFile->GetStatus();
	g_pTheDB->FileUpdate( pSelFile );
	SetSubItems( GetSelectedItem(), pSelFile );
}


// (3)
//==============================================================================
void CFilesToArcView::OnUpdateFileToArcPause(CCmdUI* pCmdUI) 
{
	CFileToArc* pSelFile = GetSelectedFile();
	bool bEnable = pSelFile && ( ! pSelFile->m_bPaused )
							&& ( ! pSelFile->m_bSystem )/*(4)*/;
	pCmdUI->Enable( bEnable );
}


// (3)
//==============================================================================
void CFilesToArcView::OnFileToArcPause() 
{
	CFileToArc* pSelFile = GetSelectedFile();
	if( ! pSelFile->m_bSystem )
	{
		pSelFile->m_bPaused = true;
		pSelFile->m_nStatus = fsPaused;
		g_pTheDB->FileUpdate( pSelFile );
		SetSubItems( GetSelectedItem(), pSelFile );
	}
}


//==============================================================================
void CFilesToArcView::UpdateList()
{
	CLeftView* pView = ((CMainFrame*)AfxGetMainWnd())->m_pFilesToArcFrame->GetLeftPane(); 
	pView->TrackSelChange();
}


// (3) Moved from ShowFileList()
//==============================================================================
void CFilesToArcView::SetSubItems( int nItem, CFileToArc* pFile )
{
	CListCtrl& ctlList = GetListCtrl();
	LV_ITEM lvItem;
	lvItem.iItem = nItem;

	if( pFile->m_nStatus != fsNotFound && pFile->m_nStatus != fsPaused )
	// Just don't show File's Size and Date if it is not found or paused
	{
		lvItem.iSubItem = m_nSizeColumn;
		lvItem.mask = LVIF_TEXT;
		CString tmp;
		tmp.Format( "%d", pFile->m_nSize );
		LPCTSTR strItem = tmp;
		lvItem.pszText = (LPTSTR)strItem;
		VERIFY( ctlList.SetItem( &lvItem ) );
			// (5) Was:	ASSERT( ctlList.SetItem( &lvItem ) );

		lvItem.iSubItem = m_nModifiedColumn;
		lvItem.mask = LVIF_TEXT;
		tmp = pFile->m_LastWriteTime.Format();
		strItem = tmp;
		lvItem.pszText = (LPTSTR)strItem;
		VERIFY( ctlList.SetItem( &lvItem ) );
			// (5) Was: ASSERT( ctlList.SetItem( &lvItem ) );
	}

	lvItem.iSubItem = m_nStatusColumn;
	char * const strStatus[] = { "Not Found", "Up-to-date", "New", "Changed", 
								 "Older", "System", "Paused" };
	lvItem.mask = LVIF_IMAGE | LVIF_TEXT;
	lvItem.iImage = pFile->m_nStatus;
	lvItem.pszText = strStatus[ pFile->m_nStatus ];
	VERIFY( ctlList.SetItem( &lvItem ) );
		// (5) Was: ASSERT( ctlList.SetItem( &lvItem ) );
}


// (3)
//==============================================================================
int CFilesToArcView::GetSelectedItem()
{
	int nItem = -777;
	CListCtrl& ctlList = GetListCtrl();
	POSITION pos = ctlList.GetFirstSelectedItemPosition();
	if (pos != NULL)
	//	while (pos)	 - TO DO: multiselection
		{
			nItem = ctlList.GetNextSelectedItem(pos);
		}
	return nItem;
}


// (10)
//==============================================================================
void CFilesToArcView::OnUpdateFileToArcProperties(CCmdUI* pCmdUI) 
{
	CListCtrl& ctlList = GetListCtrl();
	POSITION pos = ctlList.GetFirstSelectedItemPosition();
	pCmdUI->Enable( pos != NULL );
}


//==============================================================================
void CFilesToArcView::OnFileToArcProperties() 
{
	CFileToArc* pSelFile = GetSelectedFile();
	CFileToArcPropertiesDialog dlg;	

	dlg.m_bCompressIt = pSelFile->m_bCompressIt;
	dlg.m_sFilename   = pSelFile->getFullName();
	dlg.m_nUpToCopies = pSelFile->m_nUpToCopies;

	int result = dlg.DoModal();
	if (result==IDOK)
		if(    pSelFile->m_bCompressIt != ( dlg.m_bCompressIt != 0 )
			|| dlg.m_nUpToCopies != (UINT)pSelFile->m_nUpToCopies )
				// Compare whether properties have been chanded
		{
			pSelFile->m_bCompressIt = ( dlg.m_bCompressIt != 0 );
			pSelFile->m_nUpToCopies = dlg.m_nUpToCopies;
			g_pTheDB->FileUpdate( pSelFile );
			SetSubItems( GetSelectedItem(), pSelFile );
		}
}
