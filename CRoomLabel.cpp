// DiskArcher.
// Implementation of the CRoomLabel class.
// (C) Marat Mirgaleev, 2001-2002.
// Modifications:
//	(1) 20.02.2002. Erase() and GetFileName() added.
//	(2) 19.05.2002. RoomID.txt file changes.
//	(3) 16.06.02. Web page changed.
//==============================================================================

#include "stdafx.h"
#include "marc2.h"
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
		str.Format( "Archive Room #%d."
// TODO: Archive ID <date-time>.
					"\nPlease do not delete this file!"
					"\n\nWHEN YOUR COMPUTER CRASHES, so, DiskArcher and its"
					"\narchive are damaged too. In this case please:"
					"\n\n1. Reinstall DiskArcher. If you don't have DiskArcher's"
					"\nsetup.exe file, go http://diskarcher.mastak.com" // (3)
					"\nor try to find \"DiskArcher\" on Internet search sites."
					"\n\n2. Open DiskArcher Help, find \"How to restore data\" topic "
					"\nand follow the instructions.", m_pRoom->m_nRoomID );
/* (2) Was str.Format( "Archive Room #%d\nPlease do not delete this file.",
					m_pRoom->m_nRoomID ); */
// LATER: Make it Read-Only?
		lblFile.WriteString( str );
		lblFile.Close();
		bSuccess = true;
	}
	catch(...)
	{
		AfxMessageBox( "The label file could not be written." );
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
				AfxMessageBox( "The Room Label is corrupted." );
					// TO DO: What must user do then?
			else
			{
				str = str.Mid( 14/*Length of "Archive Room #"*/ );
				int nID = atoi( str );
				if( nID != m_pRoom->m_nRoomID )
				{
					CString mess;
					mess.Format( "It isn't the Room #%d.", m_pRoom->m_nRoomID );
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
			char buff[200];
			fe->GetErrorMessage( buff, 199 );
			CString mess;
			mess.Format( "The Room not found on the disk."
						 "\nPlease check that it is the Room Disk and it is inserted properly."
						 "\nIf it is a network drive than check the network connection."
						 "\n\nError Code=%d:"
						 "\n%s", fe->m_cause, buff );
//			if( fe->m_lOsError == ERROR_PATH_NOT_FOUND )
//			{	
//			}
//			else
				AfxMessageBox( mess );
			break;
		}
		catch( ... )
		{
			AfxMessageBox( "Some error occured in CRoomLabel::Check()." );
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
			CStdioFile readmeFile( 
                       m_pRoom->getFullName() + "\\ReadMe.txt",
                       CFile::modeCreate | CFile::modeWrite | CFile::typeText );
			readmeFile.WriteString( "This DiskArcher's Room was deleted from the program.\n"
								 "You can delete it if you have enough copies of your files." );
			readmeFile.Close();
			bSuccess = true;
		}
		catch( CFileException e )
		{
			AfxMessageBox( "Could not write ReadMe.txt file:\n" + e.m_cause );
		}
	return bSuccess;
}


//==============================================================================
CString CRoomLabel::GetFileName()
{
	return m_pRoom->getFullName() + "\\RoomID.txt";
}
