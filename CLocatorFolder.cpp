// DiskArcher.
// CLocatorFolder class implementation file.
// (C) Marat Mirgaleev, 2002.
// Created 22.10.2002 - extracted from CNewFilesLocator.
// Modifications:
//	(1) 15.01.03. Ignore case.
//==========================================================================

#include "stdafx.h"
#include "CFileToArc.h"
#include "CLocatorFolder.h"
#include "CMyArchive.h"
#include "CNewFilesLocator.h"
#include "MArc2.h"


//------------------------------------------------------------------------------
CLocatorFolder::CLocatorFolder( const CString& strFullName,
	CNewFilesLocator*  pLocator )
// (1) Was:	CNewFilesLocatorView* pView, CNewFilesLocatorDlg* pLocatorDlg )
	: CFolder( strFullName )
{
	m_pLocator = pLocator;
// (1)	m_pView = pView;
// (1)	m_pDlg = pLocatorDlg;
}


//------------------------------------------------------------------------------
CLocatorFolder::CLocatorFolder( const CFolder& folder,
	CNewFilesLocator*  pLocator )
// (1) CLocatorFolderView* pView, CLocatorFolderDlg* pLocatorDlg )
{
	m_strComputer = folder.m_strComputer;
	m_strDrive	  = folder.m_strDrive;
	m_strDir      = folder.m_strDir;
	m_strName     = folder.m_strName;

// (1)	m_pView = pView;
// (1)	m_pDlg = pLocatorDlg;
	m_pLocator = pLocator;
}


//------------------------------------------------------------------------------
void CLocatorFolder::Analyze()
{

// First of all - check subfolders
	for( POSITION pos=m_Items.GetHeadPosition(); pos != NULL; )
	{
		if( m_pLocator->IsAborted() )	// Interrupted by user
			break;

		CDiskItem *curItem = m_Items.GetNext(pos);
		if( curItem->getType() == CDiskItem::DI_FOLDER )
		{
		// Skipping useless folders if it is required
			if( m_pLocator->IsSkippingSomeFiles() )
			{	// (1)
				CString sName = curItem->m_strName;
				sName.MakeUpper();
				if( ( m_pLocator->m_excludedFolders ).Find( sName ) != NULL )
					continue;	// We don't need this folder, go to next
			}	
			/* End of (1). Was:
				if( ( m_pLocator->excludedFolders ).Find( 
											curItem->m_strName ) != NULL )
					continue;	// We don't need this folder, go to next
			*/			
			m_pLocator->ShowFolderInDlg( curItem->getFullName() );
			CFolder *curFolder = (CFolder*)curItem;
			CLocatorFolder locator( *curFolder, m_pLocator );
			locator.GetItems();
			locator.Analyze(); // Recursively
		}
	}


// Then check files in this folder
	for( pos=m_Items.GetHeadPosition(); pos != NULL; )
	{
		if( m_pLocator->IsAborted() )	// Interrupted by user
			break;

		CDiskItem *curItem = m_Items.GetNext(pos);
		if( curItem->getType() == CDiskItem::DI_FILE )
		{
		// Skipping useless files if it is required
			if( m_pLocator->IsSkippingSomeFiles() )
			{
				char/*wchar_t*/ drive[_MAX_DRIVE];
				char dir[_MAX_DIR];
				char fname[_MAX_FNAME];
				char ext[_MAX_EXT];
				_splitpath( curItem->m_strName, drive, dir, fname, ext );
				
			// (1)
				CString sType = ext+1;	/* Skip the first character - a dot '.'*/
				sType.MakeUpper();
				if( ( m_pLocator->m_excludedFileTypes ).Find( sType ) != NULL )
					continue;	// We don't need file of this type, go to next
			/* End of (1). Was:
				if( ( m_pLocator->excludedFileTypes ).Find( 
							ext+1/* Skip the first character - a dot '.' * / 
														  ) != NULL )
					continue;	// We don't need file of this type, go to next
			*/
			}
			
			m_pLocator->ShowProgress( curItem->getFullPath() );

			COleDateTime curT = COleDateTime::GetCurrentTime();
			COleDateTimeSpan timeDiff = curT - curItem->m_LastWriteTime;
			if( timeDiff.GetTotalHours() <= m_pLocator->GetDays() * 24 )
			{
			// If this file is not in Archive yet
				CFileToArc* pFound = 
					g_TheArchive.m_FilesToArc.FileFind( curItem->getFullName() );
				if( ! pFound )
				// Add this file to the list and ListCtrl
        {
          /*res=*/ curItem->getInfo();
          CFileOnDisk* pFile = new CFileOnDisk( *((CFileOnDisk*)curItem) );
					m_pLocator->AddToList( pFile );
        }
			}
		}
	}
}
