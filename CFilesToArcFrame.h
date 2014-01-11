// DiskArcher.
// Interface of the CFilesToArcFrame class.
// (C) Marat Mirgaleev, 2001-2014.
// Modifications:
//	(2) 05.03.2002. DoDropFiles() added.
//	(3) 06.03.2002. "Go" and "Pause" buttons added.
//======================================================================

#pragma once

#include "CProgressBar.h"

class CFilesToArcView;
class CLeftView;

class CFilesToArcFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CFilesToArcFrame)
public:
	CFilesToArcFrame();

// Attributes
protected:
	CSplitterWnd m_wndSplitter;
	CToolBar    m_wndToolBar; // M
//	CToolBar    m_wndToolBar2; // M
//	bool bShowTree; // M

public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFilesToArcFrame)
	public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	CString GetTreeSelPath();
	void EraseList( void );
	void EraseTree( void );		// M
	void UpdateList( void );
	void UpdateTree( void );	// M
	virtual ~CFilesToArcFrame();
	CLeftView* GetLeftPane();	// M
	void DoDropFiles( HDROP hDropInfo );	// (2)
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CFilesToArcView* GetRightPane();
// Generated message map functions
protected:
	//{{AFX_MSG(CFilesToArcFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnClose();
	afx_msg void OnFileOpen();
	afx_msg void OnUpdateUpdateFiles(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFiles();
	//}}AFX_MSG
	afx_msg void OnUpdateViewStyles(CCmdUI* pCmdUI);
	afx_msg void OnViewStyle(UINT nCommandID);
	afx_msg void OnUpdateFileCount(CCmdUI *pCmdUI); // M
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
