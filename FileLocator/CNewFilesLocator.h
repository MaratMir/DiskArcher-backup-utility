// DiskArcher.
// CNewFilesLocator class interface file.
// Locator of new files in a given folder and its subfolders.
// (C) Marat Mirgaleev, 2002.
// Created 25.07.2002.
// Modifications:
//  (1) 02.11.2002. CLocatorFolder class extracted from this class.
//  (3) 15.01.2003. One copy of excludedFolders and one copy of excludedFileTypes for all instances of the class.
//  (4) 14.06.2004. ShowCurrentFolder() renamed into ShowFolderInDlg().
//  (5) 24.07.2004. Totally reconstructed.
//                  const added everywhere.
//==============================================================================

#if !defined(CNewFilesLocator_h)
#define CNewFilesLocator_h

#include "../resource.h"
#include "afxtempl.h"
#include "..\MArcLib\CFileOnDisk.h"
#include "..\MArcLib\CFilesOnDisk.h"
#include "..\MArcCore\IFilesLocator.h"
#include "CNewFilesLocatorDlg.h"

class CLocatorFolder;
class CNewFilesLocatorFrame;
class CNewFilesLocatorView;
class CNewFilesLocatorDoc;

class CNewFilesLocator : public IFilesLocator
{
public:

  CFilesOnDisk m_foundFiles;
  CNewFilesLocatorDlg*   m_pDlg;
  CNewFilesLocatorView*  m_pView;
  CNewFilesLocatorFrame* m_pFrame;
  CNewFilesLocatorDoc*   m_doc; // 2014

  CNewFilesLocator( CMultiDocTemplate* i_locatorTemplate );
  virtual ~CNewFilesLocator();
  void Analyze( const CString& startPath );
  bool LoadOptions();
  bool CheckAndAddToExclFolders( const CFileOnDisk* const pFile );
  bool CheckAndAddToExclTypes( const CFileOnDisk* const pFile );

  // Overrides
  virtual bool init();
  virtual bool isAborted() const;
  virtual bool isSkippingSomeFiles() const    { return ( m_pDlg->m_bSkipSomeFiles != 0 ); };
  virtual CStringList& getExcludedFolders() const     { return m_excludedFolders; }
  virtual CStringList& getExcludedFileTypes() const   { return m_excludedFileTypes; }
  virtual void showFolderInDlg( const CString& strFolder ) const;
  virtual void showProgress( const CString& strFName ) const  { m_pDlg->ShowProgress( strFName ); };
  virtual int getDays() const { return m_pDlg->m_nDays; };
  virtual void addToList( CFileOnDisk* pFile );
  virtual void enableControls( bool bOnOff );


protected:

  CLocatorFolder* m_pStartFolder; // Locator starts with this Folder, 
      //  then this Folder will scan its subfolders recursivelly

  static CStringList m_excludedFolders;
  static CStringList m_excludedFileTypes;

  int  RemoveFilesOfType( const CString& sExtension );

  bool AddExcludedToDB( LocatorWhatToExclude nType, const CString& sName );
  bool AddExcludedTypeToDB( const CString& sExtension );
  bool AddExcludedFolderToDB( const CString& sFolderName );

private:
  CMultiDocTemplate* m_locatorTemplate; // 2014

};

#endif /* CNewFilesLocator_h */
