// DiskArcher.
// Interface of the CLocatorExcludedFrame class.
// (C) Marat Mirgaleev, 2003.
// Created 12.01.2003.
//==================================================================

#if !defined(AFX_LocatorExcludedFrame_H__D9C31E2B_F16B_4568_8045_BCE591223ACF__INCLUDED_)
#define AFX_LocatorExcludedFrame_H__D9C31E2B_F16B_4568_8045_BCE591223ACF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CLocatorExcludedView;


class CLocatorExcludedFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CLocatorExcludedFrame)
public:

// Attributes
protected:
	CToolBar    m_wndToolBar; // M
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLocatorExcludedFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
//	void UpdateList( void );	// M
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CLocatorExcludedFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LocatorExcludedFrame_H__D9C31E2B_F16B_4568_8045_BCE591223ACF__INCLUDED_)
