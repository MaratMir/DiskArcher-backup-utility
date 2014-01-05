// DiskArcher.
// Dialog of FileToArc properties.
// (C) Marat Mirgaleev, 2003.
//==========================================================================

#include "stdafx.h"
#include "marc2.h"
#include "CFileToArcPropertiesDialog.h"
#include "CMyArchive.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileToArcPropertiesDialog dialog


CFileToArcPropertiesDialog::CFileToArcPropertiesDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CFileToArcPropertiesDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFileToArcPropertiesDialog)
	m_bCompressIt = FALSE;
	m_sFilename = _T("");
	m_nUpToCopies = 0;
	//}}AFX_DATA_INIT
}


void CFileToArcPropertiesDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileToArcPropertiesDialog)
	DDX_Check(pDX, IDC_COMPRESS, m_bCompressIt);
	DDX_Text(pDX, IDC_EDIT_FILENAME, m_sFilename);
	DDX_Text(pDX, IDC_EDIT_UPTOCOPIES, m_nUpToCopies);
	DDV_MinMaxUInt(pDX, m_nUpToCopies, 1, 99);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFileToArcPropertiesDialog, CDialog)
	//{{AFX_MSG_MAP(CFileToArcPropertiesDialog)
	ON_BN_CLICKED(IDC_COMPRESS, On_CompressIt_Changed)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileToArcPropertiesDialog message handlers


void CFileToArcPropertiesDialog::On_CompressIt_Changed() 
{
	UpdateData();
	if( m_bCompressIt )
		if( ! g_TheArchive.isCompressorDefined() )
			AfxMessageBox( "You nave not defined the compression utility yet.\n"
						         "Go to \"File\" - \"Options\" menu item" );
}
