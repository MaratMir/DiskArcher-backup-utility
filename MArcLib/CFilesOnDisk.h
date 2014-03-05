// DiskArcher.
// CFilesOnDisk class - a list of files.
// (C) Marat Mirgaleev, 2004-2014.
// Created 11.07.2004.
// Note: Do not add objects created on stack - it leads to an exception in the collection destruction.
// This object is responsible for destruction of the objects contained in the collection.
//====================================================================================================

#pragma once

#include <vector>

#include "CFileOnDisk.h"

class CFilesOnDisk
{

public:

  virtual ~CFilesOnDisk();

  // 2014: Eliminated:
  // POSITION findPos( CString sPath, CString sName ) const;
  // CFileOnDisk* find( CString i_path, CString i_name ) const;

  unsigned int remove( CString i_path, CString i_name );


public:

  std::vector<CFileOnDisk*> m_files;

};
