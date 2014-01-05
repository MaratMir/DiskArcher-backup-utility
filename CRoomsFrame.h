// CRoomsFrame.h : interface of the CRoomsFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RoomsFrame_H__D9C31E2B_F16B_4568_8045_BCE591223ACF__INCLUDED_)
#define AFX_RoomsFrame_H__D9C31E2B_F16B_4568_8045_BCE591223ACF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CRoomsView;


class CRoomsFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CRoomsFrame)
public:
	CRoomsFrame();

// Attributes
protected:
	CToolBar    m_wndToolBar; // M
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRoomsFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	void EraseList( void );		// M ??
	void UpdateList( void );	// M ??
	virtual ~CRoomsFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CRoomsFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	//}}AFX_MSG
	afx_msg void OnUpdateViewStyles(CCmdUI* pCmdUI);
	afx_msg void OnViewStyle(UINT nCommandID);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RoomsFrame_H__D9C31E2B_F16B_4568_8045_BCE591223ACF__INCLUDED_)
