// DiskArcher.
// Options dialog.
// (C) Marat Mirgaleev, 2001-2003.
// Created in summer 2003.
//================================================================

#if !defined(AFX_COPTIONSDIALOG_H__5A582301_7706_4DF2_BBAB_70F9449E0115__INCLUDED_)
#define AFX_COPTIONSDIALOG_H__5A582301_7706_4DF2_BBAB_70F9449E0115__INCLUDED_

#include "resource.h"


class COptionsDialog : public CDialog
{
// Construction
public:
	COptionsDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COptionsDialog)
	enum { IDD = IDD_OPTIONS_DIALOG };
	CButton	m_BrowseButton;
	int		m_nDefaulNumberOfCopies;
	CString	m_sCompressionUtility;
	CString	m_sCompressorPath;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionsDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COptionsDialog)
	afx_msg void OnBrowseButton();
	afx_msg void OnSelChangeCompressionUtilityCombo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COPTIONSDIALOG_H__5A582301_7706_4DF2_BBAB_70F9449E0115__INCLUDED_)
