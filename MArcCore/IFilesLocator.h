// DiskArcher.
// IFilesLocator interface - Locator of new files in a given folder and its subfolders.
// (C) Marat Mirgaleev, 2002-2014.
//==============================================================================

#pragma once

/*zzz
#include "resource.h"
#include "afxtempl.h"*/
#include "CFileOnDisk.h"
/*#include "CFilesOnDisk.h"
#include "CNewFilesLocatorDlg.h"

class CLocatorFolder;
class CNewFilesLocatorFrame;
class CNewFilesLocatorView;
*/

class IFilesLocator
{
public:
/*zzz
    CFilesOnDisk m_foundFiles;       // (5)
	CNewFilesLocatorDlg*   m_pDlg;
	CNewFilesLocatorView*  m_pView;
    CNewFilesLocatorFrame* m_pFrame; // (5)
*/
// zzzz  IFilesLocator();
  virtual ~IFilesLocator() {}

  virtual bool isAborted() const = 0;
  virtual bool isSkippingSomeFiles() const = 0;
  virtual CStringList& getExcludedFolders() const = 0;
  virtual CStringList& getExcludedFileTypes() const = 0;
  virtual void showFolderInDlg( const CString& strFolder ) const = 0;
  virtual void showProgress( const CString& strFName ) const = 0;
  virtual int getDays() const = 0;
  virtual void addToList( CFileOnDisk* pFile ) = 0;

/*
    bool Init();
    void Analyze( const CString& startPath );
    bool LoadOptions();
	void AddToList( CFileOnDisk* pFile );
    bool CheckAndAddToExclFolders( const CFileOnDisk* const pFile );// (5) Added
    bool CheckAndAddToExclTypes( const CFileOnDisk* const pFile );  // (5) Added

// UI features
//------------------------------------------------------------------------------
	void EnableControls( bool bOnOff );	// (5)
		{ return ( m_pDlg->m_bSkipSomeFiles != 0 ); };	// (1)

protected:
	CLocatorFolder* m_pStartFolder;	// Locator starts with this Folder, 
			//	then this Folder will scan its subfolders recursivelly

    int  RemoveFilesOfType( const CString& sExtension );       // (5) Added

    bool AddExcludedToDB( LocatorWhatToExclude nType,
                          const CString& sName );              // (5) Added
    bool AddExcludedTypeToDB( const CString& sExtension );     // (5) Added
    bool AddExcludedFolderToDB( const CString& sFolderName );  // (5) Added
*/
};
