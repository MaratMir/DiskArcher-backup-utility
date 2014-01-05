// DiskArcher.
// CFileOnDisk.h - Implementation of CFileOnDisk class.
// (C) Marat Mirgaleev, 2001-2002.
// Modifications:
//	(1) 21.01.2002. GetType() removed.
//	(2) 21.08.2002. SetType() added to constructor.
//=======================================================

#include "stdafx.h"
#include "CFileOnDisk.h"


CFileOnDisk::CFileOnDisk(  const CString& strFullName ) : CDiskItem( strFullName )
{
	setType( DI_FILE );	// (2)
		// Probably, we know that this is a File, not a Folder,
		//	if we call this constructor
}
