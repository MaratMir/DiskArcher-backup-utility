// DiskArcher.
// Interface of the CLocatorExcludedView class.
// (C) Marat Mirgaleev, 2003.
// Created 12.01.2003.
// Modifications:
//=================================================================

#pragma once

#include "..\CMyListView.h"

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
