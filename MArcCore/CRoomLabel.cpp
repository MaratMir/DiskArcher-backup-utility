// DiskArcher.
// Implementation of the CRoomLabel class.
// (C) Marat Mirgaleev, 2001-2002.
// Modifications:
//	(1) 20.02.2002. Erase() and GetFileName() added.
//	(2) 19.05.2002. RoomID.txt file changes.
//	(3) 16.06.02. Web page changed.
//==============================================================================

#include "stdafx.h"
#include "CRoomLabel.h"
#include "CRoom.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//==============================================================================
bool CRoomLabel::Write()
{
	bool bSuccess=false;
	try
	{
		CStdioFile lblFile( 
			GetFileName(),
				// (1) Was m_pRoom->GetFullName() + "\\RoomID.txt",
			CFile::modeCreate | CFile::modeWrite | CFile::typeText );
		CString str;
		str.Format( _T("Archive Room #%d.")
// TODO: Archive ID <date-time>.
					_T("\nPlease do not delete this file!")
					_T("\n\nWHEN YOUR COMPUTER CRASHES, so, DiskArcher and its")
					_T("\narchive are damaged too. In this case please:")
					_T("\n\n1. Reinstall DiskArcher. If you don't have DiskArcher's")
					_T("\nsetup.exe file, go http://diskarcher.mastak.com") // (3)
					_T("\nor try to find \"DiskArcher\" on Internet search sites.")
					_T("\n\n2. Open DiskArcher Help, find \"How to restore data\" topic ")
					_T("\nand follow the instructions."), m_pRoom->m_nRoomID );
/* (2) Was str.Format( "Archive Room #%d\nPlease do not delete this file.",
					m_pRoom->m_nRoomID ); */
// LATER: Make it Read-Only?
		lblFile.WriteString( str );
		lblFile.Close();
		bSuccess = true;
	}
	catch(...)
	{
		AfxMessageBox( _T("The label file could not be written.") );
	}
	return bSuccess;
}


//==============================================================================
bool CRoomLabel::Check()
{
	bool bSuccess=false;

	while( true )
		try
		{
			CStdioFile lblFile( 
				GetFileName(),
					// (1) Was m_pRoom->GetFullName() + "\\RoomID.txt",
				CFile::modeRead | CFile::typeText );
			CString str;
			lblFile.ReadString( str );
			lblFile.Close();
		
			if( str.Left( 14 ) != "Archive Room #" )
				AfxMessageBox( _T("The Room Label is corrupted.") );
					// TO DO: What must user do then?
			else
			{
				str = str.Mid( 14/*Length of "Archive Room #"*/ );
				int nID = _ttoi(str); // atoi( str );
				if( nID != m_pRoom->m_nRoomID )
				{
					CString mess;
					mess.Format( _T("It isn't the Room #%d."), m_pRoom->m_nRoomID );
					if( m_pRoom->m_bRemovable )
					{
						mess += "\nPlease insert the proper disk then press 'Retry'."
								"\nOr press 'Cancel' to skip the disk";
					}
					if( AfxMessageBox( mess, MB_RETRYCANCEL | MB_ICONEXCLAMATION ) == IDRETRY )
						continue;
					else
						break;	// Stop Archiving
				}
				else
				{
// TODO: Check Archive ID <date-time>.
					bSuccess = true;
					break;	// End the cycle
				}
			}
		}
		catch( CFileException *fe )
		{
			wchar_t buff[200];
			fe->GetErrorMessage( buff, 199 );
			CString mess;
			mess.Format( _T("The Room not found on the disk.")
						 _T("\nPlease check that it is the Room Disk and it is inserted properly.")
						 _T("\nIf it is a network drive than check the network connection.")
						 _T("\n\nError Code=%d:")
						 _T("\n%s"), fe->m_cause, buff );
//			if( fe->m_lOsError == ERROR_PATH_NOT_FOUND )
//			{	
//			}
//			else
				AfxMessageBox( mess );
			break;
		}
		catch( ... )
		{
			AfxMessageBox( _T("Some error occured in CRoomLabel::Check().") );
		}

	return bSuccess;
}


//==============================================================================
bool CRoomLabel::Erase()
{
// Delete file from disk
  bool bSuccess = ( DeleteFile( GetFileName() ) != 0 );

// Write ReadMe.txt file
  if( bSuccess )
    try
    {
      CStdioFile readmeFile( m_pRoom->getFullName() + L"\\ReadMe.txt",
                             CFile::modeCreate | CFile::modeWrite | CFile::typeText );
      readmeFile.WriteString( L"This DiskArcher's Room was deleted from the program.\n"
                              L"You can delete it if you have enough copies of your files." );
      readmeFile.Close();
      bSuccess = true;
    }
    catch( CFileException e )
    {
      AfxMessageBox( L"Could not write ReadMe.txt file:\n" + e.m_cause );
    }
  return bSuccess;
}


//==============================================================================
CString CRoomLabel::GetFileName()
{
  return m_pRoom->getFullName() + L"\\RoomID.txt";
}
