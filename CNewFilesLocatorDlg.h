// DiskArcher.
// CNewFilesLocatorDlg class interface file.
// (C) Marat Mirgaleev, 2002.
// Created in Aug 2002.
// Modifications:
//	(1) 26.10.02. m_pLocator added.
//	(2) 12.01.03. "Files..." and "Folders..." buttons added.
//				  Destructor added.
//  (4) 24.07.04. Reconstruction.
//==================================================================

#if !defined(CNewFilesLOCATORDLG_H__INCLUDED_)
#define CNewFilesLOCATORDLG_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CLocatorExcludedView.h"


class CNewFilesLocator;
class CNewFilesLocatorView;

class CNewFilesLocatorDlg : public CDialog
{
public:
	CNewFilesLocator* m_pLocator;	// (1)

	CNewFilesLocatorDlg( CNewFilesLocator* pLocator
                            /*(4) Was: CNewFilesLocatorView* pView */,
		                 CWnd* pParent = NULL );
	~CNewFilesLocatorDlg();	// (2)
	void ShowProgress( CString strFName ); // (1) Was CDiskItem* pItem );
	void CountFile();

// Dialog Data
	//{{AFX_DATA(CNewFilesLocatorDlg)
	enum { IDD = IDD_LOCATOR_DIALOG };
	CButton	m_ExclFileTypesButton;
	CButton	m_ExclFoldersButton;
	CButton	m_BrowseBtn;
	CStatic	m_CheckedFiles;
	CButton	m_CancelBtn;
	CSpinButtonCtrl	m_Spin;
	CButton	m_OkButton;
	CString	m_strCurrentFolder;
	CString	m_strCounter;
	CString	m_StartPath;
	UINT	m_nDays;
	BOOL	m_bSkipSomeFiles;
	//}}AFX_DATA

	bool m_bIsAborted;	// Flag, is "Cancel" button pressed
	bool IsAborted();	// Check "Cancel"
	CNewFilesLocatorView* m_pView;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewFilesLocatorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation

protected:
	bool m_bRunning;	// (2)
	void Finalize();	// (2)
	
	int m_nCounter;
	void EnableControls( bool bOnOff );

	virtual void OnCancel();
	// Generated message map functions
	//{{AFX_MSG(CNewFilesLocatorDlg)
	virtual void OnOK();
	afx_msg void OnAbort();
	afx_msg void OnClose();
	afx_msg void OnLctrBrowse();
	virtual BOOL OnInitDialog();
	afx_msg void OnSkipSomeFilesClick();
	afx_msg void OnExclFileTypesButton();
	afx_msg void OnExclFoldersButton();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void ShowExcluded( enum LocatorWhatToExclude );
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(CNewFilesLOCATORDLG_H__INCLUDED_)
