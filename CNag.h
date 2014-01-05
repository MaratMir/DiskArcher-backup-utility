// DiskArcher.
// Nag dialog.
// (C) Marat Mirgaleev, 2001-2002.
// Created 06.05.2002.
// Modifications:
//==================================================================

#if !defined(AFX_CNAG_H__353C2043_6019_11D6_A60A_000000000000__INCLUDED_)
#define AFX_CNAG_H__353C2043_6019_11D6_A60A_000000000000__INCLUDED_

#include "resource.h"


class CNag : public CDialog
{
// Construction
public:
	CNag(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNag)
	enum { IDD = IDD_NAG };
	CStatic	m_DaysCounter;
	CString	m_RegText;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNag)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	LONG m_nCounterTop;
	UINT m_nTimer;
	CFont m_CounterFont;

	// Generated message map functions
	//{{AFX_MSG(CNag)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnRegister();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CNAG_H__353C2043_6019_11D6_A60A_000000000000__INCLUDED_)
