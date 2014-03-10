// DiskArcher.
// CRoomsView class - shows Rooms.
// (C) Marat Mirgaleev, 2001-2014.
// Modifications:
//  (4) 16.01.2003. EraseList() moved into the base class.
//=====================================================================

#pragma once

#include "CMyListView.h"

class CRoom;
class CRoomsDoc;


class CRoomsView : public CMyListView
{
protected: // create from serialization only
  CRoomsView();
  DECLARE_DYNCREATE(CRoomsView)

public:
  CRoomsDoc* GetDocument();
  CImageList m_ctlImage;

public:

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CRoomsView)
  public:
  virtual void OnDraw(CDC* pDC);  // overridden to draw this view
  virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
  protected:
  virtual void OnInitialUpdate(); // called first time after construct
  //}}AFX_VIRTUAL

public:
  CRoom* GetSelectedRoom( int& nSelectedItem );
// (4) void EraseList();
  void ShowRoomList();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

protected:
  void OnRightClick( NMHDR *pNotifyStruct, LRESULT *result );
  int  m_nIDColumn;

protected:
  void ShowPopupMenu( int nItemSel, CPoint curPoint );
  bool SetSubItems( int nItem, CRoom* pRoom );
  //{{AFX_MSG(CRoomsView)
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnRoomProperties();
  afx_msg void OnRoomNew();
  afx_msg void OnUpdateRoomProperties(CCmdUI* pCmdUI);
  afx_msg void OnKeyDown(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnRoomDelete();
  afx_msg void OnUpdateRoomDelete(CCmdUI* pCmdUI);
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in RoomsView.cpp
inline CRoomsDoc* CRoomsView::GetDocument()
   { return (CRoomsDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
