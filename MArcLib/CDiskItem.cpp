// DiskArcher.
// CDiskItem.cpp - Implementation of CDiskItem class.
// (C) Marat Mirgaleev, 2001-2002.
// Modifications:
//	(1) 21.01.2002. m_nType added.
//	(2) 09.04.2002. Getting exact file name and path from the disk
//					to exclude differences in capitalization.
//	(3) 24.05.2002. Corrections in (2) for UNC.
//  (4) 26.07.2004. GetExtension() added.
//==============================================================================

#include "stdafx.h"
#include "CDiskItem.h"
#include "CFolder.h"

//zz#include "MArc2.h" // M


// (3) Totally changed
//==============================================================================
CDiskItem::CDiskItem( const CString &strFullName )
{
//   wchar_t cFullName[_MAX_PATH];
//   _tcscpy( cFullName, fullName ); // M If Unicode
	wchar_t drive[_MAX_DRIVE];
	wchar_t dir[_MAX_DIR];
	wchar_t fname[_MAX_FNAME];
	wchar_t ext[_MAX_EXT];

	CString strTmp = strFullName;	// (3) We copy the string because we can't change const &
	if( strTmp.Right(1) == "\\" )	// (3) Consider a folder as a file
		strTmp = strTmp.Left( strTmp.GetLength()-1 );

	_wsplitpath_s( strTmp/*(3)strFullName*/, drive, dir, fname, ext );
	// (3) m_strDrive = drive;
	// (3) m_strDir = dir;
	// (3) m_strName = CString(fname) + CString(ext);

// Support UNC
// I must do it because _splitpath doesn't work correctly with UNC!
	if( strFullName.Left( 2 ) == "\\\\" )	// If UNC path is specified
	// (3) Was if( m_strDir.Left( 2 ) == "\\\\" )	// If UNC...
	{
	// (3) In all places - strTmp instead of m_strDir

		int compEnd = strTmp.Find( _T("\\"), 2 );
		m_strComputer = strTmp.Left( compEnd+1 );

		int driveEnd = strTmp.Find( _T("\\"), compEnd+1 );
		if( driveEnd == -1 )	// (3) Slash not found
		{
			m_strDrive = strTmp.Mid( compEnd+1 );
			m_strDir   = "";	// (3)
			m_strName = "";		// (3)
		}
		else					// (3) Slash found
		{
			m_strDrive = strTmp.Mid( compEnd+1, driveEnd-compEnd );

			int dirEnd = strTmp.ReverseFind( '\\' );				// (3)
			if( dirEnd > driveEnd )									// (3)
				m_strDir = strTmp.Mid( driveEnd+1, dirEnd-driveEnd );// (3)

			if( strTmp.GetLength() > dirEnd )				// (3)
				m_strName = CString(fname) + CString(ext);	// (3)
			else
				m_strName = "";
		}
	}
	else	// (3) All in brackets added
	{
		m_strComputer = "";
		m_strDrive = drive;
		m_strDir = dir;
		m_strName = CString(fname) + CString(ext);
	}
}


// Add slash, if there isn't
//==============================================================================
CString AddWithSlash( const CString& str1, const CString& str2 )
{
	CString res = str1;
	if( res.Right( 1 ) != "\\" )
		res += "\\";
	if( str2.Left(1) == "\\" )
		res += str2.Mid( 1 );
	else
		res += str2;
	return res;
}


//==============================================================================
CString CDiskItem::getFullPath() const
{
	CString strResult;
	if( m_strComputer.IsEmpty() )
		strResult = m_strDrive;
	else
		strResult = AddWithSlash( m_strComputer, m_strDrive );
	strResult = AddWithSlash( strResult, m_strDir );
	return strResult;
}


//==============================================================================
CString CDiskItem::getFullName() const
{
	CString strResult = getFullPath();
	strResult = AddWithSlash( strResult, m_strName );
	return strResult;
}


// (4)
//==============================================================================
CString CDiskItem::getExtension() const
{
	wchar_t drive[_MAX_DRIVE];
	wchar_t dir[_MAX_DIR];
	wchar_t fname[_MAX_FNAME];
	wchar_t ext[_MAX_EXT];

	_wsplitpath_s( m_strName, drive, dir, fname, ext );
    return ext+1;   // exclude the period (.)
}


//==============================================================================
bool CDiskItem::checkExistence()
{
	return getInfo();
}


//==============================================================================
bool CDiskItem::getInfo()
{
	CFileFind finder;
	BOOL bFound = finder.FindFile( getFullName() );
		// TO DO: Only one file must be found!
	if( bFound )
	{
		/*bNext=*/ finder.FindNextFile();

	// (2) Get exact file name from the disk to exclude differences
	//	   in capitalization
		CDiskItem tmpDiskItem( finder.GetFilePath() );

		CString strRoot = tmpDiskItem.getFullPath();
			/* (3) Was, but was wrong with UNC:
				CString strRoot = finder.GetRoot(); */
		CDiskItem roottt( strRoot );

		if( ! roottt.m_strName.IsEmpty() )
		{
			roottt.getInfo();	// Recursively
			this->m_strDir = AddWithSlash( roottt.m_strDir, roottt.m_strName ) + L"\\";
		}
		this->m_strName = tmpDiskItem.m_strName;
	// End of (2)

		FILETIME ft;
// TO DO: Error processing?
		/*if(!  ??? */ finder.GetLastWriteTime( &ft );
		m_LastWriteTime = ft;

    m_nSize = (DWORD)finder.GetLength(); // TODO! Get over the limitation of 4GB file size!
		if( finder.IsDirectory() )
			m_nType = DI_FOLDER;
		else
			m_nType = DI_FILE;
	}
	finder.Close();
	return (bFound != 0);
}
