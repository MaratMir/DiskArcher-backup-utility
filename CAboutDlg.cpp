// DiskArcher.
// About Dialog.
// (C) Marat Mirgaleev, 2001-2002.
// Moved in a separate file from MArc2.cpp on 10.05.2002.
// Modifications:
//	(1) 21.09.03. realAuthor added.
//==================================================================

#include "stdafx.h"
#include "CAboutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const char* realAuthor = "(C) Designed and developed by Marat R. Mirgaleev";	// (1)


CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	m_strLicensedTo = _T("");
	//}}AFX_DATA_INIT
}


void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Text(pDX, IDC_LICENSED_TO, m_strLicensedTo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
