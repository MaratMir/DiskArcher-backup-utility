// DiskArcher.
// CFileToArc class - a file that the program shall copy to the Archive.
// (C) Marat Mirgaleev, 2001-2014.
// Modifications:
//  (1) 09.02.02. m_bSystem added.
//  (2) 18.02.02. Delete() added.
//  (4) 08.03.02. GetStatus() added.
//  (5) 28.03.02. Init() added.
//  (6) 29.10.02. Daylight saving time taken in account.
//  (7) 05.01.02. Correction of a few seconds difference when files moved from NTFS to FAT.
//  (8) 11.05.03-26.10.03. Compression.
//  (9) 22.11.2003. ResetRuntimeData() added.
// (10) 24.05.2006. getRequiredCopiesNum() added.
//==================================================================

#include "stdafx.h"
#include <memory>
#include "..\MArcLib\Misc.h"
#include "..\MArcCore\Miscelaneous.h"
#include "CFileToArc.h"
#include "CMyArchive.h"
#include "CArchiveDB.h"
#include "CFileCompressor.h"


CFileToArc::CFileToArc()
{
  Init(); // (5)
}


CFileToArc::CFileToArc(CString fullName)
  : CFileOnDisk( fullName )
{
  Init(); // (5)
}


void CFileToArc::Init()
{
  m_bSystem = false;
  m_bPaused = false;
  m_bCompressIt = false; // (8)

  ResetRuntimeData();
}


// (9) Clear some non-actual runtime information, including deletion
//	    of temporary files
//===================================================================
void CFileToArc::ResetRuntimeData()
{
  m_addCopy = false; // 2014. Was: nCommand = fcNothing;
  m_nPredictedCompressedSize = -1/*Not calculated*/;
  m_pBundle = NULL;
  m_CopyToRooms.clear();

// Delete precompressed file if any
  if( this->IsPreCompressed() )
    if( 0 == DeleteFile( this->GetCompressedFileName() ) )
      g_TheArchive.m_LogFile.AddRecord( L"Temp dir", L"Compressed " + this->m_strName, GetErrDescription( true ) );
  m_sCompressedFilename = "";
}


// (2) Delete the File from DB and its Copies too
//=================================================
bool CFileToArc::Delete()
{
  bool bSuccess = true;

  // Mark all the Copies of this File for deletion first
  std::unique_ptr<CFilesCopies> copies( g_TheArchive.m_Copies.GetFileCopies( this ) );
  POSITION pos;
  for( pos = copies->GetHeadPosition(); pos != NULL; )
  {
    CFileCopy *pCurCopy = copies->GetNext( pos );
    if( ! pCurCopy->MarkForDeletion() )
      g_TheArchive.m_LogFile.AddRecord( getFullPath(), m_strName, L"Could not delete a Copy of the File" );
  }

  bSuccess = g_TheArchive.m_pDB->FileDelete( this );
  return bSuccess;
}


// (4) Returns true if the file must be updated.
//	   The code moved from CFilesToArc::FilesGetStatus().
//=========================================================
bool CFileToArc::GetStatus()
{
  bool bMustBeUpdated = false;
  if( m_bPaused )    // The user paused the file archiving
  {
    m_nStatus = fsPaused;
  }
  else if( ! getInfo() )  // Could not find the file
    m_nStatus = fsNotFound;
  else
  {
  // Compare date-time of the file and date-time of its latest copy
    CFileCopy* pLatestCopy = g_TheArchive.m_Copies.GetLatestCopy( this );
#if _DEBUG
    if( pLatestCopy != NULL )
    {
      CString copydt = pLatestCopy->m_FileDateTime.Format();
      CString filedt = m_LastWriteTime.Format();
    }
#endif
    if( pLatestCopy == NULL ) // There are no copies of the file
    {
      m_nStatus = fsNew;
      bMustBeUpdated = true; 
    // LATER: nFilesToUpdate++;
    }
    else
    {
    // (7) Time difference code block replaced
      COleDateTimeSpan ts = m_LastWriteTime - pLatestCopy->m_FileDateTime; // (6)
      int spanSecs = (int)ts.GetTotalSeconds(); // (6)

      if( spanSecs < 5 && spanSecs > -5 )
      // (7) A few seconds difference - it could be caused by conversion
      //     from NTFS to FAT - ignore it and consider that the file is unchanged
        m_nStatus = fsUpToDate;

      else if( m_LastWriteTime > pLatestCopy->m_FileDateTime )
      {
        if( spanSecs == 3600 ) // (6) Exactly one hour
          // It must be that a Daylight Saving Time switchover date on NTFS
          //    caused the files time difference - just ignore it.
          // Should we display a message about it? Or write to the Log?
          //   - For what? The user can't do anything with it!
          m_nStatus = fsUpToDate; // (6)
        else  // (6)
        {
          m_nStatus = fsChanged;
          bMustBeUpdated = true; 
        // LATER: nFilesToUpdate++;
        } // (6)
      }

      else if( m_LastWriteTime < pLatestCopy->m_FileDateTime )
      { // (6)
      if( spanSecs == -3599 ) // (6) Exactly one hour. 
                    // I don't know were one second was lost
          // It must have been Daylight Saving Time switchover date
          //   caused the files time difference - just ignore it.
          m_nStatus = fsUpToDate;	// (6)
        else // (6)
          m_nStatus = fsOlder;
      } // (6)
    }
    // End of (7)
  }
  return bMustBeUpdated;
}


// (8)
//==============================================================================
CString CFileToArc::GetCompressedFileName() const
{
  return AddWithSlash( g_TheArchive.getTempPath(), m_sCompressedFilename );
}


// (8) Compress the file to temp directory
//==============================================================================
bool CFileToArc::PreCompress()
{
  bool bSuccess = false;

  // Create a temporary compressed file in the temp dir
  // Filenames could be the same in different folders, so we need to give 
  //  unique names for the temporary files
  wchar_t szTempName[MAX_PATH]; 
  if( ! ::GetTempFileName( g_TheArchive.getTempPath(), _T("DA_"), 0, szTempName ) )
    AfxMessageBox( _T("Couldn't get a temp filename.") );
  else
  {
    CDiskItem comprFile( szTempName );
    m_sCompressedFilename = comprFile.m_strName; // Without the path!
    // Delete this file from disk - otherwise PKZip will raise error
    bSuccess = ( DeleteFile( szTempName ) != 0 );
    if ( ! bSuccess )
      AfxMessageBox( _T("Couldn't delete the temp file.") );	
    else
    {
      CString sFileToCompress = getFullName();
      if( this->m_strName == g_TheArchive.m_pDB->MyDBFilename ) // If it's the program's database,
        sFileToCompress += '~';                                 //  it has been copied in file with '~'
                                                  // at the end, because PKZip can't compress open file

      CString sParams = L"-add=all -attr=all -nozipextension \"" + GetCompressedFileName() 
                      + L"\" \"" + sFileToCompress + L"\"";
      CWaitCursor wait;
      DWORD dwExitCode = MyShellExecute( g_TheArchive.m_pCompressor->m_strExePath,
                          sParams, g_TheArchive.getTempPath(), SW_SHOWMINIMIZED );

      if( dwExitCode != 0 )
      // Known PKZip25 error codes:
      //  1 - file being compressed is opened - sharing violation
      {
        // TODO: Log.  Error flag.  "Continue?" "View Log"
        CString s; s.Format( _T("Compressor returned Exit Code: %d"), dwExitCode );
        AfxMessageBox( s );
      }

      // Get compressed file size
      bSuccess = comprFile.getInfo();
      if( bSuccess )
        m_nPredictedCompressedSize = comprFile.getSize();
    }
  }

  return bSuccess;
}


// (10) If number of file's copies has not been set, then use the default.
//==============================================================================
unsigned int CFileToArc::getRequiredCopiesNum() const
{
  int nCopiesToHave;
  if( m_nUpToCopies > 0 && m_nUpToCopies < 100 )
    nCopiesToHave = m_nUpToCopies;
  else
    nCopiesToHave = g_TheArchive.m_nDefaultCopies;

  return nCopiesToHave;
}


//==============================================================================
unsigned int CFileToArc::CountCopies( const CRoom* const i_pRoom ) const
{
  return std::count( m_CopyToRooms.begin(), m_CopyToRooms.end(), i_pRoom );
}
