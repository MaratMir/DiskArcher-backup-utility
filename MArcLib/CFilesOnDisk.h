// DiskArcher.
// CFilesOnDisk class - a list of files.
// (C) Marat Mirgaleev, 2004.
// Created 11.07.2004.
// Modifications:
//================================================================================

#if !defined( CFILESONDISK_H__INCLUDED)
#define CFILESONDISK_H__INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "CFileOnDisk.h"


class CFilesOnDisk : public CList< CFileOnDisk*, CFileOnDisk* >
{
public:
    ~CFilesOnDisk();

    POSITION FindPos( CString sPath, CString sName ) const;
    CFileOnDisk* Find( CString sPath, CString sName ) const;
    unsigned int Remove( CString sPath, CString sName );
};

#endif // !defined(CFILESONDISK_H__INCLUDED)
