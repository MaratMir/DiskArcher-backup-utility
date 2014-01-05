// DiskArcher.
// CProgressDialog class interface file.
// (C) Marat Mirgaleev, 2001-2002.
// Modifications:
//	(3) 13.02.2002. Some functions added.
//==================================================================

#if !defined(AFX_CPROGRESSDIALOG_H__B735BBB3_AB8A_11D5_A44A_000000000000__INCLUDED_)
#define AFX_CPROGRESSDIALOG_H__B735BBB3_AB8A_11D5_A44A_000000000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CProgressDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CProgressDialog dialog

class CProgressDialog : public CDialog
{
// Construction
public:
	CProgressDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProgressDialog)
	enum { IDD = IDD_PROGRESS_DIALOG };
	CStatic	m_SuccessLabel;
	CButton	m_StopButton;
	CButton	m_OkButton;
	CStatic	m_AnalysingBmp;
	CStatic	m_CopyingBmp;
	CStatic	m_CopyingLabel;
	CProgressCtrl	m_Progress;
	CString	m_CurrentFilename;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProgressDialog)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetPos( int newPos );
	void SetMessage( CString strMessage );
	void Advance( int nAmount );
	void SetMaxRange( int nMax );
	void ResetAndShow();
	bool m_bIsAborted;	// Flag, is "Stop" button pressed
	bool IsAborted();	// Check "Stop"

protected:
	virtual void OnCancel();

	// Generated message map functions
	//{{AFX_MSG(CProgressDialog)
	virtual void OnOK();
	afx_msg void OnClose();
	afx_msg void OnAbort();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CPROGRESSDIALOG_H__B735BBB3_AB8A_11D5_A44A_000000000000__INCLUDED_)
