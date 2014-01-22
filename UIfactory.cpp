// DiskArcher.
// A place to create dialogs etc.
// (C) Marat Mirgaleev, 2001-2014.
// Started as a separate file 22.01.2014
//==============================================================================

#include "stdafx.h"
#include "UIfactory.h"

#include "MArcLib/MyException.h"
#include "MArcCore/CMyArchive.h"

#include "CProgressDialog.h"
#include "CInsertDiskDlg.h"


//--------------------------------------------------------------------------------------
void UIfactory::init( CMyArchive* i_archive )
{
  m_archive = i_archive;

  m_archive->setInsertDlg( new InsertDisk() );
    // m_archive is responsible for deletion of this object
}


// Progress Dialog Init
//--------------------------------------------------------------------------------------
OpResult UIfactory::initProgressDialog()
{
  OpResult nResult = OPR_SUCCESSFUL;
  CProgressDialog* pProgressDlg = NULL;
  try
  {
    pProgressDlg = new CProgressDialog();
      // It will be deleted in its DestroyWindow().

    if ( ! pProgressDlg->Create( IDD_PROGRESS_DIALOG ) )
      nResult = OPR_FATAL_STOP;
  }
  catch(...)
  {
    nResult = OPR_FATAL_STOP;
  }
  if( nResult == OPR_FATAL_STOP )
    throw new CMyException( L"Error creating copying process dialog." );

  if( nResult <= OPR_WARNINGS )
    pProgressDlg->resetAndShow();

  //...........................................
  m_archive->setProgressDlg( pProgressDlg );
  //...........................................

  return nResult;
}
