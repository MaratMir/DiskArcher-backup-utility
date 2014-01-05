#if !defined(AFX_CINPUTSTRINGDIALOG_H__A22F95C5_C2E1_49AF_8742_07F7ECC4E129__INCLUDED_)
#define AFX_CINPUTSTRINGDIALOG_H__A22F95C5_C2E1_49AF_8742_07F7ECC4E129__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CInputStringDialog.h : header file
//

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CInputStringDialog dialog

class CInputStringDialog : public CDialog
{
// Construction
public:
	enum dlgType { dlgTypeAdd, dlgTypeEdit };
	enum dlgType m_nMode;
	CInputStringDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInputStringDialog)
	enum { IDD = IDD_INPUT_STRING_DIALOG };
	CString	m_strEdit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputStringDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputStringDialog)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CINPUTSTRINGDIALOG_H__A22F95C5_C2E1_49AF_8742_07F7ECC4E129__INCLUDED_)
