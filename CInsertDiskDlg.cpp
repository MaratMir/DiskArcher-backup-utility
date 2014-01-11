// DiskArcher.
// "Insert Disk" dialog.
// (C) Marat Mirgaleev, 2001-2014.
//==========================================================================

#include "stdafx.h"
//zzz#include "marc2.h"
#include "CInsertDiskDlg.h"
#include "MArcCore/CRoom.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CInsertDiskDlg::CInsertDiskDlg(CWnd* pParent /*=NULL*/)
  : CDialog(CInsertDiskDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(CInsertDiskDlg)
  m_InsDiskLabel = L"";
  //}}AFX_DATA_INIT

  m_bEnableSkip = true;
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

// Add extra initialization here
  m_SkipButton.EnableWindow( m_bEnableSkip );
  
  return TRUE;  // return TRUE unless you set the focus to a control
                // EXCEPTION: OCX Property Pages should return FALSE
}


int CInsertDiskDlg::askInsertDiskForCopy( const CRoom* const i_pRoom )
{
  CString mess;
  mess.Format( L"Please insert the disk labeled as\n\"Archive Room #%d\""
               L"\ninto drive %s, then press \"OK\"",
               i_pRoom->m_nRoomID, i_pRoom->m_strDrive );
  m_InsDiskLabel = mess;

  int nRet = DoModal();

  AfxGetMainWnd()->UpdateWindow();
  int nYesNo;
  OpResult nResult = OPR_SUCCESSFUL;
  switch ( nRet )
  {
  case -1: 
    AfxMessageBox( L"'Insert Disk' dialog box could not be created!" );
    nResult = OPR_FATAL_STOP;
    break;

  case IDABORT:
  case IDCANCEL:  // CANCEL - for pressing a cross in the window's right top corner
    nYesNo = AfxMessageBox( L"Abort the archiving process?", MB_YESNO );
    if( nYesNo == IDYES )
      nResult = OPR_USER_STOP;
    break;

  case IDOK:
    // Ok, continue archiving
    break;

  case ID_SKIP_DISK: // Skip this Room, proceed with next Room
    nResult = max( OPR_WARNINGS, nResult );
    break;

  default:
    AfxMessageBox( L"Unexpected error in 'DoCopying'" );
    nResult = OPR_FATAL_STOP;
    break;
  }

  return nRet;
}


int CInsertDiskDlg::askInsertDiskForExtract( const CRoom* const i_pRoom )
{
  bool bSuccess = true;
  CInsertDiskDlg insDlg;
  insDlg.m_bEnableSkip = false;

  CString mess;
  mess.Format( L"Please insert the disk labeled as"
               L"\n\"Archive Room #%d\""
               L"\ninto drive %s, then press \"OK\"",
               i_pRoom->m_nRoomID, i_pRoom->m_strDrive );
  insDlg.m_InsDiskLabel = mess;

  int nRet = insDlg.DoModal();
  switch ( nRet )
  {
  case -1: 
    AfxMessageBox( L"'Insert Disk' dialog box could not be created!" );
    bSuccess = false;
    break;
  case IDABORT:
  case IDCANCEL:
    bSuccess = false;
    break;
  case IDOK:
    // Ok, continue extracting
    break;
  default:
    AfxMessageBox( L"Unexpected error in 'CFileCopy::Extract'" );
    break;
  }

  return nRet;
}