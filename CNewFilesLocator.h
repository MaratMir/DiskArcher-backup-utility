// DiskArcher.
// CNewFilesLocator class interface file.
// Locator of new files in a given folder and its subfolders.
// (C) Marat Mirgaleev, 2002.
// Created 25.07.2002.
// Modifications:
//	(1) 02.11.2002. CLocatorFolder class extracted from this class.
//	(3) 15.01.2003. One copy of excludedFolders and one copy 
//				      of excludedFileTypes for all instances of the class.
//  (4) 14.06.2004. ShowCurrentFolder() renamed into ShowFolderInDlg().
//  (5) 24.07.2004. Totally reconstructed.
//                  const added everywhere.
//==============================================================================

#if !defined(CNewFilesLocator_h)
#define CNewFilesLocator_h

#include "resource.h"
#include "afxtempl.h"
#include "MArcCore\IFilesLocator.h"
#include "CFileOnDisk.h"
#include "MArcCore\CFilesOnDisk.h"
#include "CNewFilesLocatorDlg.h"

class CLocatorFolder;
class CNewFilesLocatorFrame;
class CNewFilesLocatorView;


class CNewFilesLocator : public IFilesLocator
{
public:

    CFilesOnDisk m_foundFiles;       // (5)
	CNewFilesLocatorDlg*   m_pDlg;
	CNewFilesLocatorView*  m_pView;
    CNewFilesLocatorFrame* m_pFrame; // (5)

    CNewFilesLocator();
    /* (5) Was:   CNewFilesLocator( const CString& strFullName, 
					                CNewFilesLocatorView* pView,
					                CNewFilesLocatorDlg* pLocatorDlg );*/
    ~CNewFilesLocator();
	/*(5) static*/ bool Init();
    void Analyze( const CString& startPath/*(5)Added*/ );
	/*(5) static*/ bool LoadOptions();	// (1)
    bool CheckAndAddToExclFolders( const CFileOnDisk* const pFile );// (5) Added
    bool CheckAndAddToExclTypes( const CFileOnDisk* const pFile );  // (5) Added

// UI features
//------------------------------------------------------------------------------
	void EnableControls( bool bOnOff );	// (5)

  // Overrides
  virtual bool isAborted() const;
  virtual bool isSkippingSomeFiles() const    { return ( m_pDlg->m_bSkipSomeFiles != 0 ); };
  virtual CStringList& getExcludedFolders() const     { return m_excludedFolders; }
  virtual CStringList& getExcludedFileTypes() const   { return m_excludedFileTypes; }
  virtual void showFolderInDlg( const CString& strFolder ) const;
  virtual void showProgress( const CString& strFName ) const  { m_pDlg->ShowProgress( strFName ); };
  virtual int getDays() const { return m_pDlg->m_nDays; };
  virtual void addToList( CFileOnDisk* pFile );


protected:

	CLocatorFolder* m_pStartFolder;	// Locator starts with this Folder, 
			//	then this Folder will scan its subfolders recursivelly

  static CStringList m_excludedFolders;
  static CStringList m_excludedFileTypes;

  int  RemoveFilesOfType( const CString& sExtension );       // (5) Added

  bool AddExcludedToDB( LocatorWhatToExclude nType, const CString& sName );              // (5) Added
  bool AddExcludedTypeToDB( const CString& sExtension );     // (5) Added
  bool AddExcludedFolderToDB( const CString& sFolderName );  // (5) Added
};

#endif /* CNewFilesLocator_h */
