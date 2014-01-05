// DiskArcher.
// Dialog of FileToArc properties.
// (C) Marat Mirgaleev, 2003.
//==============================================================================

#if !defined(AFX_CFILETOARCPROPERTIESDIALOG_H__867452BF_EBFC_47FF_9314_AF965DF05642__INCLUDED_)
#define AFX_CFILETOARCPROPERTIESDIALOG_H__867452BF_EBFC_47FF_9314_AF965DF05642__INCLUDED_

#include "resource.h"


class CFileToArcPropertiesDialog : public CDialog
{
// Construction
public:
	CFileToArcPropertiesDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFileToArcPropertiesDialog)
	enum { IDD = IDD_FILETOARC_PROPERTIES_DIALOG };
	BOOL	m_bCompressIt;
	CString	m_sFilename;
	UINT	m_nUpToCopies;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileToArcPropertiesDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFileToArcPropertiesDialog)
	afx_msg void On_CompressIt_Changed();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CFILETOARCPROPERTIESDIALOG_H__867452BF_EBFC_47FF_9314_AF965DF05642__INCLUDED_)
