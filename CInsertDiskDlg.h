// DiskArcher.
// "Insert Disk" dialog.
// (C) Marat Mirgaleev, 2001-2014.
//==========================================================================

#pragma once

#include "resource.h"
#include "MArcCore/IInsertDisk.h"

class CRoom;

class InsertDisk : public IInsertDisk
{
// IInsertDisk overrides:
  virtual int askInsertDiskForCopy( const CRoom* const i_pRoom );
  virtual int askInsertDiskForExtract( const CRoom* const i_pRoom );
};


class CInsertDiskDlg : public CDialog
{
// Construction
public:
	bool m_bEnableSkip; // M
	CInsertDiskDlg(CWnd* pParent = NULL);   // standard constructor


// Dialog Data
	//{{AFX_DATA(CInsertDiskDlg)
	enum { IDD = IDD_INS_DISK_DIALOG };
	CButton	m_SkipButton;
	CString	m_InsDiskLabel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInsertDiskDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInsertDiskDlg)
	afx_msg void OnSkipDisk();
	afx_msg void OnAbort();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
