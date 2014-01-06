// DiskArcher.
// CArchiveDB.cpp - Implementation of CArchiveDB class.
// (C) Marat Mirgaleev, 2001-2014.
// Modifications:
//	(1) 15.01.2002. Log Table added.
//	(2) 04.02.2002. FilesToArc: Field "IsSystem" added.
//	(3) 09.02.2002. FilesToArc: A record about the Database.
//	(4) 10.02.2002.	IsOpen() added.
//					FileCopies: Field "DeleteIt" added.
//	(5)	11.02.2002. CopyDelete() added.
//	(6)	18.02.2002. FileDelete() added.
//	(7)	20.02.2002. BundleDelete() and RoomDelete() added.
//	(8) 03.03.2002. "FoldersToArc" table added.
//					In "FilesToArchive" table "FolderID" and "Paused" 
//						fields added.
//	(9) 07.03.2002. FileUpdate() added.
// (10) 09.04.2002. Correcting error in path capitalization
//					when the DB file is adding to FilesToArc.
// (11) 09.05.2002. CoInitialize() and CoUninitialize() moved out from here.
//					FilesLoad() - ORDER BY FileName.
// (12) 15.10.2002. Creating of DB copies before structure modifications 
//					disabled.
//					Table "Options" added.
// (13) 12.11.2002. Files Order	changed.
// (14) 26.01.2003. Error in LocatorRestoreDefaultOptions() fixed.
// (15) 12.03.2003. More detailed error tracking in Create().
// (16) 06.04.2003. Corrections in (15).
// (17) 11.05-20.11.2003. Compression.
// (18) 02.05.2006. ADO compilation warning suppressed.
//                  optionRead() added.
// (19) 23.12.2013. Migration to VS 2012 and Unicode.
//                  See the modification history on Github.
//======================================================================================

#include "stdafx.h"
#include "CArchiveDB.h"
#include "CFileToArc.h"
#include "CFilesToArc.h"
#include "CMyArchive.h"
#include "CRoom.h"

#include "MArc2.h"		// M
#include "CFileCopy.h"	// M
#include "CBundle.h"	// M
#include "Miscelaneous.h"	// (2)
#include "CFolderToArc.h"	// (8)
#include "CZipBundle.h"	// M


// Returns a "TRUE" or "FALSE" string.
//======================================
const CString TrueFalse( bool value )
{
	return CString( value ? "TRUE" : "FALSE" );
}


// Display a description of the ADO error.
// Parameters:	- pConnection is not required, default is NULL.
// LATER: The text of this window must be able to copy to clipboard!!!
//======================================================================
void ShowADOErrors( _com_error &e, _ConnectionPtr pConnection,
				    CString strAdditionalInfo/*(17)*/ )
{
	CString mess, tmp;

// Print Provider Errors from Connection object.
// pErr is a record object in the Connection's Error collection.
	if( pConnection != NULL )
	{
	    ErrorPtr    pErr  = NULL;
		if( (pConnection->Errors->Count) > 0)
		{
			long nCount = pConnection->Errors->Count;
	        for( long i = 0; i < nCount; i++ )
		    {
			    pErr = pConnection->Errors->GetItem(i);
				tmp.Format( _T("Provider Error %x:\nDescription: %s\n"), pErr->Number,
							(LPCTSTR)(pErr->Description) );
	            mess += tmp;
		    }
            mess += "\n";
		}
    }
	else									// (15)
		mess += "pConnection = NULL.\n";	// (15)

// Print COM errors
	_bstr_t bstrSource( e.Source() );
	_bstr_t bstrDescription( e.Description() );
	tmp.Format( _T("COM Error %08lx\n%s"), e.Error(), e.ErrorMessage() );
	mess += tmp;
	CString src = (LPCSTR)bstrSource;	
	mess += "\nSource: " + src;
	CString des = (LPCSTR)bstrDescription;
	mess += "\nDescription: " + des;
	
	if( strAdditionalInfo != "" )	// (17)
		mess += "\n" + strAdditionalInfo;

	AfxMessageBox( mess );
}


// Create the Database and its Tables.
// This code was copied from MSDN Jan 2000 - Platform SDK 
//	- Data Access Services - MDAC SDK - ADO - ADOX Reference 
//	- ADOX API Ref. - ADOX Code Examples - VC++ - Create Method.
//==================================================================
bool CArchiveDB::Create()
{
// LATER: Wait cursor/ Progress bar
	CString step = "0";		// (15)
	bool bSuccess = false;	// (15)
	LCID locale = GetUserDefaultLCID(); // (15)

// Create Database first
//===================================================================

	HRESULT hr = S_OK;

// Define ADOX object pointers.
// Initialize pointers on define.
// These are in the ADOX::  namespace.
	ADOX::_CatalogPtr pCatalog = NULL;

// Set ActiveConnection of Catalog to this string
	_bstr_t strcnn( "Provider=Microsoft.JET.OLEDB.4.0;"
// LATER: move this strings into constants or resources
					// Can add: "Locale Identifier=1049;" Russian or 1033 USA
				    "Data source = " + m_strDBFilename );
	try
	{
		step = "10";	  // (15)
		hr = pCatalog.CreateInstance(__uuidof (ADOX::Catalog));
		step = "15";      // (16)
		TESTHR( hr );
		step = "20";      // (15)
		pCatalog->Create( strcnn );
		step = "25";      // (16)
		m_pConnection = pCatalog->GetActiveConnection();	// (16)
		bSuccess = true;  // (15)
	}   
	catch(_com_error &e)
	{
	// Notify the user of errors if any
		ShowADOErrors( e );
	}
	catch(...)
	{
		bSuccess = false;  /* (15) Was:
		AfxMessageBox( "Some error occured in CArchiveDB::Create(): [1]." );*/
	}

	// (16) Moved into "try" above: m_pConnection = pCatalog->GetActiveConnection();
	

// Create Tables in the Database now
//===================================================================
	if( ( hr == S_OK ) && bSuccess/*(15)*/ )
	{
		bSuccess = false; // (15) Just clear the flag
		step = "30";	  // (15)
		ADOX::_TablePtr pTable = NULL;
    try
    {
		// Table "Files To Archive"
		//=================================
			step = "40";	  // (15)
      hr = pTable.CreateInstance(__uuidof(ADOX::Table));
      TESTHR( hr );
			step = "50";	  // (15)
      pTable->PutName( "FilesToArchive" );
      pTable->Columns->Append( "Computer", ADOX::adWChar,   MAX_COMPUTERNAME_LENGTH );
      pTable->Columns->Append( "Drive",	 ADOX::adWChar,   15/*_MAX_DRIVE=3 - it's not enough*/);
      pTable->Columns->Append( "Dir",		 ADOX::adWChar,   255/*_MAX_DIR=256 - it's too long*/ );
      pTable->Columns->Append( "Name",	 ADOX::adWChar,   255/*_MAX_FNAME=256 - it's too long*/ );
	// Including extension
      pTable->Columns->Append( "Priority",   ADOX::adInteger, 0 );
      pTable->Columns->Append( "UpToCopies", ADOX::adInteger, 0);
			pTable->Columns->Append( "IsSystem",   ADOX::adBoolean, 0 );	// (2)
			pTable->Columns->Append( "FolderID",   ADOX::adInteger, 0 );	// (8)
			pTable->Columns->Append( "Paused",	   ADOX::adBoolean, 0 );	// (8)
			step = "70";	  // (15)
			pCatalog->Tables->Append( _variant_t((IDispatch *)pTable) );

		// Table "Archive Rooms"
		//=================================
			step = "80";	  // (15)
      hr = pTable.CreateInstance(__uuidof(ADOX::Table));
      TESTHR( hr );
			step = "90";	  // (15)
      pTable->PutName( "Rooms" );
      pTable->Columns->Append( "RoomID",		ADOX::adInteger, 0 );
      pTable->Columns->Append( "Filename",	ADOX::adWChar,   255/*_MAX_DIR=256 - it's too long*/ );
      pTable->Columns->Append( "Removable",	ADOX::adBoolean, 0 );
      pTable->Columns->Append( "SizeLimit",	ADOX::adInteger, 0 );
      pTable->Columns->Append( "SpaceFree",	ADOX::adInteger, 0);
			pCatalog->Tables->Append( _variant_t((IDispatch *)pTable) );

		// Table "FileCopies"
		//=================================
      hr = pTable.CreateInstance(__uuidof(ADOX::Table));
      TESTHR( hr );
      pTable->PutName( "FileCopies" );
      pTable->Columns->Append( "CopyID",	   ADOX::adInteger,  0 );
      pTable->Columns->Append( "Path",	   ADOX::adWChar,    255/*_MAX_DIR=256 - it's too long*/ );
      pTable->Columns->Append( "Filename",   ADOX::adWChar,    255/*_MAX_DIR=256 - it's too long*/ );
      pTable->Columns->Append( "UserID",	   ADOX::adWChar,    MyArcUserLen );
      pTable->Columns->Append( "FileDateTime", ADOX::adDate,	 0 );
	// They don't work: ADOX::adFileTime; ADOX::adDBTimeStamp 
      pTable->Columns->Append( "SourceSize",   ADOX::adInteger,  0 );
	// adInteger is 32-bit long, so the limit of size is 4 Gb!!!
      pTable->Columns->Append( "PackedSize",   ADOX::adInteger,  0 );
      pTable->Columns->Append( "BundleID",     ADOX::adInteger,  0 );
			pCatalog->Tables->Append( _variant_t((IDispatch *)pTable) );

		// Table "Bundles"
		//=================================
      hr = pTable.CreateInstance(__uuidof(ADOX::Table));
      TESTHR( hr );
      pTable->PutName( "Bundles" );
      pTable->Columns->Append( "BundleID",	ADOX::adInteger, 0 );
      pTable->Columns->Append( "RoomID",	ADOX::adInteger, 0 );
			pTable->Columns->Append( "Extension",	ADOX::adWChar,   3 );
      pTable->Columns->Append( "Type",		ADOX::adInteger, 0 );
      pTable->Columns->Append( "Path",		ADOX::adWChar, 255/*_MAX_DIR=256 - it's too long*/ );
      pTable->Columns->Append( "Filename",	ADOX::adWChar, 255/*_MAX_DIR=256 - it's too long*/ );
			pCatalog->Tables->Append( _variant_t((IDispatch *)pTable) );

		// (1) Table "ArcLog"
		//=================================
			step = "200";	  // (15)
			CreateArcLogTable( pCatalog );
	
			bSuccess = true;  // (15)
    }
	  catch(_com_error &e)
		{
		// Notify the user of errors if any
			ShowADOErrors( e, m_pConnection );
		}
	  catch(...)
		{
			bSuccess = false;  /* (15) Was:
			AfxMessageBox( "Some error occured in CArchiveDB::Create(): [2]." ); */
		}
	}
	if( ! bSuccess )	// (15)
	{
		CString mess;	// (15)
		mess.Format( _T("Some error occured in CArchiveDB::Create():\n")
					       _T("Step=%s. LCID=%d."), step, locale );	// (15)
		AfxMessageBox( mess );	// (15)
	}
	return bSuccess/*(15) Was ( hr == S_OK )*/;
}


// (17) Create a local copy of the database before open it:
//	some compression utilities cannot copy opened files,
//	also if the DB is damaged, program cannot start
//==========================================================
bool CArchiveDB::PreserveDB()
{
	BOOL bOk = CopyFile( m_strDBFilename, m_strDBFilename + "~",
						           FALSE/*Overwrite existing*/ );
	return( bOk == TRUE );
}


// Open the Database
//==============================================================================
bool CArchiveDB::Open()
{
	bool bSuccess = true;

	_ConnectionPtr pConnection    = NULL;
// Set Connection string
	_bstr_t strCnn( "Provider=Microsoft.JET.OLEDB.4.0;"
				    "Data source = " + m_strDBFilename );
// LATER     "Initial Catalog=Pubs;User Id=sa;Password=;");
//		I should protect data from editing in Access!

    HRESULT  hr = S_OK;
    try
    {
	// Open connection and record set
		hr = pConnection.CreateInstance(__uuidof(Connection));
        TESTHR( hr );
        pConnection->Open( strCnn, ""/*UserID*/, ""/*Pswd*/,
						   NULL/*Options*/ );
		m_pConnection = pConnection.GetInterfacePtr();


	// (2) Modify structure of the DB if needed
	//===========================================
		DBStructureModifications();

	// (3) Check is there a record about the Database in the FilesToArc table
	//========================================================================
		CheckRecordAboutDB();

	// (8) Modify DB structure if needed
	//===========================================
		DBStructureModifications2();

	}
    catch(_com_error &e)
    {
		ShowADOErrors( e );
		bSuccess = false;
	}

	return bSuccess;
}


// (4) Check is the DB open.
//===========================================
bool CArchiveDB::IsOpen()
{
	return ( m_pConnection.vt != VT_EMPTY );
}


// Close the Database
//==============================================================================
bool CArchiveDB::Close()
{
	bool bSuccess = true;

	try	// (12)
	{
		if( IsOpen() )
			// (4) Was	if( m_pConnection.vt != VT_EMPTY )	// Only if the connection has been established
			((_ConnectionPtr)m_pConnection)->Close();
	}
	catch(...)
	{
		AfxMessageBox( _T("Some error occured in CArchiveDB::Close().") );
	}

/*(11) Moved into the App
    ::CoUninitialize(); */

	return bSuccess;
}


// Add the file to DB.
// Parameters:	- a pointer to the CFileToArc object.
//==============================================================================
bool CArchiveDB::FileAdd(CFileToArc* pFile)
{
	bool bSuccess = false;

// Check is this file already in the Archive
//...........................................
	if ( ! CheckIsFileInArc( pFile ) )
	{
		try
		// This file isn't within Archive - append it
		{
			CString cmd;
			cmd.Format( 
        L"INSERT INTO FilesToArchive (Computer, Drive, Dir, Name, Priority, UpToCopies, "
                                     L"IsSystem, " // (2)
                                     L"FolderID, Paused)"	// (8)
        L" VALUES (\"%s\", \"%s\", \"%s\", \"%s\", %d, %d, %s, %d, %s)",
        pFile->m_strComputer, pFile->m_strDrive, pFile->m_strDir, pFile->m_strName,
        pFile->m_nPriority, pFile->m_nUpToCopies, TrueFalse( pFile->m_bSystem ),
        pFile->m_nFolderID/*(8)*/, TrueFalse( pFile->m_bPaused )/*(8)*/ );
		  bstr_t converted = cmd;
			((_ConnectionPtr)m_pConnection)->Execute( converted, NULL, NULL );
			bSuccess = true;
		}
		catch(_com_error &e)
		{
		// Notify the user of errors if any
			ShowADOErrors( e, m_pConnection );
		}
		catch(...)
		{
			AfxMessageBox( _T("Some error occured in CArchiveDB::FileAdd().") );
		}
	}
	else	// Already in Archive - why?
		AfxMessageBox( _T("INTERNAL ERROR: FileAdd: File Count > 1") );

	return bSuccess;
}


// (9) Update File Info in DB
//==============================================================================
bool CArchiveDB::FileUpdate(CFileToArc *pFile)
{
	bool bSuccess = false;
// Update record
	CString cmd;
	cmd.Format( 
		_T("UPDATE FilesToArchive ")
			_T("SET Priority=%d, UpToCopies=%d, Paused=%s, CompressIt=%s ")
			_T("WHERE Computer=\"%s\" AND Drive=\"%s\" AND Dir=\"%s\" ")
				_T("AND Name=\"%s\""),
		pFile->m_nPriority, pFile->m_nUpToCopies, 
		TrueFalse( pFile->m_bPaused ), TrueFalse( pFile->m_bCompressIt ),
		pFile->m_strComputer, pFile->m_strDrive, pFile->m_strDir, 
		pFile->m_strName );
	bSuccess = ExecSQL( cmd );

	return bSuccess;
}


// (6)
//==============================================================================
bool CArchiveDB::FileDelete( CFileToArc *pFile )
{
	bool bSuccess = false;
	try
	{
		CString cmd;
		cmd.Format( _T("DELETE FROM FilesToArchive WHERE Computer=\"%s\" AND ")
					_T("Drive=\"%s\" AND Dir=\"%s\" AND Name=\"%s\""),
					pFile->m_strComputer, pFile->m_strDrive, 
					pFile->m_strDir, pFile->m_strName );
	// Delete File's record from the FilesToArchive table
		bstr_t converted = cmd;
		((_ConnectionPtr)m_pConnection)->Execute( converted, NULL, NULL );
		bSuccess = true;
	}
	catch(_com_error &e)
	{
	// Notify the user of errors if any
		ShowADOErrors( e, m_pConnection );
	}
	catch(...)
	{
		AfxMessageBox( _T("Some error occured in CArchiveDB::CopyDelete().") );
	}
	return bSuccess;
}


// Load file list from DB into memory.
//==============================================================================
bool CArchiveDB::FilesLoad()
{
	bool bSuccess = false;

	_RecordsetPtr rstFiles;
	rstFiles.CreateInstance(__uuidof(Recordset)); 
	HRESULT hr;
	try
	{
	// Select all files
		CString select = "SELECT * FROM FilesToArchive "
						         "ORDER BY Drive, Dir, Name" 
							        /* (13) Was: "ORDER BY Name" */ ;
		bstr_t converted = select;
		hr = rstFiles->Open( converted, m_pConnection, adOpenStatic,
		          					 adLockReadOnly, adCmdText );
    TESTHR( hr );

		if( ! rstFiles->adoEOF )
			rstFiles->MoveFirst();
		while( ! rstFiles->adoEOF )
		{
			CFileToArc *pCurFile = new CFileToArc();	// It will be freed in CMyArchive destructor
			_bstr_t  bstrTmp; // Temporary string for type conversion
      bstrTmp = rstFiles->Fields->Item["Computer"]->Value;
			pCurFile->m_strComputer = bstrTmp.GetBSTR(); // Was (LPCSTR)bstrTmp;
			pCurFile->m_strComputer.TrimRight();
            
      bstrTmp = rstFiles->Fields->Item["Drive"]->Value;
			pCurFile->m_strDrive = bstrTmp.GetBSTR(); // Was (LPCSTR)bstrTmp;
			pCurFile->m_strDrive.TrimRight();

      bstrTmp = rstFiles->Fields->Item["Dir"]->Value;
			pCurFile->m_strDir = bstrTmp.GetBSTR(); // Was (LPCSTR)bstrTmp;
			pCurFile->m_strDir.TrimRight();

      bstrTmp = rstFiles->Fields->Item["Name"]->Value;
      pCurFile->m_strName = bstrTmp.GetBSTR();
			pCurFile->m_strName.TrimRight();

			_variant_t vtTmp;
      vtTmp = rstFiles->Fields->Item["Priority"]->Value;
      pCurFile->m_nPriority = (long)vtTmp;

      vtTmp = rstFiles->Fields->Item["UpToCopies"]->Value;
		// (17) If number of file's copies has not been set then use default.
    //      Keep it for backward compatibility
			int nUpToCopies = (long)vtTmp;
			if( nUpToCopies > 0 && nUpToCopies < 100 )
				pCurFile->m_nUpToCopies = nUpToCopies;
			else
				pCurFile->m_nUpToCopies = g_TheArchive.m_nDefaultCopies;
		// End of (17). Was just: pCurFile->m_nUpToCopies = (long)vtTmp;

      vtTmp = rstFiles->Fields->Item["IsSystem"]->Value;	// (2)
      pCurFile->m_bSystem = (bool)vtTmp;

      vtTmp = rstFiles->Fields->Item["FolderID"]->Value;	// (8)
			if( vtTmp.vt == VT_NULL )	// The value not assigned
				pCurFile->m_nFolderID = 0;
			else
				pCurFile->m_nFolderID = (long)vtTmp;

      vtTmp = rstFiles->Fields->Item["Paused"]->Value;	// (8)
      pCurFile->m_bPaused = (bool)vtTmp;

      vtTmp = rstFiles->Fields->Item["CompressIt"]->Value;	// (17)
      pCurFile->m_bCompressIt = (bool)vtTmp;

			m_pArchive->m_FilesToArc.AddTail( pCurFile );
			rstFiles->MoveNext();
		}

		rstFiles->Close();

		bSuccess = true;
	}
	catch(_com_error &e)
	{
	// Notify the user of errors if any
		ShowADOErrors( e, m_pConnection );
	}
	catch(...)
	{
		AfxMessageBox( _T("Some error occured in CArchiveDB::FilesLoad().") );
	}

	return bSuccess;
}


// Add the Room to DB.
// Parameters:	- a pointer to the CRoom object.
//==============================================================================
bool CArchiveDB::RoomAdd(CRoom *pRoom)
{
	bool bSuccess = false;
	try
	{	// Append record
		CString cmd;
		cmd.Format( 
			L"INSERT INTO Rooms ( RoomID, FileName, Removable, SizeLimit, SpaceFree,\n"
			                   L" CompressionMode )\n"	// (17)
			L" VALUES ( %d, \"%s\", %s, %d, %d, %d )",
			pRoom->m_nRoomID, pRoom->getFullName(),
			TrueFalse( pRoom->m_bRemovable ), pRoom->m_nSizeLimit,
			pRoom->m_nDiskSpaceFree, pRoom->m_nCompressionMode );
		bstr_t converted = cmd;
		((_ConnectionPtr)m_pConnection)->Execute( converted, NULL, NULL );
		bSuccess = true;
	}
	catch(_com_error &e)
	{	// Notify the user of errors if any
		ShowADOErrors( e, m_pConnection );
	}
	catch(...)
	{
		AfxMessageBox( _T("Some error occured in CArchiveDB::RoomAdd().") );
	}
	return bSuccess;
}


// Load Room list from DB into memory.
//==============================================================================
bool CArchiveDB::RoomsLoad()
{
	bool bSuccess = false;

	_RecordsetPtr rsRooms;
	rsRooms.CreateInstance(__uuidof(Recordset)); 
	HRESULT hr;
	try
	{
	// Select all Rooms
		CString select = "SELECT * FROM Rooms";  // LATER: ORDER BY
		bstr_t converted = select;
		hr = rsRooms->Open( converted, m_pConnection, adOpenStatic,
							 adLockReadOnly, adCmdText );
    TESTHR( hr );

// (17)	if( ! rsRooms->adoEOF )
// (17)		rsRooms->MoveFirst();	// ? Is not necessary?
		while( ! rsRooms->adoEOF )
		{
			_bstr_t  bstrTmp; // Temporary string for type conversion
			_variant_t vtTmp;
			CString strTmp;
      bstrTmp = rsRooms->Fields->Item["Filename"]->Value;
			strTmp = bstrTmp.GetBSTR(); // Was (LPCSTR)bstrTmp;
			strTmp.TrimRight();
			CRoom *pCurRoom = new CRoom( strTmp );
			  // It will be freed in CMyArchive destructor

      vtTmp = rsRooms->Fields->Item["RoomID"]->Value;
      pCurRoom->m_nRoomID = (long)vtTmp;

      vtTmp = rsRooms->Fields->Item["Removable"]->Value;
      pCurRoom->m_bRemovable = (bool)vtTmp;

      vtTmp = rsRooms->Fields->Item["SizeLimit"]->Value;
      pCurRoom->m_nSizeLimit = (long)vtTmp;

      vtTmp = rsRooms->Fields->Item["SpaceFree"]->Value;
      pCurRoom->m_nDiskSpaceFree = (long)vtTmp;
			pCurRoom->m_nDiskSpaceFree = pCurRoom->m_nDiskSpaceFree << 10;	// LATER ???

      vtTmp = rsRooms->Fields->Item["CompressionMode"]->Value;
			if( vtTmp.vt == VT_NULL )	// The value not assigned
				pCurRoom->m_nCompressionMode = rcmAllowed;
			else
				pCurRoom->m_nCompressionMode = 
									(enum roomCompressionMode)((long)vtTmp);

			m_pArchive->m_Rooms.AddTail( pCurRoom );
			rsRooms->MoveNext();
		}
		rsRooms->Close();

		bSuccess = true;
	}
	catch(_com_error &e)
	{
	// Notify the user of errors if any
		ShowADOErrors( e, m_pConnection, "Place: RoomsLoad" );
	}
	catch(...)
	{
		AfxMessageBox( _T("Some error occured in CArchiveDB::RoomsLoad().") );
	}

	return bSuccess;
}


// Update the Room info in the Database.
//==============================================================================
bool CArchiveDB::RoomUpdate(CRoom *pRoom)
{
	bool bSuccess = false;
	CString cmd;
	cmd.Format( 
		_T("UPDATE Rooms ")
		_T("SET FileName=\"%s\", Removable=%s, SizeLimit=%d, SpaceFree=%d, ")
			_T("CompressionMode=%d ")
		_T("WHERE RoomID=%d"),
		pRoom->getFullName(),
		TrueFalse( pRoom->m_bRemovable ),
		pRoom->m_nSizeLimit, (int)(pRoom->m_nDiskSpaceFree >> 10)/*LATER!*/,
		pRoom->m_nCompressionMode,
		pRoom->m_nRoomID );
	bSuccess = ExecSQL( cmd );
	return bSuccess;
}


// Get max Room ID from Database.
// Returns -1 if any error occurs
//==============================================================================
int CArchiveDB::RoomGetMaxID()
{
	int nMax = GetMaxID( "Rooms", "RoomID" );
	return nMax;
}


// (7)
//==============================================================================
bool CArchiveDB::RoomDelete(CRoom *pRoom)
{
	bool bSuccess = false;
	try
	{
		CString cmd;
		cmd.Format( _T("DELETE FROM Rooms WHERE RoomID=%d"), pRoom->m_nRoomID );
	// Delete Room's record from the "Rooms" table
		bstr_t converted = cmd;
		((_ConnectionPtr)m_pConnection)->Execute( converted, NULL, NULL );
		bSuccess = true;
	}
	catch(_com_error &e)
	{
	// Notify the user of errors if any
		ShowADOErrors( e, m_pConnection );
	}
	catch(...)
	{
		AfxMessageBox( _T("Some error occured in CArchiveDB::RoomDelete().") );
	}
	return bSuccess;
}

//======================================================================================


// Update the Copy info in the Database.
//==============================================================================
bool CArchiveDB::CopyUpdate(CFileCopy *pCopy)
{
  bool bSuccess = false;
  CString cmd;
  try
  {	// Update record
    CString dt = pCopy->m_FileDateTime.Format();
    cmd.Format( 
      L"UPDATE FileCopies SET Path=\"%s\", FileName=\"%s\", UserID=\"%s\", FileDateTime=\"%s\", "
             L"SourceSize=%d, PackedSize=%d, BundleID=%d, DeleteIt=%s "
      L"WHERE CopyID=%d",
      pCopy->m_strPath, pCopy->m_strFilename, pCopy->m_strUser, dt,
      pCopy->m_nSize, pCopy->m_nPackedSize, pCopy->m_nBundleID,					
      TrueFalse( pCopy->m_bDeleteIt )/*(4)*/, pCopy->m_nCopyID );
    bstr_t converted = cmd;
    ((_ConnectionPtr)m_pConnection)->Execute( converted, NULL, NULL );
    bSuccess = true;
  }
  catch(_com_error &e)
  {	// Notify the user of errors if any
    ShowADOErrors( e, m_pConnection );
  }
  catch(...)
  {
    AfxMessageBox( _T("Some error occured in CArchiveDB::CopyUpdate().") );
  }
  return bSuccess;
}


// Add the Copy to DB.
// LATER: Make a general Add function?
//==============================================================================
bool CArchiveDB::CopyAdd( CFileCopy *pCopy )
{
	bool bSuccess = false;

// Get max Copy ID
	int maxID = CopyGetMaxID();
	ASSERT( maxID != -1 );	// LATER: Message?
	bSuccess = ( maxID != -1 );
	pCopy->m_nCopyID = maxID + 1;

	_RecordsetPtr recSet;
	if( bSuccess )
	{
		recSet.CreateInstance(__uuidof(Recordset)); 
		recSet->CursorType = adOpenStatic;
		try
		{
		// Append the record
		//============================================
      CString cmd, dt = pCopy->m_FileDateTime.Format();
      cmd.Format( 
        L"INSERT INTO FileCopies (CopyID, Path, Filename, UserID, FileDateTime, "
                    L"SourceSize, PackedSize, BundleID)"
        L" VALUES (%d, \"%s\", \"%s\", \"%s\", \"%s\", %d, %d, %d)",
        pCopy->m_nCopyID, pCopy->m_strPath, pCopy->m_strFilename, 
        pCopy->m_strUser, dt, pCopy->m_nSize, 
        pCopy->m_nPackedSize, pCopy->m_nBundleID );
      bstr_t converted = cmd;
      ((_ConnectionPtr)m_pConnection)->Execute( converted, NULL, NULL );

			bSuccess = true;
		}
		catch( _com_error &e )
		{
			ShowADOErrors( e, m_pConnection );
		}
		catch( CException *oe )
		{
			oe->ReportError();
		}
		catch(...)
		{
			AfxMessageBox( _T("Some error occured in CArchiveDB::CopyAdd().") );
		}
	}
	return bSuccess;
}


// (5)
//==============================================================================
bool CArchiveDB::CopyDelete( CFileCopy *pCopy )
{
	bool bSuccess = false;
	try
	{
		CString cmd;
		cmd.Format( _T("DELETE FROM FileCopies WHERE CopyID=%d"), pCopy->m_nCopyID );
	// Delete the Copy record from the FileCopies table
		bstr_t converted = cmd;
		((_ConnectionPtr)m_pConnection)->Execute( converted, NULL, NULL );
		bSuccess = true;
	}
	catch(_com_error &e)
	{
	// Notify the user of errors if any
		ShowADOErrors( e, m_pConnection );
	}
	catch(...)
	{
		AfxMessageBox( _T("Some error occured in CArchiveDB::CopyDelete().") );
	}
	return bSuccess;
}


// Get max Copy ID from Database.
// Returns -1 if any error occurs
//==============================================================================
int CArchiveDB::CopyGetMaxID()
{
	int nMax = GetMaxID( "FileCopies", "CopyID" );
	return nMax;
}


// Get max ID from Database.
// Parameters:	- Table name;
//				- Field name;
//				- additional SQL clauses.
// Returns -1 if any error occurs
//==============================================================================
int CArchiveDB::GetMaxID( CString strTableName, CString strFieldName, 
                          CString strAdditionalClauses )
{
	int nMax = -1;
	HRESULT hr;
	CString cmd;
	try
	{
    _RecordsetPtr rsMax;
    rsMax.CreateInstance(__uuidof(Recordset)); 
    cmd.Format( L"SELECT MAX(%s) As MaxID FROM %s %s",
                strFieldName, strTableName, strAdditionalClauses );
    bstr_t converted = cmd;
    hr = rsMax->Open( converted, m_pConnection, adOpenStatic,
                      adLockReadOnly, adCmdText );
    TESTHR( hr );

		_variant_t vtTmp;
	  vtTmp = rsMax->Fields->Item["MaxID"]->Value;
		if( vtTmp.vt == VT_NULL )	// There aren't any records in Rooms
			nMax = 0;
		else
			nMax = (long)vtTmp;
		hr = rsMax->Close();
    TESTHR( hr );
	}
	catch(_com_error &e)
	{	// Notify the user of errors if any
		ShowADOErrors( e, m_pConnection );
	}
	catch(...)
	{
		AfxMessageBox( _T("Some error occured in CArchiveDB::GetMaxID().") );
	}
	return nMax;
}


// Load Copies list from DB into memory.
//==============================================================================
bool CArchiveDB::CopiesLoad()
{
	bool bSuccess = false;

	_RecordsetPtr recSet;
	recSet.CreateInstance(__uuidof(Recordset)); 
	HRESULT hr;
	try
	{
	// Select all Copies
		CString select = L"SELECT * FROM FileCopies ORDER BY FileDateTime";
		bstr_t converted = select;
		hr = recSet->Open( converted, m_pConnection, adOpenStatic,
							 adLockReadOnly, adCmdText );
    TESTHR( hr );

		while( ! recSet->adoEOF )
		{
			CFileCopy *pCurCopy = new CFileCopy();	// It will be freed in CMyArchive destructor
			_bstr_t  bstrTmp; // Temporary string for type conversion
			_variant_t vtTmp;

      vtTmp = recSet->Fields->Item["CopyID"]->Value;
      pCurCopy->m_nCopyID = (long)vtTmp;

      bstrTmp = recSet->Fields->Item["Path"]->Value;
			pCurCopy->m_strPath = bstrTmp.GetBSTR(); // Was (LPCSTR)bstrTmp;
			pCurCopy->m_strPath.TrimRight();

      bstrTmp = recSet->Fields->Item["Filename"]->Value;
			pCurCopy->m_strFilename = bstrTmp.GetBSTR(); // Was (LPCSTR)bstrTmp;
			pCurCopy->m_strFilename.TrimRight();

      vtTmp = recSet->Fields->Item["FileDateTime"]->Value;
      pCurCopy->m_FileDateTime = vtTmp;
#ifdef _DEBUG
			CString copydt = pCurCopy->m_FileDateTime.Format();
#endif			

      vtTmp = recSet->Fields->Item["BundleID"]->Value;
			pCurCopy->m_nBundleID = (long)vtTmp;

      vtTmp = recSet->Fields->Item["DeleteIt"]->Value;	// (4)
      pCurCopy->m_bDeleteIt = (bool)vtTmp;				// (4)

      vtTmp = recSet->Fields->Item["SourceSize"]->Value;	// (5)
      pCurCopy->m_nSize = (long)vtTmp;					// (5)
      if( pCurCopy->m_nSize < 0 )							// (5) TO DO!
				pCurCopy->m_nSize = 0;							// (5)

      vtTmp = recSet->Fields->Item["PackedSize"]->Value;	// (5)
      pCurCopy->m_nPackedSize = (long)vtTmp;				// (5)
      if( pCurCopy->m_nPackedSize < 0 )					// (5) TO DO!
				pCurCopy->m_nPackedSize = 0;					// (5)

			m_pArchive->m_Copies.AddTail( pCurCopy );
			recSet->MoveNext();

		}
		recSet->Close();

		bSuccess = true;
	}
	catch(_com_error &e)
	{
	// Notify the user of errors if any
		ShowADOErrors( e, m_pConnection );
	}
	catch(...)
	{
		AfxMessageBox( _T("Some error occured in CArchiveDB::CopiesLoad().") );
	}

	return bSuccess;
}


// Load Bundles list from DB into memory.
//==============================================================================
bool CArchiveDB::BundlesLoad()
{
	bool bSuccess = false;

	_RecordsetPtr recSet;
	recSet.CreateInstance(__uuidof(Recordset)); 
	HRESULT hr;
	try
	{
	// Select all Bundles
		CString select = L"SELECT * FROM Bundles";
		bstr_t converted = select;
		hr = recSet->Open( converted, m_pConnection, adOpenStatic,
							         adLockReadOnly, adCmdText );
    TESTHR( hr );

		while( ! recSet->adoEOF )
		{
			_bstr_t  bstrTmp; // Temporary string for type conversion
			_variant_t vtTmp;
			CString fname;
			int iTmp;

      bstrTmp = recSet->Fields->Item["Path"]->Value;
			fname = bstrTmp.GetBSTR(); // Was (LPCSTR)bstrTmp;
			fname.TrimRight();

      bstrTmp = recSet->Fields->Item["Filename"]->Value;
			fname = fname + "\\" + bstrTmp.GetBSTR(); // Was (LPCSTR)bstrTmp;
			fname.TrimRight();

      vtTmp = recSet->Fields->Item["Type"]->Value;
			iTmp = (long)vtTmp;
			CBundle *pCurBundle = NULL;
			switch( iTmp )	// (17)
			{
			case btSingleFile:
				pCurBundle = new CBundle( fname );
					// It will be deleted in CMyArchive destructor
				pCurBundle->m_strExtension = "MAB";
				break;
			case btZipFile:
				pCurBundle = new CZipBundle( fname );
					// It will be deleted in CMyArchive destructor
				pCurBundle->m_strExtension = "ZIP";
				break;
			default:
				g_TheArchive.m_LogFile.AddRecord( "Bundle", "Loading", 
												                  "Unknown type of the Bundle" );
				continue;	// Impossible to go on with this record, go to next
			}
      pCurBundle->m_nType = (bundleType)iTmp;
			// End of (17). Was just: CBundle *pCurBundle = new CBundle( fname );

      vtTmp = recSet->Fields->Item["BundleID"]->Value;
      pCurBundle->m_nBundleID = (long)vtTmp;

      vtTmp = recSet->Fields->Item["RoomID"]->Value;
      pCurBundle->m_nRoomID = (long)vtTmp;

      bstrTmp = recSet->Fields->Item["Extension"]->Value;
			pCurBundle->m_strExtension = bstrTmp.GetBSTR(); // Was (LPCSTR)bstrTmp;
			pCurBundle->m_strExtension.TrimRight();

			m_pArchive->m_Bundles.AddTail( pCurBundle );
			recSet->MoveNext();
		}
		recSet->Close();

		bSuccess = true;
	}
	catch(_com_error &e)
	{
	// Notify the user of errors if any
		ShowADOErrors( e, m_pConnection );
	}
	catch(...)
	{
		AfxMessageBox( _T("Some error occured in CArchiveDB::BundlesLoad().") );
	}

	return bSuccess;
}


// Add the Bundle to DB.
//==============================================================================
bool CArchiveDB::BundleAdd(CBundle *pBundle)
{
	bool bSuccess = false;
	_RecordsetPtr recSet;
	recSet.CreateInstance(__uuidof(Recordset)); 
	recSet->CursorType = adOpenStatic;
	try
		{
		// Append the record
		//============================================
			CString cmd;
      cmd.Format( 
          L"INSERT INTO Bundles (BundleID, RoomID, Extension, Type, Path, Filename)"
          L" VALUES (%d, %d, \"%s\", %d, \"%s\", \"%s\")",
          pBundle->m_nBundleID, pBundle->m_nRoomID, pBundle->m_strExtension, 
          pBundle->m_nType, pBundle->getFullPath(), pBundle->m_strName );
      bstr_t converted = cmd;
      ((_ConnectionPtr)m_pConnection)->Execute( converted, NULL, NULL );

			bSuccess = true;
		}
		catch( _com_error &e )
		{
			ShowADOErrors( e, m_pConnection );
		}
		catch( CException *oe )
		{
			oe->ReportError();
		}
		catch(...)
		{
			AfxMessageBox( _T("Some error occured in CArchiveDB::BundleAdd().") );
		}

	return bSuccess;
}


// Get max Bundle ID from the Database.
// Returns -1 if any error occurs
//==============================================================================
int CArchiveDB::BundleGetMaxID()
{
	int nMax = GetMaxID( "Bundles", "BundleID" );
	return nMax;
}


// (7)
//==============================================================================
bool CArchiveDB::BundleDelete( CBundle* pBundle )
{
	bool bSuccess = false;
	try
	{
		CString cmd;
		cmd.Format( L"DELETE FROM Bundles WHERE BundleID=%d", pBundle->m_nBundleID );
	// Delete Bundle's record from the "Bundles" table
		bstr_t converted = cmd;
		((_ConnectionPtr)m_pConnection)->Execute( converted, NULL, NULL );
		bSuccess = true;
	}
	catch(_com_error &e)
	{
	// Notify the user of errors if any
		ShowADOErrors( e, m_pConnection );
	}
	catch(...)
	{
		AfxMessageBox( _T("Some error occured in CArchiveDB::BundleDelete().") );
	}
	return bSuccess;
}


// (1) If there is not the "ArcLog" table in the DB, create this table.
// LATER: Actually, you should use CheckIsTableInDB().
//==============================================================================
void CArchiveDB::CheckLogTable( ADOX::_CatalogPtr pCatalog )
{
//	bool bSuccess = false;
	bool bLogTableFound = false;
  HRESULT hr = S_OK;

// Get "ArcLog" Table
	ADOX::_TablePtr pTable = NULL;
  TESTHR( hr = pTable.CreateInstance(__uuidof(ADOX::Table)) );
  _variant_t vIndex;
  for (long lIndex = 0; lIndex < pCatalog->Tables->Count; lIndex ++)
  {
		vIndex = lIndex;
    pTable = pCatalog->Tables->GetItem( vIndex );
		CString strCurTableName = pTable->Name.GetBSTR(); // Was (LPCSTR)pTable->Name;
    if( strCurTableName == "ArcLog" )
		{
			bLogTableFound = true;
			break;
		}
  }

	if( ! bLogTableFound )
	// "ArcLog" table not found, create this table
	{
		CreateArcLogTable( pCatalog );
	}	
//	return bSuccess;
}


void CArchiveDB::CreateArcLogTable( ADOX::_CatalogPtr pCatalog )
//==============================================================================
{
// try block is in the calling procedure


/* (12) disabled   
//	Make a copy of DB
	CopyFile( m_strDBFilename, m_strDBFilename + ".bakLog",
			  FALSE/*Overwrite existing* / );
*/

	ADOX::_TablePtr pTable = NULL;
    HRESULT hr = S_OK;
    TESTHR( hr = pTable.CreateInstance(__uuidof(ADOX::Table)) );
    pTable->PutName( "ArcLog" );
    pTable->Columns->Append( "MessDateTime",	ADOX::adDate,	 0 );
    pTable->Columns->Append( "Path",			ADOX::adWChar, 255/*_MAX_DIR=256 - it's too long*/ );
    pTable->Columns->Append( "Filename",		ADOX::adWChar, 255/*_MAX_DIR=256 - it's too long*/ );
    pTable->Columns->Append( "Message",			ADOX::adWChar, 255 );
	pCatalog->Tables->Append( _variant_t((IDispatch *)pTable) );
}


// (2) Check is there "IsSystem" field in "FilesToArchive" table.
// try block is in the calling procedure
//==============================================================================
void CArchiveDB::CheckIsSystemField( ADOX::_CatalogPtr pCatalog )
{
	bool bSystemFieldFound = false;
	ADOX::_TablePtr pTable = NULL;
	pTable = pCatalog->Tables->Item["FilesToArchive"];

	bSystemFieldFound = FindField( pTable, "IsSystem" );
	if( ! bSystemFieldFound )
	// "IsSystem" field not found, add it
	{
	// Write to Log
		g_TheArchive.m_LogFile.AddRecord( "DATABASE", "UPDATING", 
										                  "Adding IsSystem field" );
/* (12) disabled   
	// Make a copy of DB
		CopyFile( m_strDBFilename, m_strDBFilename + ".bakIsSys",
				  FALSE/*Overwrite existing* / );
*/
		pTable->Columns->Append( "IsSystem", ADOX::adBoolean, 0 );
			// Initial value of IsSystem will be FALSE

	// Write to Log
		g_TheArchive.m_LogFile.AddRecord( "DATABASE", "UPDATING", 
										                  "IsSystem field added" );
	}	
}


// (2) Modify structure of the DB if needed.
//==============================================================================
void CArchiveDB::DBStructureModifications()
{
    HRESULT hr = S_OK;
	ADOX::_CatalogPtr pCatalog = NULL;
	TESTHR(hr = pCatalog.CreateInstance(__uuidof (ADOX::Catalog)));
	pCatalog->PutActiveConnection( m_pConnection );

// (1) If there is not the "ArcLog" table in the DB, create this table
//=====================================================================
	CheckLogTable( pCatalog );

// If there is not the "IsSystem" field in "FileToArc" table, add this field
//===============================================================================
	CheckIsSystemField( pCatalog );

// (4) Check is there a DeleteIt field in the FileCopies table
//========================================================================
	CheckDeleteItField( pCatalog );

}


// (8) Additional DB structure modifications
//==============================================================================
void CArchiveDB::DBStructureModifications2()
{
    HRESULT hr = S_OK;
	ADOX::_CatalogPtr pCatalog = NULL;
	TESTHR(hr = pCatalog.CreateInstance(__uuidof (ADOX::Catalog)));
	pCatalog->PutActiveConnection( m_pConnection );

// If there is not the "FoldersToArc" table in the DB, create this table
//=======================================================================
	CheckFoldersTable( pCatalog );

// Check in "FilesToArchive" table "FolderID" and "Paused" fields.
// We will not make a copy of DB because it has just been made in CheckFoldersTable.
//===================================================================================
	CheckFilesTableFolderAndPausedFields( pCatalog );

// (12) If there is not the "Options" table in the DB, create this table.
//============================================================================
	CheckOptionsTable( pCatalog );

// (17) Check are there the new fields attended for compressing.
//============================================================================
	CheckFieldsForCompress( pCatalog );
}


// (3) Check is there a record about the Database in the FilesToArc table
//==============================================================================
void CArchiveDB::CheckRecordAboutDB()
{
	CFileToArc theDBfile( m_strDBFilename );
	theDBfile.getInfo();	// (10) It will change filename's and 
		// path's capitalization according to current file's info on disk 
	if( ! CheckIsFileInArc( &theDBfile ) )
	{
		theDBfile.m_nPriority = -1;		// (17) "Not Used"   LATER
		theDBfile.m_nUpToCopies = g_TheArchive.m_nDefaultCopies;	// (17)
		theDBfile.m_nFolderID = 0;		// (17)
		theDBfile.m_bPaused = false;	// (17)
		theDBfile.m_bSystem = true;
		FileAdd( &theDBfile );
	}
}


// Check is this file already in the Archive
//==============================================================================
bool CArchiveDB::CheckIsFileInArc( CFileToArc* pFile )
{
	bool bFileAlreadyInDB = false;
	_RecordsetPtr rstFiles;
	rstFiles.CreateInstance(__uuidof(Recordset)); 
	rstFiles->CursorType = adOpenStatic;
	HRESULT hr;
	try
	{
	// Select Files with this filename - their number must be JUST ONE
	// Will use \" symbol, because ' symbol can be found in filenames!
	// Be careful, there can be spaces in filenames!
    CString select = L"SELECT * FROM FilesToArchive WHERE Computer=\""
                   + pFile->m_strComputer;
    select += L"\" AND Drive=\"" + pFile->m_strDrive;
    select += L"\" AND Dir=\"" + pFile->m_strDir;
    select += L"\" AND Name=\"" + pFile->m_strName + L"\"";
    bstr_t converted = select;
    hr = rstFiles->Open( converted, m_pConnection, adOpenStatic,
                         adLockReadOnly, adCmdText );
    TESTHR( hr );

		long rCount = rstFiles->GetRecordCount();
		if( rCount == 0 )
			bFileAlreadyInDB = false;
		else
			bFileAlreadyInDB = true;
	}
	catch(_com_error &e)
	{
	// Notify the user of errors if any
		ShowADOErrors( e, m_pConnection );
	}
	catch(...)
	{
		AfxMessageBox( L"Some error occured in CArchiveDB::CheckIsFileInArc()." );
	}

	rstFiles->Close();

	return bFileAlreadyInDB;
}


// (4) Check is there "DeleteIt" field in "FileCopies" table.
// try block is in the calling procedure
//==============================================================================
void CArchiveDB::CheckDeleteItField( ADOX::_CatalogPtr pCatalog )
{
	ADOX::_TablePtr pTable = NULL;
	pTable = pCatalog->Tables->Item["FileCopies"];

	bool bFieldFound = FindField( pTable, "DeleteIt" );
	if( ! bFieldFound )
	// "DeleteIt" field not found, add it
	{
	// Write to Log
		g_TheArchive.m_LogFile.AddRecord( "DATABASE", "UPDATING", 
										                  "Adding DeleteIt field" );
/* (12) disabled   
	// Make a copy of DB
		CopyFile( m_strDBFilename, m_strDBFilename + ".bakDelIt",
				  FALSE/*Overwrite existing* / );
*/
		pTable->Columns->Append( "DeleteIt", ADOX::adBoolean, 0 );
			// Initial value of DeleteIt will be FALSE
	// Write to Log
		g_TheArchive.m_LogFile.AddRecord( "DATABASE", "UPDATING", 
								                  		"DeleteIt field added" );
	}	
}


// (8) If there is not the "FoldersToArc" table in the DB, create this table.
// try block is in the calling procedure
//==============================================================================
void CArchiveDB::CheckFoldersTable( ADOX::_CatalogPtr pCatalog )
{
	bool bFoldersTableFound = CheckIsTableInDB( pCatalog, "FoldersToArc" );
	if( ! bFoldersTableFound )
	// "FoldersToArc" table not found, create this table
	{
	// Write to Log
		g_TheArchive.m_LogFile.AddRecord( "DATABASE", "UPDATING", 
								                  		"Adding FoldersToArc Table" );
/* (12) disabled   
	// Make a copy of DB
		CopyFile( m_strDBFilename, m_strDBFilename + ".bakFolders",
				  FALSE/*Overwrite existing* / );
*/
		ADOX::_TablePtr pTable = NULL;
		HRESULT hr = S_OK;
	    TESTHR( hr = pTable.CreateInstance(__uuidof(ADOX::Table)) );
	    pTable->PutName( "FoldersToArc" );
        pTable->Columns->Append( "FolderID",	 ADOX::adInteger, 0 );

	// This group of field is the same as in FilesToArchive table
		pTable->Columns->Append( "Computer", ADOX::adWChar,   MAX_COMPUTERNAME_LENGTH );
        pTable->Columns->Append( "Drive",	 ADOX::adWChar,   15/*_MAX_DRIVE=3 - it's not enough*/);
        pTable->Columns->Append( "Dir",		 ADOX::adWChar,   255/*_MAX_DIR=256 - it's too long*/ );
        pTable->Columns->Append( "Name",	 ADOX::adWChar,   255/*_MAX_FNAME=256 - it's too long*/ );
			// Including extension
        pTable->Columns->Append( "UpToCopies", ADOX::adInteger, 0);

		pTable->Columns->Append( "Masks",		 ADOX::adWChar, nFolderMasksLen );
		pTable->Columns->Append( "ExcludeMasks", ADOX::adWChar, nFolderMasksLen );
        pTable->Columns->Append( "IncludeSubfolders", ADOX::adBoolean, 0 );
		pCatalog->Tables->Append( _variant_t((IDispatch *)pTable) );

		g_TheArchive.m_LogFile.AddRecord( "DATABASE", "UPDATING", 
								                      "FoldersToArc Table added" );
	}	
}


// (4) Check is there a field with a given name in the given table
//==============================================================================
bool CArchiveDB::FindField( ADOX::_TablePtr pTable, CString strFieldName )
{
	bool bFieldFound = false;
  _variant_t vIndex;
	ADOX::ColumnsPtr pColumns = pTable->Columns;
	ADOX::_ColumnPtr pCurColumn = NULL;
  for ( long lIndex = 0; lIndex < pColumns->Count; lIndex ++ )
  {
		vIndex = lIndex;
    pCurColumn = pColumns->GetItem( vIndex );
		CString strCurColumnName = pCurColumn->Name.GetBSTR(); // Was (LPCSTR)pCurColumn->Name;
    if( strCurColumnName == strFieldName )
		{
			bFieldFound = true;
			break;
		}
  }
	return bFieldFound;
}


// (8) Returns true, if there is a table with given name in the database
//==============================================================================
bool CArchiveDB::CheckIsTableInDB( ADOX::_CatalogPtr pCatalog,
                                   CString strTableName )
{
	bool bTableFound = false;
	ADOX::_TablePtr pTable = NULL;
  _variant_t vIndex;
  for (long lIndex = 0; lIndex < pCatalog->Tables->Count; lIndex ++)
  {
		vIndex = lIndex;
    pTable = pCatalog->Tables->GetItem( vIndex );
		CString strCurTableName = pTable->Name.GetBSTR(); // Was (LPCSTR)pTable->Name;
    if( strCurTableName == strTableName )
		{
			bTableFound = true;
			break;
		}
  }
	return bTableFound;
}


// (4) Check is there "FolderID" and "Paused" fields in "FilesToArchive" table.
// try block is in the calling procedure
//===============================================================================
void CArchiveDB::CheckFilesTableFolderAndPausedFields( ADOX::_CatalogPtr pCatalog )
{
	ADOX::_TablePtr pTable = NULL;
	pTable = pCatalog->Tables->Item["FilesToArchive"];

	if( ! FindField( pTable, "FolderID" ) )
	// "FolderID" field not found, add it
	//=====================================
	{
	// Write to Log
		g_TheArchive.m_LogFile.AddRecord( "DATABASE", "UPDATING", 
								                  		"FToA: Adding FolderID field" );
		pTable->Columns->Append( "FolderID", ADOX::adInteger, 0 );
// TO DO:zz			// Initial value of DeleteIt will be FALSE
		g_TheArchive.m_LogFile.AddRecord( "DATABASE", "UPDATING", 
								                   		"FolderID field added" );
	}	

	if( ! FindField( pTable, "Paused" ) )
	// "Paused" field not found, add it
	//=====================================
	{
	// Write to Log
		g_TheArchive.m_LogFile.AddRecord( "DATABASE", "UPDATING", 
										                  "FToA: Adding Paused field" );
		pTable->Columns->Append( "Paused", ADOX::adBoolean, 0 );
			// Initial value of DeleteIt will be FALSE
		g_TheArchive.m_LogFile.AddRecord( "DATABASE", "UPDATING", 
								                  		"Paused field added" );
	}	
}


// (8) Load folders list from DB into memory.
//============================================
bool CArchiveDB::FoldersLoad()
{
	bool bSuccess = false;

	_RecordsetPtr rsFolders;
	rsFolders.CreateInstance(__uuidof(Recordset)); 
	HRESULT hr;
	try
	{
	// Select all Folders
		CString select = L"SELECT * FROM FoldersToArc";
// LATER: ORDER BY
		bstr_t converted = select;
		hr = rsFolders->Open( converted, m_pConnection, adOpenStatic,
							  adLockReadOnly, adCmdText );
    TESTHR( hr );

		if( ! rsFolders->adoEOF )
			rsFolders->MoveFirst();	// ??? Is not necessary?
		while( ! rsFolders->adoEOF )
		{
			_bstr_t  bstrTmp; // Temporary string for type conversion
			_variant_t vtTmp;
			CString strTmp;
      bstrTmp = rsFolders->Fields->Item["FolderIDlename"]->Value;
			strTmp = bstrTmp.GetBSTR(); // Was (LPCSTR)bstrTmp;
			strTmp.TrimRight();
			CFolderToArc *pCurFolder = new CFolderToArc();
				// It will be freed in CMyArchive destructor

      vtTmp = rsFolders->Fields->Item["FolderID"]->Value;
      pCurFolder->m_nFolderID = (long)vtTmp;


		// This group of fields is the same as in FilesToArchive table
		//------------------------------------------------------------
      bstrTmp = rsFolders->Fields->Item["Computer"]->Value;
			pCurFolder->m_strComputer = bstrTmp.GetBSTR(); // Was (LPCSTR)bstrTmp;
			pCurFolder->m_strComputer.TrimRight();
            
      bstrTmp = rsFolders->Fields->Item["Drive"]->Value;
			pCurFolder->m_strDrive = bstrTmp.GetBSTR(); // Was (LPCSTR)bstrTmp;
			pCurFolder->m_strDrive.TrimRight();

      bstrTmp = rsFolders->Fields->Item["Dir"]->Value;
			pCurFolder->m_strDir = bstrTmp.GetBSTR(); // Was (LPCSTR)bstrTmp;
			pCurFolder->m_strDir.TrimRight();

      bstrTmp = rsFolders->Fields->Item["Name"]->Value;
			pCurFolder->m_strName = bstrTmp.GetBSTR(); // Was (LPCSTR)bstrTmp;
			pCurFolder->m_strName.TrimRight();

      vtTmp = rsFolders->Fields->Item["UpToCopies"]->Value;
      pCurFolder->m_nUpToCopies = (long)vtTmp;
		//--end of the group-------------------------------------------

            
			bstrTmp = rsFolders->Fields->Item["Masks"]->Value;
			pCurFolder->m_strMasks = bstrTmp.GetBSTR(); // Was (LPCSTR)bstrTmp;
			pCurFolder->m_strMasks.TrimRight();

			bstrTmp = rsFolders->Fields->Item["ExcludeMasks"]->Value;
			pCurFolder->m_strExcludeMasks = bstrTmp.GetBSTR(); // Was (LPCSTR)bstrTmp;
			pCurFolder->m_strExcludeMasks.TrimRight();

      vtTmp = rsFolders->Fields->Item["UpToCopies"]->Value;
      pCurFolder->m_nUpToCopies = (long)vtTmp;

      vtTmp = rsFolders->Fields->Item["IncludeSubfolders"]->Value;
      pCurFolder->m_bIncludeSubfolders = (bool)vtTmp;

			m_pArchive->m_FoldersToArc.AddTail( pCurFolder );
			rsFolders->MoveNext();
		}
		rsFolders->Close();

		bSuccess = true;
	}
	catch(_com_error &e)
	{
	// Notify the user of errors if any
		ShowADOErrors( e, m_pConnection );
	}
	catch(...)
	{
		AfxMessageBox( L"Some error occured in CArchiveDB::FoldersLoad()." );
	}

	return bSuccess;
}


// (12) If there is not the "Options" table in the DB, create this table.
// try block is in the calling procedure
//============================================================================
void CArchiveDB::CheckOptionsTable( ADOX::_CatalogPtr pCatalog )
{
	bool bOptionsTableFound = CheckIsTableInDB( pCatalog, "ProgramOptions" );
	if( ! bOptionsTableFound )
	// "Options" table not found, create this table
	{
	// Write to Log
		g_TheArchive.m_LogFile.AddRecord( "DATABASE", "UPDATING", 
								  		                "Adding Options Table" );
		ADOX::_TablePtr pTable = NULL;
		HRESULT hr = S_OK;
    TESTHR( hr = pTable.CreateInstance(__uuidof(ADOX::Table)) );
    pTable->PutName( "ProgramOptions" );

		pTable->Columns->Append( "SectionName",  ADOX::adWChar,  15 );
		pTable->Columns->Append( "OptionName",   ADOX::adWChar,  15 );
    pTable->Columns->Append( "OptionValue",  ADOX::adWChar, 255 );
    pTable->Columns->Append( "OptionValue2", ADOX::adWChar, 255 );

		pCatalog->Tables->Append( _variant_t((IDispatch *)pTable) );

		LocatorRestoreDefaultOptions( 99 );	// LATER: Add here other defaults
		
		g_TheArchive.m_LogFile.AddRecord( "DATABASE", "UPDATING", 
  										                "Options Table added" );
	}	
}


// (17) Write an option value into DB
//==============================================================================
bool CArchiveDB::optionSave( CString sSection, CString sOptionName,
						                 CString sValue, CString sValue2 )
{
	bool bSuccess=false;
	try
	{
	// Delete existing record from DB
		CString sCmd = "DELETE FROM ProgramOptions"
					         " WHERE SectionName='" + sSection + "'";
		sCmd += " AND OptionName='" + sOptionName + "'";
	  bSuccess = g_pTheDB->ExecSQL( sCmd );

	// Write the new option value into DB 
		if( bSuccess )
		{
			CString cmd;
			cmd.Format( _T("INSERT INTO ProgramOptions\n")
				        _T(" ( SectionName, OptionName, OptionValue, OptionValue2 )\n")
						_T(" VALUES ( '%s', '%s', '%s', '%s' )"),
						sSection, sOptionName, sValue, sValue2 );
			bSuccess = g_pTheDB->ExecSQL( cmd );
		}	
	}
	catch(_com_error &e)
	{
	// Notify the user of errors if any
		ShowADOErrors( e, g_pTheDB->m_pConnection );
	}
	catch(...)
	{
		AfxMessageBox( _T("Some error occured\n")
					         _T(" in CArchiveDB::::OptionSave().") );
	}
	return bSuccess;
}


// (18) Read an option value from the DB.
// Returns true if the required option was found. The value is passed through
//   o_sValue parameter.
//==============================================================================
bool CArchiveDB::optionRead( CString i_sSection, CString i_sOptionName, 
                             CString& o_sValue )
{
	bool bSuccess = false;

	_RecordsetPtr rsOptions;
	rsOptions.CreateInstance(__uuidof(Recordset)); 
	HRESULT hr;
	try
	{
	// Select all Options
		CString select = "SELECT * FROM ProgramOptions"
			" WHERE SectionName=\"Archive\" AND OptionName=\"" + i_sOptionName + "\"";
		bstr_t converted = select;
		hr = rsOptions->Open( converted, g_pTheDB->m_pConnection, adOpenStatic,
							            adLockReadOnly, adCmdText );
    TESTHR( hr );
		if( ! rsOptions->adoEOF )
		{
			_bstr_t  bstrTmp; // Temporary string for type conversion
			_variant_t vtTmp;
			
      bstrTmp = rsOptions->Fields->Item["OptionValue"]->Value;
			o_sValue = bstrTmp.GetBSTR(); // Was (LPCSTR)bstrTmp;
			o_sValue.TrimRight();

      bSuccess = true;
		}
		rsOptions->Close();
	}
	catch(_com_error &e)
	{
	// Notify the user of errors if any
		ShowADOErrors( e, g_pTheDB->m_pConnection );
	}
	catch(...)
	{
		AfxMessageBox( _T("Some error occured in CMyArchive::optionRead().") );
	}

	return bSuccess;
}


//== Locator ================================================================
// (12)
void CArchiveDB::LocatorRestoreDefaultOptions( int nOptionsSection )
{
// LATER: Use CArchiveDB::OptionSave()
// LATER: Move to the Locator respective class

// Default Values
  wchar_t* fileTypes[]   = { L"log", L"cab", L"lnk", L"bak", L"tmp", 
                             L"swp", L"mab", NULL };
  wchar_t* folderNames[] = { L"Cookies", L"Temporary Internet Files", L"Recent",
                             L"System Volume Information",
                             L"RECYCLED", L"RECYCLER", NULL };

  if(    nOptionsSection == 44/* Locator's Options */
      || nOptionsSection == 99/* All Options */ )
  {
    bool bOk = ExecSQL( "DELETE FROM ProgramOptions"
                        " WHERE SectionName=\"Locator\"" ); 
            // (14) Was: OptionName
    if( bOk )
      for( int i=0; fileTypes[i] != NULL; i++ )
      {
        CString cmd;
        cmd.Format( L"INSERT INTO ProgramOptions"
                    L" (SectionName, OptionName, OptionValue, OptionValue2) "
                    L" VALUES (\"Locator\", \"exclFileType\", \"%s\", \"\" )",
                    fileTypes[i] );
        bOk = ExecSQL( cmd );
        if( ! bOk )
          break;
      }
    if( bOk )
      for( int i=0; folderNames[i] != NULL; i++ )
      {
        CString cmd;
        cmd.Format( L"INSERT INTO ProgramOptions"
                    L" (SectionName, OptionName, OptionValue, OptionValue2) "
                    L" VALUES (\"Locator\", \"exclFolder\", \"%s\", \"\" )",
                    folderNames[i] );
        bOk = ExecSQL( cmd );
        if( ! bOk )
          break;
      }
  }
}


// (17)
// Check are there the new fields attended for compressing.
// try block is in the calling procedure.
//-----------------------------------------------------------------------------
void CArchiveDB::CheckFieldsForCompress( ADOX::_CatalogPtr pCatalog )
{

// "FilesToArchive" table, "CompressIt" field
//-----------------------------------------------------------------------------
	ADOX::_TablePtr pFToA_Table = pCatalog->Tables->Item["FilesToArchive"];
	if( ! FindField( pFToA_Table, "CompressIt" ))
	// The field not found, add it
	{
	// Write to Log
		g_TheArchive.m_LogFile.AddRecord( "DATABASE", "UPDATING", 
								                  		"FToA: Adding CompressIt field" );
		pFToA_Table->Columns->Append( "CompressIt", ADOX::adBoolean, 0 );
								// Initial value of CompressIt will be FALSE
		g_TheArchive.m_LogFile.AddRecord( "DATABASE", "UPDATING", 
								                  		"CompressIt field added" );
	}	

// "Rooms" table, "CompressionMode" field
//-----------------------------------------------------------------------------
	ADOX::_TablePtr pRoomTable = pCatalog->Tables->Item["Rooms"];
	if( ! FindField( pRoomTable, "CompressionMode" ))
	// Field not found, add it
	{
	// Write to Log
		g_TheArchive.m_LogFile.AddRecord( "DATABASE", "UPDATING", 
								                  		"Rooms: Adding CompressionMode field" );
		pRoomTable->Columns->Append( "CompressionMode", ADOX::adInteger, 0 );
									// Initial value will be NULL! Be careful
		g_TheArchive.m_LogFile.AddRecord( "DATABASE", "UPDATING", 
								                  		"CompressionMode field added" );
	}
}


//== Common database procedures ==============================================
// (12)
bool CArchiveDB::ExecSQL( CString cmd )
{
	bool bSuccess = false;
	try
	{
		bstr_t converted = cmd;
		((_ConnectionPtr)m_pConnection)->Execute( converted, NULL, NULL );
		bSuccess = true;
	}
	catch(_com_error &e)
	{
	// Notify the user of errors if any
		ShowADOErrors( e, m_pConnection, cmd );
	}
	catch(...)
	{
		AfxMessageBox( "Some error occured in CArchiveDB::ExecuteSQL().\n" 
			         + cmd );
	}
	return bSuccess;
}


