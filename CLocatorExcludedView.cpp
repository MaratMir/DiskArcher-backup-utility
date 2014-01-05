// DiskArcher.
// Implementation of the CLocatorExcludedView class.
// (C) Marat Mirgaleev, 2003.
// Created 14.01.2003.
// Modifications:
//  (1) 08.04.03. Ask before restore defaults.
//=================================================================

#include "stdafx.h"
#include "CArchiveDB.h"	// It also includes ADO headers
#include "CInputStringDialog.h"
#include "CNewFilesLocator.h"
#include "CLocatorExcludedDoc.h"
#include "CLocatorExcludedView.h"
#include "CMyArchive.h"

#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CLocatorExcludedView, CMyListView)

BEGIN_MESSAGE_MAP(CLocatorExcludedView, CMyListView)
	//{{AFX_MSG_MAP(CLocatorExcludedView)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(LVN_KEYDOWN, OnKeyDown)
	ON_WM_DESTROY()
	ON_COMMAND(ID_LOCATOR_EXCL_ADD, OnLocatorExclAdd)
	ON_UPDATE_COMMAND_UI(ID_LOCATOR_EXCL_ADD, OnUpdateLocatorExclAdd)
	ON_COMMAND(ID_LOCATOR_EXCL_REMOVE, OnLocatorExclRemove)
	ON_UPDATE_COMMAND_UI(ID_LOCATOR_EXCL_REMOVE, OnUpdateLocatorExclRemove)
	ON_COMMAND(ID_LOCATOR_EXCL_ACCEPT, OnLocatorExclAccept)
	ON_UPDATE_COMMAND_UI(ID_LOCATOR_EXCL_ACCEPT, OnUpdateLocatorExclAccept)
	ON_UPDATE_COMMAND_UI(ID_LOCATOR_EXCL_RESTORE_DEFAULTS, OnUpdateLocatorExclRestoreDefaults)
	ON_COMMAND(ID_LOCATOR_EXCL_RESTORE_DEFAULTS, OnLocatorExclRestoreDefaults)
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRightClick)
END_MESSAGE_MAP()


void CLocatorExcludedView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();
}


CLocatorExcludedDoc* CLocatorExcludedView::GetDocument()
{
	CDocument* pDoc = CListView::GetDocument();
	CLocatorExcludedDoc* pExFDoc = (CLocatorExcludedDoc*)pDoc;
	return pExFDoc;
}


CNewFilesLocator* CLocatorExcludedView::GetLocator()
{
	CLocatorExcludedDoc* pExFDoc = GetDocument();
	CNewFilesLocator* pLocator = pExFDoc->m_pLocator;
	return pLocator;
}


// Returns one of two lists that is the source for current mode
CStringList* CLocatorExcludedView::GetSourceList()
{
	CStringList* pSrcList = NULL;
	CNewFilesLocator* pLocator = GetLocator();
	DBG_UNREFERENCED_LOCAL_VARIABLE( pLocator );
	switch( GetDocument()->m_nShowType )
	{
		case LocExclFileTypes:
			pSrcList = &( pLocator->m_excludedFileTypes );
			break;
		case LocExclFolders:
			pSrcList = &( pLocator->m_excludedFolders );
			break;
	}
	return pSrcList;
}


// Add a file or folder to ListCtrl.
// Parameters:	- fname - a folder name or a file type extension.
// Returns -1 if fails.
int CLocatorExcludedView::AddRowToListCtrl( CString fname )
{
	CListCtrl& ctlList = GetListCtrl();
	LV_ITEM lvItem;
	lvItem.iItem = ctlList.GetItemCount();	// Insert at the end
	lvItem.mask = LVIF_TEXT;
	lvItem.iSubItem = 0;
	LPCTSTR strItem = fname;
	lvItem.pszText = (LPTSTR)strItem;
	int newItem = ctlList.InsertItem(&lvItem);
	return newItem;
}


void CLocatorExcludedView::ShowList( enum LocatorWhatToExclude nShowType )
{
	GetDocument()->m_nShowType = nShowType;

	EraseList();
	CListCtrl& ctlList = GetListCtrl();
	while( ctlList.DeleteColumn(0) );
	int res;

	CStringList* pSrcList = GetSourceList();
	CDocument* pDoc = GetDocument();
	switch( nShowType )
	{
		case LocExclFileTypes:
			res = ctlList.InsertColumn( 0, _T("File type"), LVCFMT_LEFT, 80 );
			pDoc->SetTitle( _T("File types to exclude from search" ));
			break;
		case LocExclFolders:
			res = ctlList.InsertColumn( 0, _T("Folder name"), LVCFMT_LEFT, 200 );
			pDoc->SetTitle( _T("Folders to exclude from search") );
			break;
	}
	POSITION pos;
	for( pos = pSrcList->GetHeadPosition(); pos != NULL; )
	{
		AddRowToListCtrl( pSrcList->GetNext( pos ) );
	}
}


// CLocatorExcludedView diagnostics
//=================================================================

#ifdef _DEBUG
void CLocatorExcludedView::AssertValid() const
{
	CListView::AssertValid();
}

void CLocatorExcludedView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG



// CLocatorExcludedView message handlers
//=================================================================

void CLocatorExcludedView::OnRightClick(NMHDR *pNotifyStruct, LRESULT *result)
{
	MyOnRightClick( pNotifyStruct );
	*result = 0;
}


int CLocatorExcludedView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	SetPopupMenu( IDR_LOCATOR_EXCL_POPUP );
	GetParent()->SetWindowPos( &wndTop, 75, 50, 320, 300, SWP_SHOWWINDOW );

	lpCreateStruct->style |= LVS_REPORT | LVS_SINGLESEL;
	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CListCtrl& ctlList = (CListCtrl&) GetListCtrl();
	ctlList.SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	return 0;
}


// TODO: And what about <Enter> key?
void CLocatorExcludedView::OnKeyDown(NMHDR* pNMHDR, LRESULT* pResult) 
{
	MyOnKeyDown( pNMHDR );
	*pResult = 0;
}


void CLocatorExcludedView::OnDestroy() 
{
	CMyListView::OnDestroy();
}


void CLocatorExcludedView::OnUpdateLocatorExclAdd(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( true );
}


// "Add one more item" command
void CLocatorExcludedView::OnLocatorExclAdd() 
{
// Ask user the string
	CInputStringDialog dlg;
	dlg.m_nMode = CInputStringDialog::dlgTypeAdd;
	int nRet = dlg.DoModal();

    switch ( nRet )
	{
		case IDABORT:
		case IDCANCEL:	// CANCEL - for pressing a cross in the window's right
			break;      //          top corner
		
		case IDOK:		// Ok, add the given item to the list
			GetDocument()->SetModifiedFlag();	

			dlg.m_strEdit.MakeUpper();
			AddRowToListCtrl( dlg.m_strEdit );	// Add to the ListCtrl

		// Add to the excludedFolders or excludedFileTypes list
			{
				CStringList* pSrcList = GetSourceList();
				pSrcList->AddTail( dlg.m_strEdit );
			}
			break;
		default:
			AfxMessageBox( _T("Sudden Error in 'OnLocatorExclAdd'") );
			break;
	}
}


void CLocatorExcludedView::OnLocatorExclRemove() 
{
	GetDocument()->SetModifiedFlag();	
	CListCtrl& ctlList = GetListCtrl();
	int nItem;
// For every selected item
	while( true )
	{
		POSITION posInCtrl = ctlList.GetFirstSelectedItemPosition();
			// Yes, start it again because of deleting and 
			//	positions changing
		if( ! posInCtrl )
			break;
		nItem = ctlList.GetNextSelectedItem( posInCtrl );

	// Remove the item from stringList
		CString sItem = ctlList.GetItemText( nItem, 0/*column*/ );
		POSITION posInStrList = GetSourceList()->Find( sItem );
		if( posInStrList )
			GetSourceList()->RemoveAt( posInStrList );

	// Remove the file from the list
		ctlList.DeleteItem( nItem );	// From the control
	}
}


void CLocatorExcludedView::OnUpdateLocatorExclRemove(CCmdUI* pCmdUI) 
{
// Enable it if anything is selected
	pCmdUI->Enable( IsAnythingSelected() );
}


void CLocatorExcludedView::OnUpdateLocatorExclAccept( CCmdUI* pCmdUI )
{
	pCmdUI->Enable( GetDocument()->IsModified() );
}


// Save changes to DB
void CLocatorExcludedView::OnLocatorExclAccept() 
{
	GetDocument()->Save();
	GetDocument()->m_pParentWindow->ShowWindow( SW_RESTORE );
	GetParentFrame()->DestroyWindow();
}


void CLocatorExcludedView::OnUpdateLocatorExclRestoreDefaults( CCmdUI* pCmdUI )
{
	pCmdUI->Enable( true );
}


void CLocatorExcludedView::OnLocatorExclRestoreDefaults() 
{
	if( AfxMessageBox( _T("Clear current settings\n")
					      _T("(both excluded files and folders)\n")
					       _T("and restore defaults?"), 
					   MB_YESNO ) == IDYES )	// (1)
	{
		g_pTheDB->LocatorRestoreDefaultOptions( 44 );
		GetLocator()->LoadOptions();
		ShowList( GetDocument()->m_nShowType );
	}
}

