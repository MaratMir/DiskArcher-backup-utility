// DiskArcher.
// Implementation of the CNewFilesLocatorView class.
// (C) Marat Mirgaleev, 2002.
// Created 25.07.2002.
// Modifications:
//	(1) 16.01.2003. EraseList() and ShowPopumMenu() moved into the base class.
//	(2) 02.12.2003. Processing of a return code of CFilesToArc::Add().
//  (3) 10.07.2004. Reconstruction of Locator.
//                  CountForCopying() eliminated because it is never used.
//                  File list created in addition to the ListCtrl.
//============================================================================

#include "stdafx.h"
#include "../MArc2.h"
#include "../MArcCore/CMyArchive.h"
#include "CNewFilesLocator.h"
#include "CNewFilesLocatorDoc.h"
#include "CNewFilesLocatorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CNewFilesLocatorView, CMyListView)

BEGIN_MESSAGE_MAP(CNewFilesLocatorView, CMyListView)
	//{{AFX_MSG_MAP(CNewFilesLocatorView)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(LVN_KEYDOWN, OnKeyDown)
	ON_COMMAND(ID_LOCATOR_ADD, OnLocatorAdd)
	ON_UPDATE_COMMAND_UI(ID_LOCATOR_ADD, OnUpdateLocatorAdd)
	ON_COMMAND(ID_LOCATOR_REMOVE, OnLocatorRemove)
	ON_UPDATE_COMMAND_UI(ID_LOCATOR_REMOVE, OnUpdateLocatorRemove)
	ON_WM_DESTROY()
	ON_COMMAND(ID_LOCATOR_ADD_EXCL_FOLDER, OnLocatorAddExclFolder)
	ON_UPDATE_COMMAND_UI(ID_LOCATOR_ADD_EXCL_FOLDER, OnUpdateLocatorAddExclFolder)
	ON_COMMAND(ID_LOCATOR_ADD_EXCL_TYPE, OnLocatorAddExclType)
	ON_UPDATE_COMMAND_UI(ID_LOCATOR_ADD_EXCL_TYPE, OnUpdateLocatorAddExclType)
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRightClick)
END_MESSAGE_MAP()


CNewFilesLocatorView::CNewFilesLocatorView()
{
// LATER: Change when columns dragged and dropped
	m_nPathColumn = 0;	
	m_nNameColumn = 1;	
	m_nTimeColumn = 2;
	m_nSizeColumn = 3;

// (3)	m_pDlg = NULL;
}


// (3) Constructs a new object! Delete it after use!
//------------------------------------------------------------------------------
CFileOnDisk* CNewFilesLocatorView::GetFileFromRow( const int nRow ) const
{
	CListCtrl& ctlList = GetListCtrl();
	CString path = ctlList.GetItemText( nRow, m_nPathColumn );
	CString name = ctlList.GetItemText( nRow, m_nNameColumn );
    CString fullName = AddWithSlash( path, name );
    return new CFileOnDisk( fullName );
}


// Add a file to ListCtrl
//------------------------------------------------------------------------------
void CNewFilesLocatorView::AddFileToListCtrl( const CFileOnDisk* pFile )
{
	bool bOk = true;
	CListCtrl& ctlList = GetListCtrl();
	LV_ITEM lvItem;
	lvItem.iItem = ctlList.GetItemCount();	// Insert at the end
	lvItem.mask = LVIF_TEXT;
	lvItem.iSubItem = m_nPathColumn;
	CString sTmp = pFile->getFullPath();
	LPCTSTR strItem = sTmp;
	lvItem.pszText = (LPTSTR)strItem;
	int newItem = ctlList.InsertItem(&lvItem);

	lvItem.mask = LVIF_TEXT;
	lvItem.iItem = newItem;
	lvItem.iSubItem = m_nNameColumn;
	strItem = pFile->m_strName;
	lvItem.pszText = (LPTSTR)strItem;
	if( ! ctlList.SetItem(&lvItem) )
		bOk = false;

	lvItem.mask = LVIF_TEXT;
	lvItem.iItem = newItem;
	lvItem.iSubItem = m_nTimeColumn;
	CString dt = pFile->m_LastWriteTime.Format();
	strItem = dt;
	lvItem.pszText = (LPTSTR)strItem;
	if( ! ctlList.SetItem(&lvItem) )
		bOk = false;

// (3) Size column added
	lvItem.mask = LVIF_TEXT;
	lvItem.iItem = newItem;
	lvItem.iSubItem = m_nSizeColumn;
  CString tmp;
  tmp.Format( L"%I64u", pFile->getSize() );
	strItem = tmp;
	lvItem.pszText = (LPTSTR)strItem;
	if( ! ctlList.SetItem(&lvItem) )
		bOk = false;
}


// (3) Removes files of given type from the ListCtrl.
// Returns how many rows are removed.
//------------------------------------------------------------------------------
int CNewFilesLocatorView::RemoveFilesOfType( const CString sExtension )
{
	CListCtrl& ctlList = GetListCtrl();
	int nRemoved = 0;
	while( true )
	{
        int itemCount = ctlList.GetItemCount();
			// Yes, start it again because of deleting and 
			//	positions changing
        bool bRemoved = false;
        for( int nItem=0; nItem < itemCount; nItem++ )
        {
            CFileOnDisk* pCurFile = GetFileFromRow( nItem );    // Constructs!
            CString sCurExt = pCurFile->getExtension();         // ^^^^^
            delete pCurFile;                                    // Frees
            sCurExt.MakeUpper();    // Will compare in upper case
            if( sExtension == sCurExt )
            {
    		    ctlList.DeleteItem( nItem );
                nRemoved++;
                bRemoved = true;
                break;  // Actually, we break to start it again
            }
        }
        if( ! bRemoved )    // All appropriate rows deleted
            break;
        // else - start again
	}
    return nRemoved;
}


void CNewFilesLocatorView::RemoveFilesOfFolder( const CFileOnDisk* pFile )
{
  UNREFERENCED_PARAMETER( pFile );
  // And subfolders?
}


// CNewFilesLocatorView diagnostics
//=================================================================

#ifdef _DEBUG
void CNewFilesLocatorView::AssertValid() const
{
	CListView::AssertValid();
}

void CNewFilesLocatorView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG



// CNewFilesLocatorView message handlers
//=================================================================

void CNewFilesLocatorView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

/* (3) Eliminated or moved somewhere
	m_pDlg = new CNewFilesLocatorDlg( this );
	if ( ! m_pDlg->Create( IDD_LOCATOR_DIALOG ) )
		AfxMessageBox( "Cannot create Locator Dialog." );
	else
	{
		m_pDlg->ShowWindow( SW_NORMAL );
		m_pDlg->CenterWindow();
	}
*/
}


void CNewFilesLocatorView::OnRightClick( NMHDR *pNotifyStruct, LRESULT *result )
{
	MyOnRightClick( pNotifyStruct );
	*result = 0;
}


int CNewFilesLocatorView::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	SetPopupMenu( IDR_LOCATOR_POPUP );	// (1)
	lpCreateStruct->style |= LVS_REPORT /*| LVS_NOSORTHEADER */;
	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CListCtrl& ctlList = (CListCtrl&) GetListCtrl();
	ctlList.SetExtendedStyle( /* LATER: LVS_EX_HEADERDRAGDROP |*/
		LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_SUBITEMIMAGES );

	int res;
	res=ctlList.InsertColumn( m_nPathColumn, _T("Path"),	   LVCFMT_LEFT, 250 );
	res=ctlList.InsertColumn( m_nNameColumn, _T("Filename"),   LVCFMT_LEFT, 120 );
	res=ctlList.InsertColumn( m_nTimeColumn, _T("Date, Time"), LVCFMT_LEFT, 120 );

// (3) Size column added
	res=ctlList.InsertColumn( m_nSizeColumn, _T("Size"), LVCFMT_RIGHT, 80 );

	return 0;
}

/* (1)
void CNewFilesLocatorView::EraseList()
{
	CListCtrl& ctlList = GetListCtrl();
	ctlList.DeleteAllItems();
}
*/
/* (1)
// Show Popup menu.
//=================================================================
void CNewFilesLocatorView::ShowPopupMenu( int nItemSel, CPoint curPoint )
{
	DoPopupMenu( IDR_LOCATOR_POPUP, curPoint );
}
*/

void CNewFilesLocatorView::OnKeyDown( NMHDR* pNMHDR, LRESULT* pResult )
{
	MyOnKeyDown( pNMHDR );
	*pResult = 0;
}


// Add selected files to the Archive
//------------------------------------------------------------------------------
void CNewFilesLocatorView::OnLocatorAdd() 
{
	bool isAnythingAdded = false;
	CListCtrl& ctlList = GetListCtrl();
	int nItem;
// For every selected file
	while( true )
	{
		POSITION pos = ctlList.GetFirstSelectedItemPosition();
			// Yes, start it again because of deleting and 
			//	positions changing
		if( ! pos )
			break;
		nItem = ctlList.GetNextSelectedItem(pos);

	// Add file to Archive
		CString path = ctlList.GetItemText( nItem, m_nPathColumn );
		CString name = ctlList.GetItemText( nItem, m_nNameColumn );
		if( g_TheArchive.m_FilesToArc.FileAdd( AddWithSlash( path, name )) )
		{
		// (3) Remove the file from the list of found files
      m_pLocator->m_foundFiles.Remove( path, name );
                // It will call delete for the file
		// Remove the file from the list control
			ctlList.DeleteItem( nItem );
			isAnythingAdded = true;
		}
		else
			break;
	}

// Update FilesToArc window
	if( isAnythingAdded )
		theApp.UpdateTreeAndList();
}


void CNewFilesLocatorView::OnUpdateLocatorAdd(CCmdUI* pCmdUI) 
{
// Enable it if anything is selected
	pCmdUI->Enable( IsAnythingSelected() );
}


void CNewFilesLocatorView::OnLocatorRemove() 
{
	CListCtrl& ctlList = GetListCtrl();
	int nItem;
// For every selected file
	while( true )
	{
		POSITION pos = ctlList.GetFirstSelectedItemPosition();
			// Yes, start it again because of deleting and 
			//	positions changing
		if( ! pos )
			break;
		nItem = ctlList.GetNextSelectedItem(pos);

	// Remove the file from the list
		ctlList.DeleteItem( nItem );
	}
}


void CNewFilesLocatorView::OnUpdateLocatorRemove(CCmdUI* pCmdUI) 
{
// Enable it if something is selected
	pCmdUI->Enable( IsAnythingSelected() );
}


void CNewFilesLocatorView::OnDestroy() 
{
	CMyListView::OnDestroy();
// Add your message handler code here

    g_TheArchive.m_pLocator = NULL;   // (3)
    delete m_pLocator;  // (3)
}


void CNewFilesLocatorView::OnUpdateLocatorAddExclType(CCmdUI* pCmdUI) 
{
// Enable it if something is selected
	pCmdUI->Enable( IsAnythingSelected() );
}


void CNewFilesLocatorView::OnLocatorAddExclType() 
{
	CWaitCursor wait;
	CListCtrl& ctlList = GetListCtrl();
	int nItem;
// For every selected file
	while( true )
	{
		POSITION pos = ctlList.GetFirstSelectedItemPosition();
			// Yes, start it again because of deleting and 
			//	positions changing
        bool bRemoved = false;
		while( pos != NULL )
        {
		    nItem = ctlList.GetNextSelectedItem(pos);
            CFileOnDisk* pCurFile = GetFileFromRow( nItem );
            bRemoved = m_pLocator->CheckAndAddToExclTypes( pCurFile );
            delete pCurFile;
            if( bRemoved )
                break;  // Start again
        }
        if( ! bRemoved )
            break;  // Done, nothing to remove more
	}
}


void CNewFilesLocatorView::OnUpdateLocatorAddExclFolder(CCmdUI* pCmdUI) 
{
// Enable it if something is selected
	pCmdUI->Enable( IsAnythingSelected() );
}


void CNewFilesLocatorView::OnLocatorAddExclFolder() 
{
// For each selected item
// Get file's folder
// Check that there isn't this folder in the Excluded
}
