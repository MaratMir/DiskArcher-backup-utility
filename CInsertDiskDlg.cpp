// DiskArcher.
// "Insert Disk" dialog.
// (C) Marat Mirgaleev, 2001-2006.
//==========================================================================

#include "stdafx.h"
#include "marc2.h"
#include "CInsertDiskDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInsertDiskDlg dialog


CInsertDiskDlg::CInsertDiskDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInsertDiskDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInsertDiskDlg)
	m_InsDiskLabel = _T("");
	//}}AFX_DATA_INIT
	m_bEnableSkip = true; // M
}


void CInsertDiskDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInsertDiskDlg)
	DDX_Control(pDX, ID_SKIP_DISK, m_SkipButton);
	DDX_Text(pDX, IDC_INS_DISK_LBL, m_InsDiskLabel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInsertDiskDlg, CDialog)
	//{{AFX_MSG_MAP(CInsertDiskDlg)
	ON_BN_CLICKED(ID_SKIP_DISK, OnSkipDisk)
	ON_BN_CLICKED(IDABORT, OnAbort)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInsertDiskDlg message handlers

void CInsertDiskDlg::OnSkipDisk() 
{
	EndDialog( ID_SKIP_DISK );
}

void CInsertDiskDlg::OnAbort() 
{
	EndDialog( IDABORT );
}


BOOL CInsertDiskDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

// TODO: Add extra initialization here
	m_SkipButton.EnableWindow( m_bEnableSkip );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
