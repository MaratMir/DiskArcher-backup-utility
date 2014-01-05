// DiskArcher.
// CLogFrame.h : interface of the CLogFrame class
// (C) Marat Mirgaleev, 2001-2002.
//==================================================================

#if !defined(AFX_LogFrame_H__INCLUDED_)
#define AFX_LogFrame_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CCopiesView;


class CLogFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CLogFrame)
public:
	CLogFrame();

// Attributes
protected:
//	CToolBar    m_wndToolBar; // M
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
//	void EraseList( void );		// M
	void UpdateList( void );	// M
	virtual ~CLogFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CLogFrame)
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

#endif // !defined(AFX_LogFrame_H__INCLUDED_)
