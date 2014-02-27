// DiskArcher.
// CFilesOnDisk class - a list of files.
// (C) Marat Mirgaleev, 2004-2014.
// Created 11.07.2004.
// Modifications:
//================================================================================

#include "stdafx.h"
#include <algorithm>
#include "CFileOnDisk.h"
#include "CFilesOnDisk.h"


//==============================================================================
CFilesOnDisk::~CFilesOnDisk()
{
  for( auto curFile : m_files )
    delete curFile;
}


// Finds and removes from the list all files with given path and name.
// Returns the number of deleted files.
//==============================================================================
unsigned int CFilesOnDisk::remove( CString i_path, CString i_name )
{
  unsigned int nFound = 0;
  while( true )
  {
    bool found = false;
    for( auto curFile : m_files ) // Is this syntax working? Yes! C++ evolves!
    {
      CString curPath = curFile->getFullPath();
      if( curPath == i_path && curFile->m_strName == i_name )
      {
        m_files.remove( curFile );
        delete curFile;
        found = true;
        nFound++;
        break;  // i.e. restart with a new iterator
      }
    } // for
    if( ! found ) // Deleted all matching items
      break;
  } // while

  return nFound;
}
