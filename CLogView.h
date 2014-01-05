// DiskArcher.
// CLogView.h : interface of the CLogView class
// (C) Marat Mirgaleev, 2001-2002.
//	(2) 16.01.2003. EraseList() and ShowPopupMenu() moved into the base class.
//============================================================================

#if !defined(AFX_LogView_H__INCLUDED_)
#define AFX_LogView_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "CMyListView.h"

class CLogDoc;

class CLogView : public CMyListView
{
protected: // create from serialization only
	CLogView();
	DECLARE_DYNCREATE(CLogView)

// Attributes
public:
	CLogDoc* GetDocument();
	CImageList m_ctlImage; // M

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogView)
	public:
//	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	//}}AFX_VIRTUAL

// Implementation
public:
// (2)	void EraseList();
	void ShowLog();
	virtual ~CLogView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void OnRightClick( NMHDR *pNotifyStruct, LRESULT *result );

//	int m_nIDColumn;		// Current position of ID's column
//	int m_nSizeColumn;		// Current position of Size column
//	int m_nModifiedColumn;	// Current position of Modified column

// Generated message map functions
protected:
// (2)	void ShowPopupMenu( int nItemSel, CPoint curPoint );
	//{{AFX_MSG(CLogView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKeyDown(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUpdateLogClear(CCmdUI* pCmdUI);
	afx_msg void OnLogClear();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in CLogView.cpp
inline CLogDoc* CLogView::GetDocument()
   { return (CLogDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LogView_H__INCLUDED_)
