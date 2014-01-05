// DiskArcher.
// Options dialog.
// (C) Marat Mirgaleev, 2001-2003.
// Created in summer 2003.
//================================================================

#include "stdafx.h"
#include "marc2.h"
#include "COptionsDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionsDialog dialog


COptionsDialog::COptionsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(COptionsDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(COptionsDialog)
	m_nDefaulNumberOfCopies = 0;
	m_sCompressionUtility = _T("");
	m_sCompressorPath = _T("");
	//}}AFX_DATA_INIT
}


void COptionsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionsDialog)
	DDX_Control(pDX, IDC_BROWSE_BUTTON, m_BrowseButton);
	DDX_Text(pDX, IDC_DEFAULT_NUMBER_OF_COPIES_EDIT, m_nDefaulNumberOfCopies);
	DDV_MinMaxInt(pDX, m_nDefaulNumberOfCopies, 3, 10);
	DDX_CBString(pDX, IDC_COMPRESSION_UTILITY_COMBO, m_sCompressionUtility);
	DDX_Text(pDX, IDC_COMPRESSOR_PATH_EDIT, m_sCompressorPath);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionsDialog, CDialog)
	//{{AFX_MSG_MAP(COptionsDialog)
	ON_BN_CLICKED(IDC_BROWSE_BUTTON, OnBrowseButton)
	ON_CBN_SELCHANGE(IDC_COMPRESSION_UTILITY_COMBO, OnSelChangeCompressionUtilityCombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionsDialog message handlers


void COptionsDialog::OnBrowseButton() 
{
	CFileDialog dlg( TRUE/*Open, not Save*/ );
//	dlg.m_ofn.lpstrFile = "Today.txt";
/*	dlg.m_ofn.lpstrFilter = "Pkzip 2.50 Command Line (PKZip25.exe)\0PKZip25.exe\0\0";
	dlg.m_ofn.nFilterIndex = 1;
	dlg.m_ofn.lpstrTitle = "Path to compression utility";
	dlg.m_ofn.Flags |= OFN_ENABLESIZING;
	if( dlg.DoModal() == IDOK )
	{
		m_sCompressorPath = dlg.m_ofn.lpstrFile;
		UpdateData( FALSE );
	}
*/
}


void COptionsDialog::OnSelChangeCompressionUtilityCombo() 
{
	UpdateData();
	if( m_sCompressionUtility == "(none)" )
	{
		m_sCompressorPath = "";
		m_BrowseButton.EnableWindow( FALSE );
		UpdateData( FALSE );
	}
	else
		m_BrowseButton.EnableWindow();
}
