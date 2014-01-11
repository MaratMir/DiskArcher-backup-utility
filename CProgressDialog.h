// DiskArcher.
// CProgressDialog class interface file.
// (C) Marat Mirgaleev, 2001-2002.
// Modifications:
//	(3) 13.02.2002. Some functions added.
//==================================================================

#pragma once

#include "resource.h"
#include "MArcCore/IProgressCtrl.h"

class CProgressDialog : public CDialog, public IProgressCtrl
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
  bool m_bIsAborted; // Flag, is "Stop" button pressed

  virtual void setPos( int newPos );
  virtual void setMessage( CString strMessage );
  virtual void advance( int nAmount );
  virtual void setMaxRange( int nMax );
  virtual void resetAndShow();
  virtual bool isAborted(); // Check "Stop"
  virtual void analysisDone();
  virtual void finished( bool i_stopWorking, OpResult i_result );


protected:
	virtual void OnCancel();

	// Generated message map functions
	//{{AFX_MSG(CProgressDialog)
	virtual void OnOK();
	afx_msg void OnClose();
	afx_msg void OnAbort();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
// afx_msg void OnBnClickedOk();
		// 2014-01-05: Removing. It came somehow during migration to VS 2012.

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
