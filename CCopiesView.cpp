// DiskArcher.
// Copies window.
// (C) Marat Mirgaleev, 2001-2006.
// Modifications:
//	(1) 20.01.2002. "Miscelaneous.h" added.
//	(2) 09.02.2002. GetSelectedCopy() added.
//	(3) 10.02.2002. Icons added.
//					        Deletion added.
//	(4) 19.02.2002. Column "BundleId" replaced by "RoomID". 
//	(5) 09.03.2002. Extract: check is the Room availlable.
//	(6) 05.04.2002. Delete: an error corrected.
//	(7) 03.07.2002. Wait cursor.
//	(8) 16.01.2003. EraseList() moved into base class.
//	(9) 26.03.2003. Implementation of copy extraction moved into CFileCopy.
//					Nag screen on extraction.
// (10) 26.10.2003. Extraction: Path instead of filename as a parameter.
// (11) 24.05.2006. Copy protection improved.
//==============================================================================

#include "stdafx.h"
#include "MArc2.h"

#include "CCopiesView.h"
#include "CCopiesDoc.h"
#include "CMyArchive.h"
#include "CNag.h"		// (9)
#include "CFileToArc.h"
#include "CRoom.h"
#include "Miscelaneous.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CCopiesView, CMyListView)

//==============================================================================
BEGIN_MESSAGE_MAP(CCopiesView, CMyListView)
	//{{AFX_MSG_MAP(CCopiesView)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(LVN_KEYDOWN, OnKeyDown)
	ON_UPDATE_COMMAND_UI(ID_COPY_EXTRACT, OnUpdateCopyExtract)
	ON_COMMAND(ID_COPY_EXTRACT, OnCopyExtract)
	ON_COMMAND(ID_COPY_DELETE, OnCopyDelete)
	ON_UPDATE_COMMAND_UI(ID_COPY_DELETE, OnUpdateCopyDelete)
	ON_UPDATE_COMMAND_UI(ID_COPY_PROPERTIES, OnUpdateCopyProperties)
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRightClick)
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CCopiesView construction/destruction

//==============================================================================
CCopiesView::CCopiesView()
{
	m_nIDColumn   = 0;	// TO DO: Change when columns dragged and dropped
	m_nPathColumn = 1;	
	m_nNameColumn = 2;	
	m_nTimeColumn = 3;
	m_nRoomIDColumn = 4;
		// (4) Was m_nBundleIDColumn = 4;
}


//==============================================================================
BOOL CCopiesView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CListView::PreCreateWindow(cs);
}


//==============================================================================
void CCopiesView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	ShowCopyList();
}


// CCopiesView diagnostics
//==============================================================================
#ifdef _DEBUG
void CCopiesView::AssertValid() const
{
	CListView::AssertValid();
}

void CCopiesView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CCopiesDoc* CCopiesView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCopiesDoc)));
	return (CCopiesDoc*)m_pDocument;
}
#endif //_DEBUG



/////////////////////////////////////////////////////////////////////////////
// CCopiesView message handlers

//==============================================================================
void CCopiesView::OnRightClick(NMHDR *pNotifyStruct, LRESULT *result)
{
	MyOnRightClick( pNotifyStruct );
	*result = 0;
}


//==============================================================================
int CCopiesView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	lpCreateStruct->style |= LVS_REPORT | /*LVS_NOSORTHEADER |*/ LVS_SINGLESEL;
	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
// Give the document a pointer to this view
	GetDocument()->m_pListView = this;
	
	CListCtrl& ctlList = (CListCtrl&) GetListCtrl();
	ctlList.SetExtendedStyle( /* TO DO LVS_EX_HEADERDRAGDROP |*/
		LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_SUBITEMIMAGES );

// Create the Image List
	BOOL crRes = m_ctlImage.Create( IDB_COPIES_IMAGELIST, 16, 0, 
									RGB(255,0,255));
	ASSERT( crRes );
//	ctlList.SetImageList( &m_ctlImage, LVSIL_NORMAL );
	ctlList.SetImageList( &m_ctlImage, LVSIL_SMALL );
//	ctlList.SetImageList( &m_ctlImage, LVSIL_STATE );
	
	int res;
	VERIFY( (res=ctlList.InsertColumn( m_nIDColumn, _T("CopyID"), 
									   LVCFMT_RIGHT, 60 )) >= 0 );
	res=ctlList.InsertColumn( m_nPathColumn, _T("Path"), LVCFMT_LEFT,  250 );
	res=ctlList.InsertColumn( m_nNameColumn, _T("Filename"), 
							  LVCFMT_LEFT, 120 );
	res=ctlList.InsertColumn( m_nTimeColumn, _T("Date, Time"), 
							  LVCFMT_LEFT, 170 );
	res=ctlList.InsertColumn( m_nRoomIDColumn, _T("RoomID"), LVCFMT_RIGHT, 70);
		// (4) Was res=ctlList.InsertColumn( m_nBundleIDColumn, _T("BundleID"),
		//									 LVCFMT_RIGHT, 70 );

	return 0;
}


//==============================================================================
void CCopiesView::OnCopyProperties() 
{
/* LATER
// Get selected Copy
	CListCtrl& ctlList = GetListCtrl();
	int nItem;
	POSITION pos = ctlList.GetFirstSelectedItemPosition();
	CFileCopy *pSelCopy;
	if (pos != NULL)
	//	while (pos)
		{
			nItem = ctlList.GetNextSelectedItem(pos);
			CString txt = ctlList.GetItemText( nItem, m_nIDColumn );
			int nID = atoi( txt );
			pSelCopy = theArchive.CopyFind( nID );
		}
	
// Show Dialog
	CCopyPropDialog dlg;
	dlg.m_Limit = pSelCopy->m_nSizeLimit;
	int result = dlg.DoModal();
	if (result==IDOK)
	{
	// Store changed data
		pSelCopy->m_nSizeLimit = dlg.m_Limit;

		if( ! theDB.CopyUpdate( pSelCopy ) )
			result = IDCANCEL;
	}
	if (result==IDOK)
	// Update List Control
		SetSubItems( nItem, pSelCopy );
*/
}


//==============================================================================
void CCopiesView::OnKeyDown(NMHDR* pNMHDR, LRESULT* pResult) 
{
	MyOnKeyDown( pNMHDR );
	*pResult = 0;
}


// Probably, here must be the strongest copy protection check! And it is now.
//==============================================================================
void CCopiesView::OnCopyExtract() 
{

// Ask where extract to
//......................
	CString strExtractTo;
	bool bSuccess = BrowseForFolder( "Select a folder where extract the copy to",
							                     strExtractTo );
	if( bSuccess )
	{
  // Nag screen (9) ..
    if( g_TheArchive.m_Copies.GetCount() > 50 ) // (11)
                                  // zzz ј срок использовани€ не надо провер€ть?
	    if ( ! theApp.checkRegistration3() )
	                // (11) Was: if ( ! theApp.CheckRegistration() )
	    {
				AfxMessageBox( _T("You have exceeded the limit for unregistered users.\n")
							         _T("Please register.") );  // (11)
        return;                               /* (11) Was:
        CNag nagScreen;				// (9)
		    nagScreen.DoModal();	// (9) */
	    }

// TO DO: Default - to original path?

// TO DO: Option: with/without path

	// Get selected Copy.	TO DO: Get several selected Copies
	// There is a selected copy definitely 
	//		because otherwise this function can't be called
	//			because buttons would be disabled
		CFileCopy* pSelCopy = GetSelectedCopy();
			// (10) Was: ExtractToFile = strExtractTo + pSelCopy->m_strFilename;
		bSuccess = pSelCopy->Extract( strExtractTo ); // (10) Was strExtractToFile
	}
}


// (3)
//==============================================================================
void CCopiesView::OnCopyDelete() 
{
	CFileCopy* pSelCopy = GetSelectedCopy();
	CFileToArc* pSelFile = g_TheArchive.m_FilesToArc.FileFind( pSelCopy->m_strPath 
														                        + pSelCopy->m_strFilename );
// (6)	ASSERT( pSelFile );
	if( pSelFile/*(6)*/
	 &&	pSelFile->m_bSystem )
		AfxMessageBox( _T("This file is for system purposes.\n")
					   _T("You cannot delete its copies.") );
	else
	{
		if( pSelCopy->m_bDeleteIt )
			AfxMessageBox( _T("This copy has already been marked for deletion\n")
						   _T("and will be actually deleted during next\n")
						   _T("Archive Updating process.") );
		else
			if( AfxMessageBox( _T("Are you sure to delete this copy?"), MB_YESNO ) == IDYES )
			{
			// Mark the Copy for deletion.
			// It will be deleted during the next Archiving process
				pSelCopy->MarkForDeletion();
			/* (3) Was:	pSelCopy->m_bDeleteIt = true;
				theDB.CopyUpdate( pSelCopy ); */
				ShowCopyList();
			}
	}
}


//==============================================================================
void CCopiesView::OnUpdateCopyExtract(CCmdUI* pCmdUI) 
{
// Enable it if anything is selected
	CListCtrl& ctlList = GetListCtrl();
	POSITION pos = ctlList.GetFirstSelectedItemPosition();
	pCmdUI->Enable( pos != NULL );
}


//==============================================================================
void CCopiesView::OnUpdateCopyDelete(CCmdUI* pCmdUI) 
{
// Enable it if anything is selected
	CListCtrl& ctlList = GetListCtrl();
	POSITION pos = ctlList.GetFirstSelectedItemPosition();
	pCmdUI->Enable( pos != NULL );
}


//==============================================================================
void CCopiesView::OnUpdateCopyProperties(CCmdUI* pCmdUI) 
{
// Enable it if anything is selected
	CListCtrl& ctlList = GetListCtrl();
	POSITION pos = ctlList.GetFirstSelectedItemPosition();
	pCmdUI->Enable( pos != NULL );
}



///////////////////////////////////////////////////////////////////
// Class Methods

//==============================================================================
void CCopiesView::ShowCopyList()
{
	CListCtrl& ctlList = GetListCtrl();

// Generate Column Headers
	EraseList();

	for( POSITION pos=g_TheArchive.m_Copies.GetHeadPosition(); pos != NULL; )
	{
		bool bShowIt = true;
		CFileCopy *pCurCopy = g_TheArchive.m_Copies.GetNext(pos);
		if( ( !m_strPath.IsEmpty() ) && ( !m_strFilename.IsEmpty() ) )
		// If they are not empty, show only copies of this file (with this path)
			if( ( m_strPath != pCurCopy->m_strPath ) || (m_strFilename != pCurCopy->m_strFilename ) )
				bShowIt = false;

		if( bShowIt )
		{
			LV_ITEM lvItem;
			lvItem.mask = LVIF_TEXT | LVIF_IMAGE;
			lvItem.iItem = ctlList.GetItemCount();	// Insert at the end
			lvItem.iSubItem = m_nIDColumn;
				// TO DO: if it is not 0, it will not work!!!
			CString sTmp;
			sTmp.Format( _T("%d"), pCurCopy->m_nCopyID );
			LPCTSTR strItem = sTmp;
			lvItem.pszText = (LPTSTR)strItem;
			lvItem.iImage = ( pCurCopy->m_bDeleteIt ? 0 : -1 );
			int newItem = ctlList.InsertItem(&lvItem);
			//BOOL res = ctlList.SetItemState( newItem, INDEXTOSTATEIMAGEMASK( 77/*TO DO nState*/),
			//	LVIS_STATEIMAGEMASK );//INDEXTOSTATEIMAGEMASK(nState) );

			SetSubItems( newItem, pCurCopy );
		}
	}
}


//==============================================================================
bool CCopiesView::SetSubItems( int nItem, CFileCopy* pCopy )
{
	bool bOk = true;
	CString sTmp;
	LPCTSTR strItem;
	CListCtrl& ctlList = GetListCtrl();
	LV_ITEM lvItem;

	lvItem.mask = LVIF_TEXT;
	lvItem.iItem = nItem;
	lvItem.iSubItem = m_nPathColumn;
	strItem = pCopy->m_strPath;
	lvItem.pszText = (LPTSTR)(strItem);
	if( ! ctlList.SetItem(&lvItem) )
		bOk = false;

	lvItem.mask = LVIF_TEXT;
	lvItem.iItem = nItem;
	lvItem.iSubItem = m_nNameColumn;
	strItem = pCopy->m_strFilename;
	lvItem.pszText = (LPTSTR)strItem;
	if( ! ctlList.SetItem(&lvItem) )
		bOk = false;

	lvItem.mask = LVIF_TEXT;
	lvItem.iItem = nItem;
	lvItem.iSubItem = m_nTimeColumn;
	CString dt = pCopy->m_FileDateTime.Format();
	strItem = dt;
	lvItem.pszText = (LPTSTR)strItem;
	if( ! ctlList.SetItem(&lvItem) )
		bOk = false;

	lvItem.mask = LVIF_TEXT;
	lvItem.iItem = nItem;
	lvItem.iSubItem = m_nRoomIDColumn;
	sTmp.Format( _T("%d"), pCopy->GetRoom()->m_nRoomID );
	/*	(4) Was lvItem.iSubItem = m_nBundleIDColumn;
				sTmp.Format( "%d", pCopy->m_nBundleID );
	*/	
	strItem = sTmp;
	lvItem.pszText = (LPTSTR)(strItem);
	if( ! ctlList.SetItem(&lvItem) )
		bOk = false;

	return bOk;
}


// Show Popup menu:
//	If any Copy is selected - IDR_Copy_POPUP,
//  If nothing is selected  - IDR_Copies_POPUP.
//==============================================================================
void CCopiesView::ShowPopupMenu( int nItemSel, CPoint curPoint )
{
	if( nItemSel == -1 )
	// Nothing is selected
		DoPopupMenu( IDR_COPIES_POPUP, curPoint );
	else
		DoPopupMenu( IDR_COPY_POPUP, curPoint );
}


// (2) Return a pointer to a File Copy which is selected in ListControl.
//==============================================================================
CFileCopy* CCopiesView::GetSelectedCopy()
{
	CFileCopy *pSelCopy = NULL;
	CListCtrl& ctlList = GetListCtrl();
	int nItem;
	POSITION pos = ctlList.GetFirstSelectedItemPosition();
	if (pos != NULL)
		//	while (pos)   TO DO: Multi-select - is it useless here?
			{
				nItem = ctlList.GetNextSelectedItem(pos);
				CString txt = ctlList.GetItemText( nItem, m_nIDColumn );
				int nID = _ttoi(txt);  //atoi( txt );
				pSelCopy = g_TheArchive.m_Copies.FindCopy( nID );
			}
	return pSelCopy;
}
