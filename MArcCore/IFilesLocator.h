// DiskArcher.
// IFilesLocator interface - Locator of new files in a given folder and its subfolders.
// (C) Marat Mirgaleev, 2002-2014.
//======================================================================================

#pragma once

#include "CFileOnDisk.h"

class IFilesLocator
{
public:
  virtual ~IFilesLocator() {}

  virtual bool init() = 0;
  virtual bool isAborted() const = 0;
  virtual bool isSkippingSomeFiles() const = 0;
  virtual CStringList& getExcludedFolders() const = 0;
  virtual CStringList& getExcludedFileTypes() const = 0;
  virtual void showFolderInDlg( const CString& strFolder ) const = 0;
  virtual void showProgress( const CString& strFName ) const = 0;
  virtual int getDays() const = 0;
  virtual void addToList( CFileOnDisk* pFile ) = 0;
  virtual void enableControls( bool bOnOff ) = 0;
};
