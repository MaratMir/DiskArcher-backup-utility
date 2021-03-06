// DiskArcher.
// CRoomPropDialog dialog implementation file.
// (C) Marat Mirgaleev, 2001-2003.
// Modifications:
// (1) 11.05.2003. Compression mode added.
// (2) 04.05.2006. Bug fixed: Room limit was not saved.
//==============================================================================

#include "stdafx.h"
#include "MArc2.h"
#include "CRoomPropDialog.h"

#include "MArcCore/CArchiveDB.h"
#include "MArcCore/CMyArchive.h"
#include "MArcCore/CRoom.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//==============================================================================
CRoomPropDialog::CRoomPropDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CRoomPropDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRoomPropDialog)
	m_Limit = 0;
	//}}AFX_DATA_INIT
}


//==============================================================================
void CRoomPropDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRoomPropDialog)
	DDX_Control(pDX, IDC_ROOM_WHEN_USE_COMPRESSION, m_CompressionMode);
	DDX_Text(pDX, IDC_ROOMPROP_LIMIT, m_Limit);
	DDV_MinMaxInt(pDX, m_Limit, 0, 4000000);
	//}}AFX_DATA_MAP
}


//==============================================================================
BEGIN_MESSAGE_MAP(CRoomPropDialog, CDialog)
	//{{AFX_MSG_MAP(CRoomPropDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//==============================================================================
const int firstRadio = IDC_ROOM_COMPRESSION_ALLOWED,	// (1)
		  lastRadio = IDC_ROOM_COMPRESSION_NEVER;		// (1)


// (1) Edit properties of the Room
//==============================================================================
int CRoomPropDialog::Edit(CRoom *pRoom)
{
  CRoomPropDialog dlg;
  dlg.pTheRoom = pRoom;

//-------------------------------
  int result = dlg.DoModal();
//-------------------------------

  if (result==IDOK)
  {
    if( ! g_TheArchive.m_pDB->RoomUpdate( pRoom ) )
      result = IDCANCEL;
  }
  return result;
}


// (1)
//==============================================================================
BOOL CRoomPropDialog::OnInitDialog() 
{
  CDialog::OnInitDialog();

  // I'm sure a usual int will be enough, if we show the limit in Megabytes, but let's shrink it correctly
  LARGE_INTEGER li;
  li.QuadPart = pTheRoom->m_sizeLimit >> 20;  // From Bytes to MB
  m_Limit = li.LowPart;
  int checkedRadio=0;
  switch( pTheRoom->m_nCompressionMode )
  {
  case CRoom::roomCompressionMode::rcmAllowed: 
    checkedRadio = IDC_ROOM_COMPRESSION_ALLOWED;
    break;
  case CRoom::roomCompressionMode::rcmNever:
    checkedRadio = IDC_ROOM_COMPRESSION_NEVER;
    break;
  case CRoom::roomCompressionMode::rcmAlways:
    checkedRadio = IDC_ROOM_COMPRESSION_ALWAYS;
    break;
  default:
    ASSERT(false);
    break;
  }
  CheckRadioButton( firstRadio, lastRadio, checkedRadio );
  UpdateData( FALSE );
  
  return TRUE;  // return TRUE unless you set the focus to a control
                // EXCEPTION: OCX Property Pages should return FALSE
}


// (1)
//==============================================================================
void CRoomPropDialog::OnOK() 
{
// Store changed data
  UpdateData( TRUE );
  pTheRoom->m_sizeLimit = m_Limit << 20; // From MB to Bytes
  int checkedRadio = GetCheckedRadioButton( firstRadio, lastRadio );
  switch( checkedRadio )
  {
  case IDC_ROOM_COMPRESSION_ALLOWED:
    pTheRoom->m_nCompressionMode = CRoom::roomCompressionMode::rcmAllowed;
    break;
  case IDC_ROOM_COMPRESSION_NEVER:
    pTheRoom->m_nCompressionMode = CRoom::roomCompressionMode::rcmNever;
    break;
  case IDC_ROOM_COMPRESSION_ALWAYS:
    pTheRoom->m_nCompressionMode = CRoom::roomCompressionMode::rcmAlways;
    break;
  default:
    ASSERT(false);
    break;
  }
  
  CDialog::OnOK();
}
