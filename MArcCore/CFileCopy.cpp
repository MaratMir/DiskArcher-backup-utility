// DiskArcher.
// Implementation of the CFileCopy class.
// (C) Marat Mirgaleev, 2001-2002.
// Modifications:
//	(1) 09.02.2002. MarkForDeletion() added.
//	(2) 19.02.2002. GetRoom() added.
//	(3) 01.11.2002. theDB changed to *pTheDB.
//	(4) 26.03.2003. Extract() implemented - the code moved from CCopiesView.
//	(5) 11.05-26.10.2003. Compression.
//===========================================================================

#include "stdafx.h"
#include "CFileCopy.h"
#include "CFileToArc.h"
#include "CBundle.h"

#include "CMyArchive.h" // M
#include "CArchiveDB.h" // M
#include "IInsertDisk.h"
#include "CRoom.h"		// M
#include "Miscelaneous.h"	// (1)


//==============================================================================
bool CFileCopy::Extract( const CString strExtractToPath ) const
{
  bool bSuccess = true;
	CBundle *pBundle = g_TheArchive.m_Bundles.BundleFind( m_nBundleID );
	if( pBundle == NULL )
	{
		AfxMessageBox( _T("CFileCopy::Extract(): Can't find the Bundle.") );
		bSuccess = false;
	}


// Check is this copy placed on removable disk. 
//		If it is, ask "Please insert disk..."
	CRoom *pRoom = NULL;
	if( bSuccess )
	{
		pRoom = g_TheArchive.m_Rooms.find( pBundle->m_nRoomID );
		if( pRoom == NULL )
		{
			AfxMessageBox( _T("CFileCopy::Extract(): Can't find the Room.") );
			bSuccess = false;
		}
	}
  if( bSuccess )
  {
    ASSERT( pRoom );
    if( pRoom->m_bRemovable )
    {
      while( true )
      {
        ASSERT( g_TheArchive.getInsertDlg() );
        g_TheArchive.getInsertDlg()->askInsertDiskForExtract( pRoom );
/*				CInsertDiskDlg insDlg;
        insDlg.m_bEnableSkip = false;
        insDlg.m_InsDiskLabel = mess;
        int nRet = insDlg.DoModal();
          switch ( nRet )
        {
        case -1: 
          AfxMessageBox( 
            _T("'Insert Disk' dialog box could not be created!") );
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
          AfxMessageBox( _T("Sudden Error in 'CFileCopy::Extract'") );
          break;
        }
*/
			// Check is it Room #N
				if( bSuccess )
				{
					bool bCheckedOk = pRoom->CheckLabel();
					if( bCheckedOk )
						break;
					else
						continue;	// Try next Disk
				}
				else
					break;
			}
		}
		else // Room is not Removable
		// (5) Check is the Room availlable
			if( ! pRoom->IsAvailable() )
			{
				CString msg;
				msg.Format( _T("The Room %d is unavailable. Path:\n%s")
							      _T("\nIf it is a network drive then")
							      _T("\ncheck that you have logged in."),
							      pRoom->m_nRoomID, pRoom->getFullName() );
				AfxMessageBox( msg );
				bSuccess = false;
			}
			else if( !pRoom->CheckLabel() )
			{
				AfxMessageBox( _T("Room is damaged!") );	// TO DO
				bSuccess = false;
			}
	}
	if( bSuccess )
	{
		ASSERT( pBundle );
	// Check is the file exist
		CString strExtractToFile = strExtractToPath + m_strFilename;
		CFileOnDisk theFile( strExtractToFile );
		if( theFile.checkExistence() )
			if( AfxMessageBox( 
					strExtractToFile + ":\nFile already exists. Overwrite?",
					MB_YESNO ) == IDYES )
				bSuccess = ( 0 != DeleteFile( strExtractToFile ));
					// (5) Was just: bSuccess = true;
					//   We must delete the destination file if it exist, 
					//	 because otherwise Compressor might raise an error
			else
				bSuccess = false;
		if( bSuccess )
		{
			CWaitCursor wait;	// (7)

			bSuccess = pBundle->extractFile( m_strFilename, strExtractToPath,
											                 m_strFilename );
							// It is virtual! pBundle can be a ZIP file
		/* (5) Was:
			BOOL bOk = CopyFile( pBundle->GetFullName(), 
								 strExtractToFile, 
								 FALSE/*Overwrite existing*-/ );
			bSuccess = ( bOk != 0 );
			if( ! bSuccess )
			{	CString msg = "Can't extract the file from \n"
			                + pBundle->GetFullName()
							+ ":\n" + GetErrDescription();
				AfxMessageBox( msg );
			}*/
		}

		if( bSuccess )
		{
			CString mess;
			mess.Format( _T("File has been successfully extracted to\n%s"), 
						 strExtractToFile );
			AfxMessageBox( mess, MB_ICONINFORMATION );
		}
	}
	return bSuccess;
}


// (1) Mark the Copy for deletion.
// It will be deleted during the next Archiving process.
//=======================================================
bool CFileCopy::MarkForDeletion()
{
  m_bDeleteIt = true;
  return g_TheArchive.m_pDB->CopyUpdate( this );
}


// (2)
CRoom* CFileCopy::GetRoom() const
{
	CRoom *pFoundRoom = NULL;

	CBundle *pBundle = g_TheArchive.m_Bundles.BundleFind( m_nBundleID );
	ASSERT( pBundle );
	pFoundRoom = g_TheArchive.m_Rooms.find( pBundle->m_nRoomID );

	return pFoundRoom;
}
