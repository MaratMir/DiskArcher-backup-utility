// DiskArcher.
// Implementation of the CBundles class.
// (C) Marat Mirgaleev, 2001-2002.
// Created 19.02.2002 - moved from CMyArchive.
// Modifications:
//	(1) 10.04.2002. Delete() - Freeing memory.
//	(2) 01.11.2002. theDB changed to *pTheDB.
//	(3) 11.04.2003. Compression.
//=================================================================

#include "stdafx.h"
#include "CBundles.h"
#include "CBundle.h"
#include "CRoom.h"
#include "CMyArchive.h"
#include "CArchiveDB.h"
#include "CZipBundle.h"


// Create a new Bundle.
// Parameters:
//	- Room where to create the Bundle;
//	- type of the Bundle (zipped or not).
// Returns NULL if fails.
//=========================================
CBundle* CBundles::BundleCreate( CRoom *pRoom, CBundle::bundleType nBundleType/*(3)*/ )
{
	bool bSuccess = false;
	CBundle* pNewBundle = NULL;

	switch( nBundleType )	// (3)
	{
  case CBundle::btSingleFile:
		pNewBundle = new CBundle;// It will be deleted in CMyArchive destructor
		pNewBundle->m_strExtension = "MAB";
		break;
  case CBundle::btZipFile:
		pNewBundle = new CZipBundle;
			// It will be deleted in CMyArchive destructor
		pNewBundle->m_strExtension = "ZIP";
	}

	pNewBundle->m_nType		  = nBundleType;
	pNewBundle->m_nRoomID     = pRoom->m_nRoomID;
	pNewBundle->m_strComputer = pRoom->m_strComputer;
	pNewBundle->m_strDrive	  = pRoom->m_strDrive;
  CString addSlashIfNeeded = pRoom->m_strDir.Right(1)==L"\\" ? L"" : L"\\";
  pNewBundle->m_strDir = pRoom->m_strDir + addSlashIfNeeded + pRoom->m_strName;

// Get a new Bundle ID: it will be maxID+1
  int maxID = g_TheArchive.m_pDB->BundleGetMaxID();
	bSuccess = ( maxID != -1 );	// TO DO: Message?
	if( bSuccess )
	{
		pNewBundle->m_nBundleID = maxID + 1;

		CString strBundleName;
		strBundleName.Format( _T("%8d.%3s"), pNewBundle->m_nBundleID, 
							  pNewBundle->m_strExtension );
		strBundleName.Replace( _T(" "), _T("0") );
		pNewBundle->m_strName = strBundleName;

    // Write the Bundle to DB
    bSuccess = g_TheArchive.m_pDB->BundleAdd( pNewBundle );
// TO DO: When must I write the bundle to DB?
//			Here? But if the archiving fails, I must delete this bundle from DB?
//			Or just check times-to-times are there any not-used bundles in DB?
	}
	if( bSuccess )
		g_TheArchive.m_Bundles.AddTail( pNewBundle );
	else
	{
		delete pNewBundle;
		pNewBundle = NULL;
	}
	return pNewBundle;
}


// Return NULL if there is not a Bundle with this ID
CBundle* CBundles::BundleFind( ID nBundleID )
{
	CBundle *pFound = NULL;
	POSITION pos;
	for( pos = g_TheArchive.m_Bundles.GetHeadPosition(); pos != NULL; )
	{
		CBundle *pCurBundle = g_TheArchive.m_Bundles.GetNext( pos );
		if( pCurBundle->m_nBundleID == nBundleID )
		{
			pFound = pCurBundle;
			break;
		}
	}
	return pFound;
}


bool CBundles::DeleteBundle( CBundle* pBundle )
{
	bool bSuccess = true;

// Delete the Bundle from DB
  if( bSuccess )
    bSuccess = g_TheArchive.m_pDB->BundleDelete( pBundle );

// Delete the Bundle from memory (from the list)
	if( bSuccess )
	{
		POSITION pos = Find( pBundle );
		ASSERT( pos );
		RemoveAt( pos );

		delete pBundle;	// (1)
	}	

	return bSuccess;
}
