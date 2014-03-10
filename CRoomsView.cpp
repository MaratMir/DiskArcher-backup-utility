// DiskArcher.
// CRoomsView class - shows Rooms.
// (C) Marat Mirgaleev, 2001-2014.
// Modifications:
//  (1) 09.02.2002. GetSelectedRoom() added.
//  (2) 26.05.2002. SetSubItems() correction.
//  (3) 01.11.2002. theDB changed to *pTheDB.
//  (4) 16.01.2003. EraseList() moved into the base class.
//  (5) 11.05.2003. Properties dialog changes.
//  (6) 28.10.2003. Compression mode.
//=====================================================================

#include "stdafx.h"
#include "MArc2.h"

#include "CRoomsDoc.h"
#include "CRoomsView.h"

#include "MArcCore/CRoom.h"
#include "MArcCore/CMyArchive.h"
#include "MArcCore/CArchiveDB.h"
#include "MArcCore/CFileToArc.h"
#include "CRoomPropDialog.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CRoomsView, CListView)

BEGIN_MESSAGE_MAP(CRoomsView, CListView)
  //{{AFX_MSG_MAP(CRoomsView)
  ON_WM_CREATE()
  ON_COMMAND(ID_ROOM_PROPERTIES, OnRoomProperties)
  ON_COMMAND(ID_ROOM_NEW, OnRoomNew)
  ON_UPDATE_COMMAND_UI(ID_ROOM_PROPERTIES, OnUpdateRoomProperties)
  ON_NOTIFY_REFLECT(LVN_KEYDOWN, OnKeyDown)
  ON_COMMAND(ID_ROOM_DELETE, OnRoomDelete)
  ON_UPDATE_COMMAND_UI(ID_ROOM_DELETE, OnUpdateRoomDelete)
  //}}AFX_MSG_MAP
  ON_NOTIFY_REFLECT(NM_RCLICK, OnRightClick)
END_MESSAGE_MAP()


enum { imgUnavailable, imgOk, imgRemovable };


CRoomsView::CRoomsView()
{
  m_nIDColumn = 0; // TODO: Change when columns dragged and dropped
}


BOOL CRoomsView::PreCreateWindow(CREATESTRUCT& cs)
{
  // TODO: Modify the Window class or styles here by modifying
  //  the CREATESTRUCT cs

  return CListView::PreCreateWindow(cs);
}


void CRoomsView::OnDraw(CDC* pDC)
{
  UNREFERENCED_PARAMETER( pDC );
  CRoomsDoc* pDoc = GetDocument();
  ASSERT_VALID(pDoc);
  // TODO: add draw code for native data here
}


void CRoomsView::OnInitialUpdate()
{
  CListView::OnInitialUpdate();

  // TODO: You may populate your ListView with items by directly accessing
  //  its list control through a call to GetListCtrl().
  ShowRoomList();
}


#ifdef _DEBUG
void CRoomsView::AssertValid() const
{
  CListView::AssertValid();
}

void CRoomsView::Dump(CDumpContext& dc) const
{
  CListView::Dump(dc);
}

CRoomsDoc* CRoomsView::GetDocument() // non-debug version is inline
{
  ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRoomsDoc)));
  return (CRoomsDoc*)m_pDocument;
}
#endif //_DEBUG


void CRoomsView::OnRightClick(NMHDR *pNotifyStruct, LRESULT *result)
{
  MyOnRightClick( pNotifyStruct );
  *result = 0;
}


int CRoomsView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
  lpCreateStruct->style |= LVS_REPORT | /*LVS_NOSORTHEADER |*/ LVS_SINGLESEL;
// lpCreateStruct->dwExStyle |= ; // It doesn't work this way, it works via SetExtendedStyle
  if (CListView::OnCreate(lpCreateStruct) == -1)
    return -1;
  
// Give the document a pointer to this view
  GetDocument()->m_pListView = this;
  
  CListCtrl& ctlList = (CListCtrl&) GetListCtrl();
  ctlList.SetExtendedStyle( LVS_EX_HEADERDRAGDROP | LVS_EX_FULLROWSELECT | 
                            LVS_EX_GRIDLINES | LVS_EX_SUBITEMIMAGES );

// Create the Image List
  BOOL crRes = m_ctlImage.Create( IDB_ROOMS_IMAGELIST, 16, 0, RGB(255,0,255));
  m_ctlImage.SetBkColor(GetSysColor(COLOR_WINDOW));
  if( !crRes )
    AfxMessageBox( _T("CRoomsView::OnCreate(): Image List Create Error.") );
  ctlList.SetImageList( &m_ctlImage, LVSIL_SMALL );
  
  int res;
  VERIFY( (res=ctlList.InsertColumn( 0, _T("RoomID"), LVCFMT_RIGHT, 60 )) >= 0 );
  res=ctlList.InsertColumn( 1, _T("Path"),		LVCFMT_LEFT,  300 );
  res=ctlList.InsertColumn( 2, _T("Removable"),	LVCFMT_CENTER, 75 );
  res=ctlList.InsertColumn( 3, _T("Limit, MB"),	LVCFMT_RIGHT,  70 );
  res=ctlList.InsertColumn( 4, _T("Free, KB"),	LVCFMT_RIGHT,  70 );
  res=ctlList.InsertColumn( 5, _T("Compression"), LVCFMT_CENTER, 85 );	// (6)

  return 0;
}


void CRoomsView::ShowRoomList()
{
  CListCtrl& ctlList = GetListCtrl();

// Generate Column Headers
  EraseList();

  for( POSITION pos=g_TheArchive.m_Rooms.GetHeadPosition(); pos != NULL; )
  {
    CRoom *pCurRoom = g_TheArchive.m_Rooms.GetNext(pos);
/*
    int nImageIndex = imgOk;
    if( pCurRoom->m_bRemovable )
      nImageIndex = imgRemovable;
    if( pCurRoom->m_nSpaceFree == -1 )
      nImageIndex = imgUnavailable;
*/
    CString sTmp;
    LV_ITEM lvItem;
    lvItem.mask = LVIF_TEXT;
    lvItem.iItem = ctlList.GetItemCount();	// Insert at the end
    lvItem.iSubItem = 0;
    sTmp.Format( _T("%d"), pCurRoom->m_nRoomID );
    LPCTSTR strItem = sTmp;
    lvItem.pszText = (LPTSTR)strItem;
    lvItem.mask |= LVIF_IMAGE;
    lvItem.iImage = -1;

/* Somehow, this didn't work. I did SetItemState instead
  // State M
    lvItem.mask |= LVIF_STATE;
    lvItem.stateMask = LVIS_STATEIMAGEMASK;	// Bits 12-15
    lvItem.state = INDEXTOSTATEIMAGEMASK(nState);
*/
    int newItem = ctlList.InsertItem(&lvItem);
    DBG_UNREFERENCED_LOCAL_VARIABLE(newItem);

    BOOL res = ctlList.SetItemState( newItem, INDEXTOSTATEIMAGEMASK( 11/*TO DO nState*/),
      LVIS_STATEIMAGEMASK );//INDEXTOSTATEIMAGEMASK(nState) );
    DBG_UNREFERENCED_LOCAL_VARIABLE(res);

    SetSubItems( newItem, pCurRoom );
  }
}

/* (4)
void CRoomsView::EraseList()
{
  CListCtrl& ctlList = GetListCtrl();
  ctlList.DeleteAllItems();
//	while( ctlList.DeleteColumn(0) );
}
*/

void CRoomsView::OnRoomProperties() 
{
// Get selected Room
  int nSelItem;
  CRoom *pSelRoom = GetSelectedRoom( nSelItem );
  if( pSelRoom )
  {
/* (1) Was
  CListCtrl& ctlList = GetListCtrl();
  int nItem = -1;
  POSITION pos = ctlList.GetFirstSelectedItemPosition();
  CRoom *pSelRoom = NULL;
  if (pos != NULL)	//	There can not be multiselection!  X while (pos)
  {
    nItem = ctlList.GetNextSelectedItem(pos);
    CString txt = ctlList.GetItemText( nItem, m_nIDColumn );
    int nID = atoi( txt );
    pSelRoom = theArchive.m_Rooms.RoomFind( nID ); */
  
  // Show Dialog
    int result = CRoomPropDialog::Edit( pSelRoom );	/* (5) Was:
    CRoomPropDialog dlg;
    dlg.m_Limit = pSelRoom->m_nSizeLimit >> 10;	// From Bytes to KB
    int result = dlg.DoModal();
    if (result==IDOK)
    {
    // Store changed data
      pSelRoom->m_nSizeLimit = dlg.m_Limit << 10;	// From KB to Bytes

      if( ! pTheDB->/*(3) theDB.* /RoomUpdate( pSelRoom ) )
        result = IDCANCEL;
    }*/
    if (result==IDOK)
    // Update List Control
      SetSubItems( nSelItem, pSelRoom );
  }
}


bool CRoomsView::SetSubItems( int nItem, CRoom* pRoom )
{
  bool bOk = true;
  CString sTmp;
  LPCTSTR strItem;
  CListCtrl& ctlList = GetListCtrl();
  LV_ITEM lvItem;

  lvItem.mask = LVIF_TEXT;
  lvItem.iItem = nItem;
  lvItem.iSubItem = 1;	// LATER: Moving Columns?
  sTmp = pRoom->getFullName();
  strItem = sTmp;
  lvItem.pszText = (LPTSTR)(strItem);
  if( ! ctlList.SetItem(&lvItem) )
    bOk = false;

  lvItem.mask = LVIF_TEXT /* (2) | LVIF_IMAGE */;
  if( pRoom->m_bRemovable )
  {
    lvItem.mask |= LVIF_IMAGE;	// (2)
    lvItem.iImage = imgRemovable;
  }
  lvItem.iItem = nItem;
  lvItem.iSubItem = 2;
  strItem = pRoom->m_bRemovable ? _T("Yes") : _T("No");
  lvItem.pszText = (LPTSTR)(strItem);
  if( ! ctlList.SetItem(&lvItem) )
    bOk = false;

  lvItem.mask = LVIF_TEXT;
  lvItem.iItem = nItem;
  lvItem.iSubItem = 3;
  sTmp.Format( _T("%d"), pRoom->m_sizeLimit >> 20 /* To Megabytes */ );
  strItem = sTmp;
  lvItem.pszText = (LPTSTR)(strItem);
  if( ! ctlList.SetItem(&lvItem) )
    bOk = false;

  lvItem.mask = LVIF_TEXT /* (2) | LVIF_IMAGE */;
  lvItem.iItem = nItem;
  lvItem.iSubItem = 4;
  if( pRoom->m_nDiskSpaceFree == -1 )
  {
    lvItem.mask |= LVIF_IMAGE;	// (2)
    lvItem.iImage = imgUnavailable;
    sTmp = "Unavailable";
  }
  else
  {
    // (2) lvItem.iImage = -1;	// Nothing
    sTmp.Format( _T("%d"), pRoom->m_nDiskSpaceFree >> 10 );
  }
  strItem = sTmp;
  lvItem.pszText = (LPTSTR)(strItem);
  if( ! ctlList.SetItem(&lvItem) )
    bOk = false;

// (6) "Compression mode" column
  lvItem.mask = LVIF_TEXT;
  lvItem.iItem = nItem;
  lvItem.iSubItem = 5;
  switch( pRoom->m_nCompressionMode )
  {
  case CRoom::roomCompressionMode::rcmAllowed:
    sTmp = "Allowed"; break;
  case CRoom::roomCompressionMode::rcmNever:
    sTmp = "Never";   break;
  case CRoom::roomCompressionMode::rcmAlways:
    sTmp = "Always";  break;
  default:
    ASSERT(false);
    sTmp = "UNEXPECTED";  break;
  }
  strItem = sTmp;
  lvItem.pszText = (LPTSTR)(strItem);
  if( ! ctlList.SetItem(&lvItem) )
    bOk = false;
// End of (6)

  return bOk;
}


// Create a new Archive Room
void CRoomsView::OnRoomNew() 
{
  if( g_TheArchive.m_Rooms.RoomCreate() )
    ShowRoomList();
}


void CRoomsView::OnUpdateRoomProperties(CCmdUI* pCmdUI) 
{
// Enable it if anything is selected
  CListCtrl& ctlList = GetListCtrl();
  POSITION pos = ctlList.GetFirstSelectedItemPosition();
  pCmdUI->Enable( pos != NULL );
}


void CRoomsView::OnKeyDown(NMHDR* pNMHDR, LRESULT* pResult) 
{
  MyOnKeyDown( pNMHDR );
  *pResult = 0;
}


// Show Popup menu:
//	If any Room is selected - IDR_ROOM_POPUP,
//  If nothing is selected  - IDR_ROOMS_POPUP.
void CRoomsView::ShowPopupMenu( int nItemSel, CPoint curPoint )
{
  if( nItemSel == -1 )
  // Nothing is selected
    DoPopupMenu( IDR_ROOMS_POPUP, curPoint );
  else
    DoPopupMenu( IDR_ROOM_POPUP, curPoint );
}


// Delete Room from the program.
// Don't delete files from disk - user must delete them manually.
void CRoomsView::OnRoomDelete() 
{

// TO DO: Glass Cursor

  if( AfxMessageBox( _T("Are you sure to delete this Room\n")
                     _T("and all Copies stored within this Room?"), MB_YESNO ) == IDYES )
  {
    int nSelItem;
    CRoom* pSelRoom = GetSelectedRoom( nSelItem );

    g_TheArchive.m_Rooms.Delete( pSelRoom );

  // Update display
    ShowRoomList();
    ((CMainFrame*)AfxGetMainWnd())->CopiesWindowUpdate();
  }
}


// (1) Get selected Room
CRoom* CRoomsView::GetSelectedRoom( int& nSelectedItem )
{
  CListCtrl& ctlList = GetListCtrl();
  nSelectedItem = -1;
  POSITION pos = ctlList.GetFirstSelectedItemPosition();
  CRoom *pSelRoom = NULL;
  if (pos != NULL)	//	There can not be multiselection!
  {
    nSelectedItem = ctlList.GetNextSelectedItem( pos );
    CString txt = ctlList.GetItemText( nSelectedItem, m_nIDColumn );
    int nID = _ttoi(txt); // atoi( txt );
    pSelRoom = g_TheArchive.m_Rooms.RoomFind( nID );
  }
  return pSelRoom;
}
  

void CRoomsView::OnUpdateRoomDelete(CCmdUI* pCmdUI) 
{
// Enable it if anything is selected
  CListCtrl& ctlList = GetListCtrl();
  POSITION pos = ctlList.GetFirstSelectedItemPosition();
  pCmdUI->Enable( pos != NULL );
}
