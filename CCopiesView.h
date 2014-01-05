// DiskArcher.
// CCopiesView.h : interface of the CCopiesView class
// (C) Marat Mirgaleev, 2001-2002.
// Modifications:
//	(1) 09.02.2002. GetSelectedCopy() added.
//	(4) 19.02.2002. Column "BundleId" replaced by "RoomID". 
//	(8) 16.01.2003. EraseList() moved into the base class.
//=================================================================

#if !defined(AFX_CopiesView_H__B23BE5F0_C975_423F_A345_9E50125FE4DC__INCLUDED_)
#define AFX_CopiesView_H__B23BE5F0_C975_423F_A345_9E50125FE4DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "CMyListView.h"

class CFileCopy; // M
class CCopiesDoc; // M


class CCopiesView : public CMyListView
{
protected: // create from serialization only
	CCopiesView();
	DECLARE_DYNCREATE(CCopiesView)

// Attributes
public:
	CImageList m_ctlImage; // M

// M. If they are not empty, show only copies of this file (with this path)
	CString m_strPath;		
	CString m_strFilename;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCopiesView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	//}}AFX_VIRTUAL

// Implementation
public:
	CFileCopy* GetSelectedCopy();	// (1)
// (8)	void EraseList();
	void ShowCopyList();
	CCopiesDoc* GetDocument();
	virtual ~CCopiesView()  {};
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void OnRightClick( NMHDR *pNotifyStruct, LRESULT *result );
	bool SetSubItems( int nItem, CFileCopy* pCopy );

	int m_nIDColumn;	// Current position of ID's column
	int m_nPathColumn;	// Current position of Path column
	int m_nNameColumn;	// Current position of Name column
	int m_nTimeColumn;	// Current position of Time column
	int m_nRoomIDColumn;	// Current position of RoomID column
		// (2) Was: int m_nBundleIDColumn;	// Current position of BundleID column

// Generated message map functions
protected:
	void ShowPopupMenu( int nItemSel, CPoint curPoint );
	//{{AFX_MSG(CCopiesView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnCopyProperties();
	afx_msg void OnCopyNew();
	afx_msg void OnKeyDown(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUpdateCopyExtract(CCmdUI* pCmdUI);
	afx_msg void OnCopyExtract();
	afx_msg void OnCopyDelete();
	afx_msg void OnUpdateCopyDelete(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCopyProperties(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in CopiesView.cpp
inline CCopiesDoc* CCopiesView::GetDocument()
   { return (CCopiesDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CopiesView_H__B23BE5F0_C975_423F_A345_9E50125FE4DC__INCLUDED_)
