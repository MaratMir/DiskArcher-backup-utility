// DiskArcher.
// LeftView.cpp : implementation of the CLeftView class
// (C) Marat Mirgaleev, 2001-2002.
// Modifications:
//	(1) 27.11.2001.
//	(2) 25.05.2002. AddWithSlash() using.
//=================================================================

// LATER: Rename It into xxxFilesToArcXxx

#include "stdafx.h"
#include "MArc2.h"
#include "CMyArchive.h" // (1)
#include "CFileToArc.h" // (1)
#include "CDiskItem.h"	// (2)

#include "CFilesToArcDoc.h"
#include "LeftView.h"

#include "CFilesToArcView.h" // M

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLeftView

IMPLEMENT_DYNCREATE(CLeftView, CTreeView)

BEGIN_MESSAGE_MAP(CLeftView, CTreeView)
	//{{AFX_MSG_MAP(CLeftView)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelChanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLeftView construction/destruction

CLeftView::CLeftView()
{
	// TODO: add construction code here

}

CLeftView::~CLeftView()
{
}

BOOL CLeftView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CTreeView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CLeftView drawing

void CLeftView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();

	// TODO: You may populate your TreeView with items by directly accessing
	//  its tree control through a call to GetTreeCtrl().
}

/////////////////////////////////////////////////////////////////////////////
// CLeftView diagnostics

#ifdef _DEBUG
void CLeftView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CLeftView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}

CFilesToArcDoc* CLeftView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFilesToArcDoc)));
	return (CFilesToArcDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLeftView message handlers

int CLeftView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	lpCreateStruct->style |= TVS_HASLINES | TVS_HASBUTTONS
						   /*| TVS_CHECKBOXES*/ | TVS_LINESATROOT;
	if (CTreeView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Create the Image List
	if( !m_ImageList.Create( IDB_TREEIMAGELIST, 16/*TO DO*/, 0, 
						RGB(255,0,255))  )
		AfxMessageBox( _T( "CLeftView::OnCreate(): Image Create Error.") );
	m_ImageList.SetBkColor(GetSysColor(COLOR_WINDOW));

	/// Attach image list to Tree
	CTreeCtrl& ctlTree = (CTreeCtrl&) GetTreeCtrl();
	ctlTree.SetImageList( &m_ImageList, TVSIL_NORMAL );
//	ctlTree.SetImageList( &m_ImageList, TVSIL_STATE );

	return 0;	// TO DO
}


// M Find a computer in the Tree
HTREEITEM CLeftView::GetCompItem( CString compName )
{
	CTreeCtrl& ctlTree = (CTreeCtrl&) GetTreeCtrl();
	HTREEITEM hCompItem = ctlTree.GetRootItem();
	while( true )
	{
		if( hCompItem == NULL )
			break;
		CString curText = ctlTree.GetItemText( hCompItem );
		if( curText == compName )
			break;
		else
			hCompItem = ctlTree.GetNextSiblingItem( hCompItem );
	}
	return hCompItem;
}


HTREEITEM CLeftView::GetChildItem( HTREEITEM hParentItem, CString itemName ) // M
{
	CTreeCtrl& ctlTree = (CTreeCtrl&) GetTreeCtrl();
	HTREEITEM hChildItem = ctlTree.GetChildItem( hParentItem );
	while( true )
	{
		if( hChildItem == NULL )
			break;
		CString curText = ctlTree.GetItemText( hChildItem );
		if( curText == itemName )
			break;
		else
			hChildItem = ctlTree.GetNextSiblingItem( hChildItem );
	}
	return hChildItem;
}


// M Find a Drive in the Tree
HTREEITEM CLeftView::GetDriveItem( HTREEITEM hCompItem, CString driveName )
{
	return GetChildItem( hCompItem, driveName );
}


// M Find a Directory in a parent Directory
HTREEITEM CLeftView::GetDirItem( HTREEITEM hParentDirItem, CString dirName ) // M
{
	return GetChildItem( hParentDirItem, dirName );
}


void CLeftView::OnSelChanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UNREFERENCED_PARAMETER( pNMHDR ); UNREFERENCED_PARAMETER( pResult );
	TrackSelChange();
}


CString CLeftView::GetFullPath( HTREEITEM hItem )
{
	CString result = "";
	CTreeCtrl& ctlTree = GetTreeCtrl();

	while( hItem != NULL )
	{
		CString curText = ctlTree.GetItemText( hItem );
		if( curText != strMyComp )
			result = AddWithSlash( curText, result );
			/* (2) Was:
			if( curText.Right( 1 ) == "\\" )
				result =  curText + result;
			else
				result =  curText +"\\" + result;
			*/
		hItem = ctlTree.GetParentItem( hItem );
	}
	return result;
}


void CLeftView::TrackSelChange()
{
// Let's know the full path to this folder
	CTreeCtrl& ctlTree = GetTreeCtrl();
	HTREEITEM hItemSel = ctlTree.GetSelectedItem();

	CString text = GetFullPath( hItemSel );
	CFilesToArcView *view =	GetDocument()->m_pListView;
	view->SetRedraw( FALSE );
	view->ShowFileList( text );
	view->SetRedraw( TRUE );

	m_pDocument->SetTitle( "Files To Archive - " + text ); // M
}


void CLeftView::UpdateTree()
{
	CTreeCtrl& ctlTree = GetTreeCtrl();

	EraseTree();


//= My Computer - it will be forever =======================================
	TV_INSERTSTRUCT ins;
	ins.hParent = 0;
	ins.hInsertAfter = 0;
	ins.item.mask = TVIF_TEXT;
	ins.item.pszText = strMyComp;

	ins.item.mask |= TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	ins.item.iImage = CLeftView::IID_MYCOMPUTER;
	ins.item.iSelectedImage = CLeftView::IID_MYCOMPUTER;

/*
//	m_pTree->SetTextColor( RGB(0,0,255) ); // Без толку
	ins.item.mask |= TVIF_STATE;
	ins.item.state = TVIS_BOLD;
*/
	HTREEITEM hMyComp = ctlTree.InsertItem(&ins);
	DBG_UNREFERENCED_LOCAL_VARIABLE(hMyComp);	
	
//= Files =======================================
	for( POSITION pos=g_TheArchive.m_FilesToArc.GetHeadPosition(); pos != NULL; )
	{
// Проверять, что такой еще не добавлен в Дерево
// НЕТ, полностью обновлять каждый раз может быть слишком долго!
		CFileToArc *pCurFile = g_TheArchive.m_FilesToArc.GetNext(pos);

	// Find this drive in the Tree
	//============================================
	// Find the computer first
		CString compName;
		if( pCurFile->m_strComputer.IsEmpty() )
			compName = strMyComp;
		else
			compName = pCurFile->m_strComputer;
		HTREEITEM hCompItem = GetCompItem( compName );
		if( hCompItem == NULL )
		{
		// Add the computer to the Tree
			hCompItem = ctlTree.InsertItem( pCurFile->m_strComputer, 
					CLeftView::IID_MYCOMPUTER, // TO DO: Network(!) Computer
					CLeftView::IID_MYCOMPUTER, // TO DO
					0/*Parent*/ /*, =InsertAfterLast*/ );
		}

	// Then find this drive in My Computer
		HTREEITEM hDrvItem = GetDriveItem( hCompItem, pCurFile->m_strDrive );
		if( hDrvItem == NULL )
		{
			hDrvItem = ctlTree.InsertItem( pCurFile->m_strDrive, 
					CLeftView::IID_HARDDISK, // TO DO: Type !!!
					CLeftView::IID_HARDDISK, // TO DO: Type !!!
					hCompItem /*Parent*/ /*, =InsertAfterLast*/ );
			if( hDrvItem == NULL )
			{
				AfxMessageBox( _T("UpdateTree(): Can't add item! [1]") );
				break;
			}
		}
				
	// Then find this Directory in this Drive
		int nPos = 0;
		HTREEITEM hCurDirItem, hParentDirItem = hDrvItem;
		CString subDirs = pCurFile->m_strDir;
		while( true )
		{
			CString curDir;
			nPos = subDirs.Find( _T("\\"), 0 );
			if( nPos == -1 )
				break;
			curDir = subDirs.Left( nPos );
			subDirs = subDirs.Right( subDirs.GetLength()-nPos-1 );
			if( nPos != 0 )	// Excluding the leading slash
			{
				hCurDirItem = GetDirItem( hParentDirItem, curDir );
				if( hCurDirItem == NULL )
				{
					hCurDirItem = ctlTree.InsertItem( curDir, 
						CLeftView::IID_FOLDER, // TO DO: Type !!!
						CLeftView::IID_FOLDER, // TO DO: Type !!!
						hParentDirItem /*Parent*/ /*, =InsertAfterLast*/ );
					if( hCurDirItem == NULL )
					{
						AfxMessageBox( _T("UpdateTree(): Can't add item! [2]") );
						break;
					}
				}
				hParentDirItem = hCurDirItem;
			}
		}
	}
}


void CLeftView::EraseTree()
{
	CTreeCtrl& ctlTree = GetTreeCtrl();
	ctlTree.DeleteAllItems();
}
