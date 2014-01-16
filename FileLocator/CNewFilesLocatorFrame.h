// DiskArcher.
// Interface of the CNewFilesLocatorFrame class.
// (C) Marat Mirgaleev, 2002.
// Modifications:
//   (1) 25.07.2004. Locator reconstruction.
//==================================================================

#if !defined( _NewFilesLocatorFrame_H__INCLUDED_ )
#define _NewFilesLocatorFrame_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CNewFilesLocator;
class CNewFilesLocatorView;


class CNewFilesLocatorFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CNewFilesLocatorFrame)

public:
    CNewFilesLocator* m_pLocator;   // (1)

protected:
	CToolBar    m_wndToolBar;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewFilesLocatorFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	//{{AFX_MSG(CNewFilesLocatorFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(_NewFilesLocatorFrame_H__INCLUDED_)
