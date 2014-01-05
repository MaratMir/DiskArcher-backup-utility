// DiskArcher.
// CLogFile.cpp - Implementation of the CLogFile class.
// (C) Marat Mirgaleev, 2001-2002.
// Created 19.01.2001.
//	(1) 01.11.2002. theDB changed to *pTheDB.
//=====================================================================

#include "stdafx.h"
#include "CLogFile.h"
#include <afx.h>
#include "CMyArchive.h"

#include "CArchiveDB.h"

#import "C:\Program Files\Common Files\system\ado\msado15.dll" no_namespace rename( "EOF", "adoEOF" )


CLogFile::CLogFile()
{
}


// Add the message to the Log.
// Parameters:	- string of the message.
//===============================================
bool CLogFile::AddRecord( CString strFilePath, CString strFilename, CString strMessage )
{
	bool bSuccess = false;
	try
	{	// Append record
		CString cmd;
		CString dt = COleDateTime::GetCurrentTime().Format();
		cmd.Format( 
/*			"INSERT INTO ArcLog (MessDateTime)"
			" VALUES (\"%s\")",
			dt, strFilePath, strFilename, strMessage );
*/			"INSERT INTO ArcLog (MessDateTime, Path, Filename, Message)"
			" VALUES (\"%s\", \"%s\", \"%s\", \"%s\")",
			dt, strFilePath, strFilename, strMessage );
		((_ConnectionPtr)g_pTheDB->m_pConnection)->Execute( 
              													(LPCSTR)cmd, NULL, NULL );
		bSuccess = true;
	}
	catch(_com_error &e)
	{	// Notify the user of errors if any
		ShowADOErrors( e, g_pTheDB->m_pConnection );
	}
	catch(...)
	{
		AfxMessageBox( "Some error occured in CLogFile::AddRecord()." );
	}
	return bSuccess;
}


void CLogFile::EraseAll()
{
}
