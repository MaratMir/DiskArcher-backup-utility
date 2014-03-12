// DiskArcher.
// CLocatorFolder class implementation file.
// (C) Marat Mirgaleev, 2002.
// Created 22.10.2002 - extracted from CNewFilesLocator.
// Modifications:
//  (1) 15.01.03. Ignore case.
//==========================================================================

#include "stdafx.h"
#include "CFileToArc.h"
#include "CLocatorFolder.h"
#include "CMyArchive.h"
#include "IFilesLocator.h"


//------------------------------------------------------------------------------
CLocatorFolder::CLocatorFolder( const CString& strFullName, IFilesLocator*  pLocator )
              : CFolder( strFullName )
{
  m_pLocator = pLocator;
}


//------------------------------------------------------------------------------
CLocatorFolder::CLocatorFolder( const CFolder& folder, IFilesLocator* pLocator )
{
  m_strComputer = folder.m_strComputer;
  m_strDrive    = folder.m_strDrive;
  m_strDir      = folder.m_strDir;
  m_strName     = folder.m_strName;

  m_pLocator = pLocator;
}


//------------------------------------------------------------------------------
void CLocatorFolder::Analyze()
{

// First of all - check subfolders
  for( POSITION pos=m_Items.GetHeadPosition(); pos != NULL; )
  {
    if( m_pLocator->isAborted() ) // Interrupted by user
      break;

    CDiskItem *curItem = m_Items.GetNext(pos);
    if( curItem->getType() == CDiskItem::DI_FOLDER )
    {
      // Skipping useless folders if it is required
      if( m_pLocator->isSkippingSomeFiles() )
      {
        CString sName = curItem->m_strName;
        sName.MakeUpper();
        if( ( m_pLocator->getExcludedFolders() ).Find( sName ) != NULL )
          continue; // We don't need this folder, go to next
      }
      m_pLocator->showFolderInDlg( curItem->getFullName() );
      CFolder *curFolder = (CFolder*)curItem;
      CLocatorFolder locator( *curFolder, m_pLocator );
      locator.GetItems();
      locator.Analyze(); // Recursively
    }
  }


// Then check files in this folder
  for( POSITION pos=m_Items.GetHeadPosition(); pos != NULL; )
  {
    if( m_pLocator->isAborted() ) // Interrupted by user
      break;

    CDiskItem *curItem = m_Items.GetNext(pos);
    if( curItem->getType() == CDiskItem::DI_FILE )
    {
    // Skipping useless files if it is required
      if( m_pLocator->isSkippingSomeFiles() )
      {
        wchar_t drive[_MAX_DRIVE];
        wchar_t dir[_MAX_DIR];
        wchar_t fname[_MAX_FNAME];
        wchar_t ext[_MAX_EXT];
        _wsplitpath_s( curItem->m_strName, drive, dir, fname, ext );
        
      // (1)
        CString sType = ext+1; /* Skip the first character - a dot '.'*/
        sType.MakeUpper();
        if( ( m_pLocator->getExcludedFileTypes() ).Find( sType ) != NULL )
          continue; // We don't need file of this type, go to next
        /* End of (1). Was:
        if( ( m_pLocator->excludedFileTypes ).Find( 
              ext+1/* Skip the first character - a dot '.' * / 
                              ) != NULL )
          continue; // We don't need file of this type, go to next
        */
      }

      m_pLocator->showProgress( curItem->getFullPath() );

      COleDateTime curT = COleDateTime::GetCurrentTime();
      COleDateTimeSpan timeDiff = curT - curItem->m_LastWriteTime;
      if( timeDiff.GetTotalHours() <= m_pLocator->getDays() * 24 )
      {
        // If this file is not in Archive yet
        CFileToArc* pFound = 
          g_TheArchive.m_FilesToArc.FileFind( curItem->getFullName() );
        if( ! pFound )
        // Add this file to the list and ListCtrl
        {
          /*res=*/ curItem->getInfo();
          CFileOnDisk* pFile = new CFileOnDisk( *((CFileOnDisk*)curItem) );
          m_pLocator->addToList( pFile );
        }
      }
    }
  }
}
