// DiskArcher.
// CFilesToArcView.h : interface of the CFilesToArcView class
// (C) Marat Mirgaleev, 2001-2002.
// Modifications:
//	(1) 09.02.2002. GetSelectedFile() added.
//	(3) 06.03.2002. "Go" and "Pause" buttons added.
//					SetSubItems() added.
//	(9) 16.01.2003. EraseList() moved into the base class.
//=====================================================================

#if !defined(AFX_FilesToArcVIEW_H__B23BE5F0_C975_423F_A345_9E50125FE4DC__INCLUDED_)
#define AFX_FilesToArcVIEW_H__B23BE5F0_C975_423F_A345_9E50125FE4DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "CMyListView.h"

class CFilesToArcDoc;
class CFileToArc;


class CFilesToArcView : public CMyListView
{
protected: // create from serialization only
	CFilesToArcView();
	DECLARE_DYNCREATE(CFilesToArcView)

// Attributes
public:
	CFilesToArcDoc* GetDocument();
	CImageList m_ctlImage; // M

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFilesToArcView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	//}}AFX_VIRTUAL

// Implementation
public:
	void UpdateList();
// (9)	void EraseList();
	void ShowFileList( CString showPath="" );
	void SetSubItems( int nItem, CFileToArc* pFile );	// (3)
	int GetSelectedItem();			// (3)
	CFileToArc* GetSelectedFile();	// (1)
	virtual ~CFilesToArcView() {};
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void OnRightClick( NMHDR *pNotifyStruct, LRESULT *result );

	int m_nIDColumn;		// Current position of ID's column
	int m_nSizeColumn;		// Current position of Size column
	int m_nModifiedColumn;	// Current position of Modified column
	int m_nStatusColumn;	// (3) Current position of Status column

// Generated message map functions
protected:
	void ShowPopupMenu( int nItemSel, CPoint curPoint );
	//{{AFX_MSG(CFilesToArcView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnFileToArcShowCopies();
	afx_msg void OnUpdateShowCopies(CCmdUI* pCmdUI);
	afx_msg void OnKeyDown(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUpdateUpdateFiles(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFiles();
	afx_msg void OnFileToArcDelete();
	afx_msg void OnUpdateFileToArcDelete(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileToArcGo(CCmdUI* pCmdUI);
	afx_msg void OnFileToArcGo();
	afx_msg void OnUpdateFileToArcPause(CCmdUI* pCmdUI);
	afx_msg void OnFileToArcPause();
	afx_msg void OnUpdateFileToArcProperties(CCmdUI* pCmdUI);
	afx_msg void OnFileToArcProperties();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in FilesToArcView.cpp
inline CFilesToArcDoc* CFilesToArcView::GetDocument()
   { return (CFilesToArcDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FilesToArcVIEW_H__B23BE5F0_C975_423F_A345_9E50125FE4DC__INCLUDED_)
