// DiskArcher.
// Registration Form.
// (C) Marat Mirgaleev, 2001-2002.
// Created 09.05.2002.
// Modifications:
//   (1) 24.05.2006. Copy protection changed.
//==============================================================================

#include "stdafx.h"
#include "marc2.h"
#include "CMyArchive.h"
#include "CRegisterDlg.h"
#include "CArchiveDB.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//==============================================================================
CRegisterDlg::CRegisterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRegisterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRegisterDlg)
	m_strCode = _T("");
	m_strName = _T("");
	//}}AFX_DATA_INIT
}


//==============================================================================
void CRegisterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRegisterDlg)
	DDX_Text(pDX, IDC_CODE_EDIT, m_strCode);
	DDX_Text(pDX, IDC_NAME_EDIT, m_strName);
	//}}AFX_DATA_MAP
}


//==============================================================================
BEGIN_MESSAGE_MAP(CRegisterDlg, CDialog)
	//{{AFX_MSG_MAP(CRegisterDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//==============================================================================
void CRegisterDlg::OnOK() 
{
	UpdateData( TRUE );
	bool bOk = false;

// Check conformity	of user's name and the registration code
	bOk = theApp.CheckRegDataConformity( m_strName, m_strCode );
	if( bOk )
	{
		if( theApp.WriteProfileString( "Settings", "User",    m_strName ) 
		 &&	theApp.WriteProfileString( "Settings", "RegCode", m_strCode )
     && g_pTheDB->optionSave( "Archive", "R"/*I think it will be more difficult //(16)
                to crack the program if this string is short*/, m_strCode ) ) //(16)
			AfxMessageBox(  "Congratulations!"
							        "\nYou have successfully registered"
							        "\nyour copy of DiskArcher!" );
		else
			AfxMessageBox(	"Unknown error during registering."
							"\nPlease contact the program manufacturer." );
		CDialog::OnOK();
	}
	else
		AfxMessageBox( "Given user name and code are not correct."
			             "\nPlease check them very carefully and try again." );
}
