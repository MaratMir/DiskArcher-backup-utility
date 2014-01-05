// CInputStringDialog.cpp : implementation file
//

#include "stdafx.h"
#include "marc2.h"
#include "CInputStringDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputStringDialog dialog


CInputStringDialog::CInputStringDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CInputStringDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputStringDialog)
	m_strEdit = _T("");
	//}}AFX_DATA_INIT
}


void CInputStringDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputStringDialog)
	DDX_Text(pDX, IDC_EDIT, m_strEdit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputStringDialog, CDialog)
	//{{AFX_MSG_MAP(CInputStringDialog)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputStringDialog message handlers

int CInputStringDialog::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	if( m_nMode == dlgTypeAdd )
	{
		SetWindowText( _T("Add item") );
		m_strEdit = "";
//		UpdateData( FALSE );
	}	
	return 0;
}
