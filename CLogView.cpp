// DiskArcher.
// CLogView.cpp - Implementation of the CLogView class.
// (C) Marat Mirgaleev, 2001-2002.
//	(1) 01.11.2002. theDB changed to *pTheDB.
//	(2) 16.01.2003. EraseList() and ShowPopupMenu() moved into the base class.
//============================================================================

#include "stdafx.h"
#include "CLogDoc.h"
#include "CLogView.h"
#include "CMyArchive.h"
#include "CArchiveDB.h"

#import "C:\Program Files\Common Files\system\ado\msado15.dll" no_namespace rename( "EOF", "adoEOF" )

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLogView

IMPLEMENT_DYNCREATE(CLogView, CListView)

BEGIN_MESSAGE_MAP(CLogView, CListView)
	//{{AFX_MSG_MAP(CLogView)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(LVN_KEYDOWN, OnKeyDown)
	ON_UPDATE_COMMAND_UI(ID_LOG_CLEAR, OnUpdateLogClear)
	ON_COMMAND(ID_LOG_CLEAR, OnLogClear)
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRightClick)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogView construction/destruction


CLogView::CLogView()
{
/*
	m_nIDColumn		  = 0;	// TO DO: Change when columns dragged and dropped
	m_nSizeColumn	  = 1;
	m_nModifiedColumn = 2;
*/
}


CLogView::~CLogView()
{
}


BOOL CLogView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CListView::PreCreateWindow(cs);
}


/////////////////////////////////////////////////////////////////////////////
// CLogView drawing

void CLogView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();
	ShowLog();
}


/////////////////////////////////////////////////////////////////////////////
// CLogView diagnostics

#ifdef _DEBUG
void CLogView::AssertValid() const
{
	CListView::AssertValid();
}

void CLogView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CLogDoc* CLogView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLogDoc)));
	return (CLogDoc*)m_pDocument;
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CLogView message handlers


void CLogView::OnRightClick(NMHDR *pNotifyStruct, LRESULT *result)
{
	MyOnRightClick( pNotifyStruct );
	*result = 0;
}


int CLogView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	SetPopupMenu( IDR_LOG_POPUP );
	lpCreateStruct->style |= LVS_REPORT | LVS_SINGLESEL;
	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CListCtrl& ctlList = (CListCtrl&) GetListCtrl();
	ctlList.SetExtendedStyle( /*TO DO LVS_EX_HEADERDRAGDROP*/ /*| LVS_EX_CHECKBOXES|*/
							  LVS_EX_FULLROWSELECT | 
							  LVS_EX_GRIDLINES | LVS_EX_SUBITEMIMAGES );

/*
// Create the Image List
	BOOL crRes = m_ctlImage.Create( IDB_FTOA_IMAGELIST, 16, 0, RGB(255,0,255));
	if( !crRes )
		AfxMessageBox( "CLogView::OnCreate(): Can't create Image List." );
//m_ctlImage.SetBkColor(GetSysColor(COLOR_WINDOW));	// TO DO
//	ctlList.SetImageList( &m_ctlImage, LVSIL_NORMAL );
//	ASSERT( ctlList.GetImageList( LVSIL_NORMAL ) == &m_ctlImage );
	ctlList.SetImageList( &m_ctlImage, LVSIL_SMALL );
//	ctlList.SetImageList( &m_ctlImage, LVSIL_STATE );
*/	
	int res;
	VERIFY( (res=ctlList.InsertColumn( 0, _T("Date, Time"), LVCFMT_LEFT, 120 )) >= 0 );
	res=ctlList.InsertColumn( 1, _T("Path"),     LVCFMT_LEFT, 160 );
	res=ctlList.InsertColumn( 2, _T("Filename"), LVCFMT_LEFT, 120 );
	res=ctlList.InsertColumn( 3, _T("Message"),	 LVCFMT_LEFT, 200 );
	return 0;
}


// Show files in a List Control.
// Parameters:	- if is not empty, show files of given folder.
//=====================================================================
void CLogView::ShowLog()
{
	CListCtrl& ctlList = GetListCtrl();

	EraseList();

	bool bSuccess = true;
	_RecordsetPtr recSet;
	recSet.CreateInstance(__uuidof(Recordset)); 
	HRESULT hr;
	try
	{
	// Select all Copies
		wchar_t* select = _T("SELECT * FROM ArcLog ORDER BY MessDateTime DESC");
		hr = recSet->Open( select, 
			// zzz hr = recSet->Open( (LPCSTR)select, 
							         g_pTheDB->m_pConnection, adOpenStatic, 
							         adLockReadOnly, adCmdText );
    TESTHR( hr );

		while( ! recSet->adoEOF )
		{
			_bstr_t  bstrTmp; // Temporary string for type conversion
			_variant_t vtTmp;

		// Date, Time
            vtTmp = recSet->Fields->Item["MessDateTime"]->Value;
			LV_ITEM lvItem;
			lvItem.mask = LVIF_TEXT; // | LVIF_IMAGE;
			lvItem.iItem = ctlList.GetItemCount();	// Insert at the end
			lvItem.iSubItem = 0;
			COleDateTime dt = vtTmp;
			CString sTmp = dt.Format();
			LPCTSTR strItem = sTmp;
			lvItem.pszText = (LPTSTR)strItem;
			lvItem.iImage = -1;
			int newItem = ctlList.InsertItem(&lvItem);
//			BOOL res = ctlList.SetItemState( newItem, INDEXTOSTATEIMAGEMASK( 11/*TO DO nState*/),
//				LVIS_STATEIMAGEMASK );//INDEXTOSTATEIMAGEMASK(nState) );

		// Path
            bstrTmp = recSet->Fields->Item["Path"]->Value;
			sTmp = (LPCSTR)bstrTmp;
			sTmp.TrimRight();
			lvItem.iItem = newItem;
			lvItem.iSubItem = 1;
			lvItem.mask = LVIF_TEXT;
			strItem = sTmp;
			lvItem.pszText = (LPTSTR)strItem;
			ctlList.SetItem( &lvItem );

		// Filename            
			bstrTmp = recSet->Fields->Item["Filename"]->Value;
			sTmp = (LPCSTR)bstrTmp;
			sTmp.TrimRight();
			lvItem.iItem = newItem;
			lvItem.iSubItem = 2;
			strItem = sTmp;
			lvItem.pszText = (LPTSTR)strItem;
			lvItem.mask = LVIF_TEXT;
			ctlList.SetItem( &lvItem );

		// Message            
			bstrTmp = recSet->Fields->Item["Message"]->Value;
			sTmp = (LPCSTR)bstrTmp;
			sTmp.TrimRight();
			lvItem.iItem = newItem;
			lvItem.iSubItem = 3;
			strItem = sTmp;
			lvItem.pszText = (LPTSTR)strItem;
			lvItem.mask = LVIF_TEXT;
			ctlList.SetItem( &lvItem );

			recSet->MoveNext();
		}
		recSet->Close();

		bSuccess = true;
	}
	catch(_com_error &e)
	{
	// Notify the user of errors if any
		ShowADOErrors( e, g_pTheDB->m_pConnection );
	}
	catch(...)
	{
		AfxMessageBox( _T("Some error occured in CArchiveDB::CopiesLoad().") );
	}
}

/* (2)
// Erase all rows from the List Control
//=======================================
void CLogView::EraseList()
{
	CListCtrl& ctlList = GetListCtrl();
	ctlList.DeleteAllItems();
}
*/

void CLogView::OnKeyDown(NMHDR* pNMHDR, LRESULT* pResult) 
{
	MyOnKeyDown( pNMHDR );
	*pResult = 0;
}

/* (2)
// Show Popup menu
//====================================================================
void CLogView::ShowPopupMenu( int nItemSel, CPoint curPoint )
{
	DoPopupMenu( IDR_LOG_POPUP, curPoint );
}
*/

void CLogView::OnUpdateLogClear(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
}


void CLogView::OnLogClear() 
{
	try
	{
	// Clear the table in the Database
		((_ConnectionPtr)g_pTheDB->m_pConnection)->Execute( 
										"DELETE FROM ArcLog", NULL, NULL );
	
	// Update the window
		ShowLog();
	}
	catch(_com_error &e)
	{
	// Notify the user of errors if any
		ShowADOErrors( e, g_pTheDB->m_pConnection );
	}
	catch(...)
	{
		AfxMessageBox( _T("Some error occured in CLogView::OnLogClear().") );
	}
}
