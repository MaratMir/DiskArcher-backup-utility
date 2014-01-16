// DiskArcher.
// CFilesOnDisk class - a list of files.
// (C) Marat Mirgaleev, 2004.
// Created 11.07.2004.
// Modifications:
//================================================================================

#include "stdafx.h"
#include "CFilesOnDisk.h"


//==============================================================================
CFilesOnDisk::~CFilesOnDisk()
{
	POSITION pos;
	for( pos = GetHeadPosition(); pos != NULL; )
	{
		CFileOnDisk *pCurFile = GetNext( pos );
		delete pCurFile;
	}
}


//==============================================================================
POSITION CFilesOnDisk::FindPos( CString sPath, CString sName ) const
{
	POSITION posFound = NULL, prevPos = NULL;
	for( POSITION pos = GetHeadPosition(); pos != NULL; )
	{
    prevPos = pos;
		CFileOnDisk* pCurFile = GetNext( pos );
    CString curPath = pCurFile->getFullPath();
		if( curPath == sPath && pCurFile->m_strName == sName )
		{
			posFound = prevPos;
			break;
		}
	}
	return posFound;
}


// Returns the first occurence of a file given by a path and a name.
//==============================================================================
CFileOnDisk* CFilesOnDisk::Find( CString sPath, CString sName ) const
{
	CFileOnDisk* pFound = NULL;
	POSITION pos = FindPos( sPath, sName );
  if( pos != NULL )
    pFound = GetAt( pos );
	return pFound;
}


// Finds and removes from the list all files with given path and name.
// Returns the number of deleted files.
//==============================================================================
unsigned int CFilesOnDisk::Remove( CString sPath, CString sName )
{
    POSITION pos = NULL;
    unsigned int nFound = 0;
    while( ( pos = FindPos( sPath, sName ) ) != NULL )
    {
        CFileOnDisk* pFile = GetAt( pos );
        RemoveAt( pos );
        delete pFile;
        nFound++;
    }
    return nFound;
}
