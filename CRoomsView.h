// DiskArcher.
// Interface of the CRoomsView class.
// (C) Marat Mirgaleev, 2001-2003.
// Modifications:
//	(4) 16.01.2003. EraseList() moved into the base class.
//=====================================================================

#if !defined(AFX_RoomsView_H__B23BE5F0_C975_423F_A345_9E50125FE4DC__INCLUDED_)
#define AFX_RoomsView_H__B23BE5F0_C975_423F_A345_9E50125FE4DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "CMyListView.h"

class CRoom;	 // M
class CRoomsDoc; // M

class CRoomsView : public CMyListView
{
protected: // create from serialization only
	CRoomsView();
	DECLARE_DYNCREATE(CRoomsView)

// Attributes
public:
	CRoomsDoc* GetDocument();
	CImageList m_ctlImage; // M

// Operations
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

// Implementation
public:
	CRoom* GetSelectedRoom( int& nSelectedItem );
// (4)	void EraseList();
	void ShowRoomList();
	virtual ~CRoomsView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void OnRightClick( NMHDR *pNotifyStruct, LRESULT *result );
	int  m_nIDColumn;

// Generated message map functions
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

#endif // !defined(AFX_RoomsView_H__B23BE5F0_C975_423F_A345_9E50125FE4DC__INCLUDED_)
