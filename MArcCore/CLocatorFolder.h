// DiskArcher.
// CLocatorFolder class interface file.
// Folder which CNewFilesLocator scans for files that are not in Archive yet
//		(recursively with subfolders).
// (C) Marat Mirgaleev, 2002.
// Created 22.10.2002 - extracted from CNewFilesLocator.
// Modifications:
//==========================================================================

#pragma once

#include "CFolder.h"

class IFilesLocator;


class CLocatorFolder : public CFolder
{
public:
  CLocatorFolder( const CString& strFullName, IFilesLocator* pLocator );
  CLocatorFolder( const CFolder& folder, IFilesLocator* pLocator );

  void Analyze();

protected:
  IFilesLocator* m_pLocator; // The Locator that controls this object

};
