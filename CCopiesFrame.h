// DiskArcher.
// CCopiesFrame.h : interface of the CCopiesFrame class
// (C) Marat Mirgaleev, 2001-2002.
//==================================================================

#if !defined(AFX_CopiesFrame_H__D9C31E2B_F16B_4568_8045_BCE591223ACF__INCLUDED_)
#define AFX_CopiesFrame_H__D9C31E2B_F16B_4568_8045_BCE591223ACF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CCopiesView;


class CCopiesFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CCopiesFrame)
public:
	CCopiesFrame();

// Attributes
protected:
	CToolBar    m_wndToolBar; // M
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCopiesFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
//	void EraseList( void );		// M ??
	void UpdateList( void );	// M
	virtual ~CCopiesFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CCopiesFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnCopyDelete();
	afx_msg void OnUpdateCopyDelete(CCmdUI* pCmdUI);
	afx_msg void OnCopyProperties();
	afx_msg void OnUpdateCopyProperties(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CopiesFrame_H__D9C31E2B_F16B_4568_8045_BCE591223ACF__INCLUDED_)
