// DiskArcher.
// Interface of the CLocatorExcludedView class.
// (C) Marat Mirgaleev, 2003.
// Created 12.01.2003.
// Modifications:
//=================================================================

#if !defined(AFX_LocatorExcludedView_H__B23BE5F0_C975_423F_A345_9E50125FE4DC__INCLUDED_)
#define AFX_LocatorExcludedView_H__B23BE5F0_C975_423F_A345_9E50125FE4DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CMyListView.h"

class CLocatorExcludedDoc;
class CNewFilesLocator;


class CLocatorExcludedView : public CMyListView
{
protected: // create from serialization only
	DECLARE_DYNCREATE(CLocatorExcludedView)

// Attributes
public:
	CLocatorExcludedDoc* GetDocument();
	CNewFilesLocator* GetLocator();
	CStringList* GetSourceList();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLocatorExcludedView)
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	//}}AFX_VIRTUAL

// Implementation
public:
	void ShowList( enum LocatorWhatToExclude nShowType );
	int AddRowToListCtrl( CString fname );
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void OnRightClick( NMHDR *pNotifyStruct, LRESULT *result );

// Generated message map functions
protected:
	//{{AFX_MSG(CLocatorExcludedView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKeyDown(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	afx_msg void OnLocatorExclAdd();
	afx_msg void OnUpdateLocatorExclAdd(CCmdUI* pCmdUI);
	afx_msg void OnLocatorExclRemove();
	afx_msg void OnUpdateLocatorExclRemove(CCmdUI* pCmdUI);
	afx_msg void OnLocatorExclAccept();
	afx_msg void OnUpdateLocatorExclAccept(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLocatorExclRestoreDefaults(CCmdUI* pCmdUI);
	afx_msg void OnLocatorExclRestoreDefaults();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LocatorExcludedView_H__B23BE5F0_C975_423F_A345_9E50125FE4DC__INCLUDED_)
