// DiskArcher.
// CFilesOnDisk class - a list of files.
// (C) Marat Mirgaleev, 2014.
// Created 11.07.2004.
// Note: Do not add objects created on stack - it leads to an exception in the collection destruction.
//====================================================================================================

#pragma once

#include <afxtempl.h>
#include "CFileOnDisk.h"


class CFilesOnDisk : public CList< CFileOnDisk*, CFileOnDisk* >
  // Yeah, I know that inheritance from containers has some drawbacks, but in this case that's fine
{
public:
    ~CFilesOnDisk(); // I'd make it virtual, but the base CList destructor is not virtual

    POSITION FindPos( CString sPath, CString sName ) const;

    // sPath shall end with a slash
    CFileOnDisk* Find( CString sPath, CString sName ) const;

    unsigned int Remove( CString sPath, CString sName );
};
