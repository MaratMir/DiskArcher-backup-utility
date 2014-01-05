// DiskArcher.
// CMyListView class implementation file.
// (C) Marat Mirgaleev, 2001-2003.
// Modifications:
//	(1) 11.04.2002. Context menu corrections.
//	(2) 02.09.2002. IsAnythingSelected() added.
//	(3) 16.01.2003. EraseList() added.
//					Typical behaviour of ShowPopupMenu implemented;
//						m_nMenuID and SetPopupMenu() added.
//==========================================================================


#include "stdafx.h"
#include "MArc2.h"
#include "CMyListView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CMyListView, CListView)


BEGIN_MESSAGE_MAP(CMyListView, CListView)
	//{{AFX_MSG_MAP(CMyListView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CMyListView diagnostics

#ifdef _DEBUG
void CMyListView::AssertValid() const
{
	CListView::AssertValid();
}

void CMyListView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG


// CMyListView message handlers

void CMyListView::MyOnKeyDown( NMHDR* pNMHDR )
{
	LV_KEYDOWN* pLVKeyDown = (LV_KEYDOWN*)pNMHDR;
	CListCtrl& ctlList = GetListCtrl();
	POSITION pos = ctlList.GetFirstSelectedItemPosition();
		//	while (pos)	 - LATER: multiselection
	int nItemSel = ctlList.GetNextSelectedItem(pos);
	if( pLVKeyDown->wVKey == VK_APPS)
	{
		RECT curRect;
	// (1) Was: ASSERT( ctlList.GetItemRect( nItemSel, &curRect, LVIR_BOUNDS ) );
		CPoint curPoint;													// (1)
		if( ctlList.GetItemRect( nItemSel, &curRect, LVIR_BOUNDS ) )		// (1)
	// (1) Was: CPoint curPoint( curRect.left + 5, curRect.bottom );
		{	curPoint.x = curRect.left + 5; curPoint.y = curRect.bottom;	}	// (1)
		else	// No selection - just show the menu anywhere				// (1)
		{	curPoint.x = 25; curPoint.y = 25;	}							// (1)

		ClientToScreen(&curPoint);
		ShowPopupMenu( nItemSel, curPoint );
	}
}


// If you want different behaviour, override this method.
// nItemSel may be useful for derived classes.
void CMyListView::ShowPopupMenu( int nItemSel, CPoint curPoint )
{
	DoPopupMenu( m_nPopupMenuID, curPoint );
}


void CMyListView::MyOnRightClick( NMHDR *pNotifyStruct )
{
	CListCtrl& ctlList = GetListCtrl();
	UINT nFlags;
	CPoint curPoint, curPointClient;
	GetCursorPos(&curPoint);
	curPointClient = curPoint;
	ScreenToClient(&curPointClient);
	int nItemSel = ctlList.HitTest(curPointClient, &nFlags);
	ShowPopupMenu( nItemSel, curPoint );
}


void CMyListView::DoPopupMenu( UINT nMenuID, CPoint curPoint )
{
	CMenu popMenu;
	VERIFY/*(1)*/( popMenu.LoadMenu(nMenuID) );	// How should I destroy it?

/* LATER: Maybe, in .NET? :-)
   It was an attempt to show bitmaps within a popup menu. The problem is that the image
	is inverted when it is selected...
	Another one problem is associating menus (nMenuID) with bitmap lists.
	CBitmap m_CheckBitmap, m_UnCheckBitmap;
   ASSERT(m_CheckBitmap.LoadBitmap(IDB_TREEIMAGELIST));
   ASSERT(m_UnCheckBitmap.LoadBitmap(IDB_COPIES_IMAGELIST));
   // Associate bitmaps with the "Test" menu item. 
   ASSERT(popMenu.SetMenuItemBitmaps( ID_ROOM_PROPERTIES, MF_BYCOMMAND, 
      &m_CheckBitmap, &m_UnCheckBitmap));
*/	
/*	CPoint posMouse;
	GetCursorPos(&posMouse);
*/
	popMenu.GetSubMenu(0)->TrackPopupMenu( TPM_RIGHTBUTTON,
		curPoint.x, curPoint.y, AfxGetMainWnd() ); // this );
//		posMouse.x, posMouse.y, pWnd );
}


// (2)
bool CMyListView::IsAnythingSelected()
{
	CListCtrl& ctlList = GetListCtrl();
	POSITION pos = ctlList.GetFirstSelectedItemPosition();
	return ( pos != NULL );
}


// (3) Erase all rows from the List Control
//==========================================
void CMyListView::EraseList()
{
	CListCtrl& ctlList = GetListCtrl();
	ctlList.DeleteAllItems();
}
