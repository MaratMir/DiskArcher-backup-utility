// DiskArcher.
// CArchiveDB class - the interface to the program database.
// (C) Marat Mirgaleev, 2001-2014.
// Modifications:
//  (1) 15.01.2002. Log Table added.
//  (2) 04.02.2002. FilesToArc: Field "IsSystem" added.
//  (3) 09.02.2002. FilesToArc: A record about the Database.
//  (4) 10.02.2002. IsOpen() added.
//                  FileCopies: Field "DeleteIt" added.
//  (5) 11.02.2002. CopyDelete() added.
//  (6) 18.02.2002. FileDelete() added.
//  (7) 20.02.2002. BundleDelete() and RoomDelete() added.
//  (8) 03.03.2002. "FoldersToArc" table added.
//                  In "FilesToArchive" table "FolderID" and "Paused" 
//                  fields added.
//  (9) 07.03.2002. FileUpdate() added.
// (10) 09.04.2002. Correcting error in path capitalization
//                  when the DB file is adding to FilesToArc.
// (11) 09.05.2002. CoInitialize() and CoUninitialize() moved out from here.
//                  FilesLoad() - ORDER BY FileName.
// (12) 15.10.2002. Creating of DB copies before structure modifications 
//                  disabled.
//                  Table "Options" added.
// (13) 12.11.2002. Files Order changed.
// (14) 26.01.2003. Error in LocatorRestoreDefaultOptions() fixed.
// (15) 12.03.2003. More detailed error tracking in Create().
// (16) 06.04.2003. Corrections in (15).
// (17) 11.05-20.11.2003. Compression.
// (18) 02.05.2006. ADO compilation warning suppressed.
//                  optionRead() added.
// (19) 23.12.2013. Migration to VS 2012 and Unicode.
//                  See the latest modification history on Github.
//======================================================================================

#include "stdafx.h"
#include "CArchiveDB.h"
#include "CFileToArc.h"
#include "CFilesToArc.h"
#include "CMyArchive.h"
#include "CRoom.h"

#include "CFileCopy.h"	// M
#include "CBundle.h"	// M
#include "Miscelaneous.h"	// (2)
#include "CFolderToArc.h"	// (8)
#include "CZipBundle.h"	// M

const CString CArchiveDB::MyDBFilename = L"MArc.MDB";


// Create the Database and its Tables.
// This code was copied from MSDN Jan 2000 - Platform SDK 
//	- Data Access Services - MDAC SDK - ADO - ADOX Reference 
//	- ADOX API Ref. - ADOX Code Examples - VC++ - Create Method.
//==================================================================
bool CArchiveDB::Create()
{
// LATER: Wait cursor/ Progress bar
  CString step = L"0";		// (15)
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
  _bstr_t strcnn( L"Provider=Microsoft.JET.OLEDB.4.0;"
// LATER: move this strings into constants or resources
        // Can add: "Locale Identifier=1049;" Russian or 1033 USA
                  L"Data source = " + getDBfilename() );
	try
	{
		step = L"10";	  // (15)
		hr = pCatalog.CreateInstance(__uuidof (ADOX::Catalog));
		step = L"15";      // (16)
		TESTHR( hr );
		step = L"20";      // (15)
		pCatalog->Create( strcnn );
		step = L"25";      // (16)
		m_pConnection = pCatalog->GetActiveConnection();	// (16)
		bSuccess = true;  // (15)
	}   
	catch(_com_error &e)
	{
    showADOErrors( e );
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
		step = L"30";	  // (15)
		ADOX::_TablePtr pTable = NULL;
    try
    {
		// Table "Files To Archive"
		//=================================
			step = L"40";	  // (15)
      hr = pTable.CreateInstance(__uuidof(ADOX::Table));
      TESTHR( hr );
			step = L"50";	  // (15)
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
			step = L"70";	  // (15)
			pCatalog->Tables->Append( _variant_t((IDispatch *)pTable) );

		// Table "Archive Rooms"
		//=================================
			step = L"80";	  // (15)
      hr = pTable.CreateInstance(__uuidof(ADOX::Table));
      TESTHR( hr );
			step = L"90";	  // (15)
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
	// adInteger is 32-bit long, so the limit of size is 4 Gb!!! LATER: Add another field for higher bits.
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
      step = L"200";  // (15)
			CreateArcLogTable( pCatalog );
	
			bSuccess = true;  // (15)
    }
	  catch(_com_error &e)
		{
      showADOErrors( e, m_pConnection );
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


// Open the Database
//==============================================================================
bool CArchiveDB::Open()
{
  bool bSuccess = true;

  _ConnectionPtr pConnection    = NULL;
  // Set Connection string
  _bstr_t strCnn( L"Provider=Microsoft.JET.OLEDB.4.0;"
                  L"Data source = " + getDBfilename() );
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
    showADOErrors( e );
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
        pFile->m_nPriority, pFile->m_nUpToCopies, trueFalse( pFile->m_bSystem ),
        pFile->m_nFolderID, trueFalse( pFile->m_bPaused ) );
		  bstr_t converted = cmd;
			((_ConnectionPtr)m_pConnection)->Execute( converted, NULL, NULL );
			bSuccess = true;
		}
    catch(_com_error &e)
    {
    // Notify the user of errors if any
      showADOErrors( e, m_pConnection );
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
    trueFalse( pFile->m_bPaused ), trueFalse( pFile->m_bCompressIt ),
		pFile->m_strComputer, pFile->m_strDrive, pFile->m_strDir, 
		pFile->m_strName );
  bSuccess = execSQL( cmd );

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
    showADOErrors( e, m_pConnection );
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
    CString select = L"SELECT * FROM FilesToArchive "
                     L"ORDER BY Drive, Dir, Name";
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
    showADOErrors( e, m_pConnection );
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
			trueFalse( pRoom->m_bRemovable ), pRoom->m_sizeLimit >> 20 /* To Megabytes */,
			pRoom->m_nDiskSpaceFree, pRoom->m_nCompressionMode );
		bstr_t converted = cmd;
		((_ConnectionPtr)m_pConnection)->Execute( converted, NULL, NULL );
		bSuccess = true;
	}
	catch(_com_error &e)
  { // Notify the user of errors if any
    showADOErrors( e, m_pConnection );
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
    CString select = L"SELECT * FROM Rooms";  // LATER: ORDER BY
    bstr_t converted = select;
    hr = rsRooms->Open( converted, m_pConnection, adOpenStatic, adLockReadOnly, adCmdText );
    TESTHR( hr );

// (17) if( ! rsRooms->adoEOF )
// (17)   rsRooms->MoveFirst();	// ? Is not necessary?
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
      pCurRoom->m_sizeLimit = (long)vtTmp;
      pCurRoom->m_sizeLimit = pCurRoom->m_sizeLimit << 20; // It was saved in Megabytes, return to Bytes

      vtTmp = rsRooms->Fields->Item["SpaceFree"]->Value;
      pCurRoom->m_nDiskSpaceFree = (long)vtTmp;
      pCurRoom->m_nDiskSpaceFree = pCurRoom->m_nDiskSpaceFree << 10; // LATER ???

      vtTmp = rsRooms->Fields->Item["CompressionMode"]->Value;
      if( vtTmp.vt == VT_NULL )  // The value not assigned
        pCurRoom->m_nCompressionMode = CRoom::roomCompressionMode::rcmAllowed;
      else
      {
        int compMode = static_cast<int>(vtTmp);
        if(    ( compMode < 0 )
            || ( compMode >= CRoom::roomCompressionMode::rcmCount ) )
          // A wrong value
          pCurRoom->m_nCompressionMode = CRoom::roomCompressionMode::rcmAllowed;
        else
          pCurRoom->m_nCompressionMode = static_cast<CRoom::roomCompressionMode>(compMode);
      }
      m_pArchive->m_Rooms.m_rooms.push_back( pCurRoom );

      rsRooms->MoveNext();
    }
    rsRooms->Close();

    bSuccess = true;
  }
  catch(_com_error &e)
  {
    // Notify the user of errors if any
    showADOErrors( e, m_pConnection, L"Place: RoomsLoad" );
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
		_T("SET FileName=\"%s\", Removable=%s, SizeLimit=%I64u, SpaceFree=%I64u, ")
			_T("CompressionMode=%d ")
		_T("WHERE RoomID=%d"),
		pRoom->getFullName(), trueFalse( pRoom->m_bRemovable ),
		pRoom->m_sizeLimit >> 20 /*To Megabytes*/, pRoom->m_nDiskSpaceFree >> 10/*To Kilobytes*/,
		static_cast<int>(pRoom->m_nCompressionMode),
		pRoom->m_nRoomID );
  bSuccess = execSQL( cmd );
	return bSuccess;
}


// Get max Room ID from Database.
// Returns -1 if any error occurs
//==============================================================================
int CArchiveDB::RoomGetMaxID()
{
  int nMax = getMaxID( L"Rooms", L"RoomID" );
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
    showADOErrors( e, m_pConnection );
	}
	catch(...)
	{
		AfxMessageBox( _T("Some error occured in CArchiveDB::RoomDelete().") );
	}
	return bSuccess;
}


// Update the Copy info in the Database.
//==============================================================================
bool CArchiveDB::CopyUpdate(CFileCopy *pCopy)
{
  bool bSuccess = false;
  CString cmd;
  try
  { // Update record
    CString dt = pCopy->m_FileDateTime.Format();
    cmd.Format( 
      L"UPDATE FileCopies SET Path=\"%s\", FileName=\"%s\", UserID=\"%s\", FileDateTime=\"%s\", "
             L"SourceSizeHiBits=%d, SourceSize=%d, PackedSizeHiBits=%d, PackedSize=%d, BundleID=%d, DeleteIt=%s "
      L"WHERE CopyID=%d",
      pCopy->m_strPath, pCopy->m_strFilename, pCopy->m_strUser, dt,
      pCopy->getSizeHi(), pCopy->getSizeLow(), pCopy->getPackedSizeHi(), pCopy->getPackedSizeLow(),
      pCopy->m_nBundleID, trueFalse( pCopy->m_bDeleteIt ), pCopy->m_nCopyID );
    bstr_t converted = cmd;
    ((_ConnectionPtr)m_pConnection)->Execute( converted, NULL, NULL );
    bSuccess = true;
  }
  catch(_com_error &e)
  {
    showADOErrors( e, m_pConnection );
  }
  catch(...)
  {
    AfxMessageBox( _T("Some error occured in CArchiveDB::CopyUpdate().") );
  }
  return bSuccess;
}


// Add the Copy to DB.
// LATER: Make a generic Add function?
//==============================================================================
bool CArchiveDB::CopyAdd( CFileCopy *pCopy )
{
  bool bSuccess = false;

// Get max Copy ID
  int maxID = CopyGetMaxID();
  ASSERT( maxID != -1 ); // LATER: Message?
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
      //==================================================
      CString cmd, dt = pCopy->m_FileDateTime.Format();
      cmd.Format( L"INSERT INTO FileCopies (CopyID, Path, Filename, UserID, FileDateTime, "
                  L"   SourceSizeHiBits, SourceSize, PackedSizeHiBits, PackedSize, BundleID)"
                  L" VALUES (%d, \"%s\", \"%s\", \"%s\", \"%s\", %d, %d, %d, %d, %d)",
                  pCopy->m_nCopyID, pCopy->m_strPath, pCopy->m_strFilename, pCopy->m_strUser, dt, 
                  pCopy->getSizeHi(), pCopy->getSizeLow(), pCopy->getPackedSizeHi(), pCopy->getPackedSizeLow(),
                  pCopy->m_nBundleID );
      bstr_t converted = cmd;
      ((_ConnectionPtr)m_pConnection)->Execute( converted, NULL, NULL );

      bSuccess = true;
    }
    catch( _com_error &e )
    {
      showADOErrors( e, m_pConnection );
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
    showADOErrors( e, m_pConnection );
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
  int nMax = getMaxID( L"FileCopies", L"CopyID" );
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

      vtTmp = recSet->Fields->Item["SourceSizeHiBits"]->Value;
      int sizeHi = ( vtTmp.vt == VT_NULL ) ? 0 : (long)vtTmp;
      if( sizeHi < 0 ) // TODO: report it
        sizeHi = 0;
      vtTmp = recSet->Fields->Item["SourceSize"]->Value;
      int sizeLow = (long)vtTmp;
      if( sizeLow < 0 ) // TODO: report it
        sizeLow = 0;
      pCurCopy->setSize( sizeHi, sizeLow );

      vtTmp = recSet->Fields->Item["PackedSizeHiBits"]->Value;
      int packedSizeHi = ( vtTmp.vt == VT_NULL ) ? 0 : (long)vtTmp;
      if( packedSizeHi < 0 ) // TODO: report it
        packedSizeHi = 0;
      vtTmp = recSet->Fields->Item["PackedSize"]->Value;
      int packedSizeLow = (long)vtTmp;
      if( packedSizeLow < 0 ) // TODO
        packedSizeLow = 0;
      pCurCopy->setPackedSize( packedSizeHi, packedSizeLow );

			m_pArchive->m_Copies.AddTail( pCurCopy );
			recSet->MoveNext();

		}
		recSet->Close();

		bSuccess = true;
	}
	catch(_com_error &e)
  {
    showADOErrors( e, m_pConnection );
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
      fname = fname + L"\\" + bstrTmp.GetBSTR(); // Was (LPCSTR)bstrTmp;
			fname.TrimRight();

      vtTmp = recSet->Fields->Item["Type"]->Value;
			iTmp = (long)vtTmp;
			CBundle *pCurBundle = NULL;
			switch( iTmp )	// (17)
			{
      case CBundle::btSingleFile:
				pCurBundle = new CBundle( fname );
					// It will be deleted in CMyArchive destructor
				pCurBundle->m_strExtension = "MAB";
				break;
      case CBundle::btZipFile:
				pCurBundle = new CZipBundle( fname );
					// It will be deleted in CMyArchive destructor
				pCurBundle->m_strExtension = "ZIP";
				break;
			default:
        g_TheArchive.m_LogFile.AddRecord( L"Bundle", L"Loading", L"Unknown type of the Bundle" );
				continue;	// Impossible to go on with this record, go to next
			}
      pCurBundle->m_nType = (CBundle::bundleType)iTmp;
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
    showADOErrors( e, m_pConnection );
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
      showADOErrors( e, m_pConnection );
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
  int nMax = getMaxID( L"Bundles", L"BundleID" );
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
    showADOErrors( e, m_pConnection );
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

  bSystemFieldFound = doesFieldExist( pTable, L"IsSystem" );
	if( ! bSystemFieldFound )
	// "IsSystem" field not found, add it
	{
    g_TheArchive.m_LogFile.AddRecord( L"DATABASE", L"UPDATING", L"Adding IsSystem field" );
/* (12) disabled   
	// Make a copy of DB
		CopyFile( m_strDBFilename, m_strDBFilename + ".bakIsSys",
				  FALSE/*Overwrite existing* / );
*/
		pTable->Columns->Append( "IsSystem", ADOX::adBoolean, 0 );
			// Initial value of IsSystem will be FALSE

    g_TheArchive.m_LogFile.AddRecord( L"DATABASE", L"UPDATING", L"IsSystem field added" );
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

// Check there are the new fields for big numbers.
//============================================================================
	checkFileSizeIs64( pCatalog );
}


// (3) Check is there a record about the Database in the FilesToArc table
//==============================================================================
void CArchiveDB::CheckRecordAboutDB()
{
  CFileToArc theDBfile( getDBfilename() );
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
    showADOErrors( e, m_pConnection );
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

  bool bFieldFound = doesFieldExist( pTable, L"DeleteIt" );
	if( ! bFieldFound )
	// "DeleteIt" field not found, add it
	{
    g_TheArchive.m_LogFile.AddRecord( L"DATABASE", L"UPDATING", L"Adding DeleteIt field" );
/* (12) disabled   
	// Make a copy of DB
		CopyFile( m_strDBFilename, m_strDBFilename + ".bakDelIt",
				  FALSE/*Overwrite existing* / );
*/
		pTable->Columns->Append( "DeleteIt", ADOX::adBoolean, 0 );
			// Initial value of DeleteIt will be FALSE
    g_TheArchive.m_LogFile.AddRecord( L"DATABASE", L"UPDATING", L"DeleteIt field added" );
  }
}


// (8) If there is not the "FoldersToArc" table in the DB, create this table.
// try block is in the calling procedure
//==============================================================================
void CArchiveDB::CheckFoldersTable( ADOX::_CatalogPtr pCatalog )
{
  bool bFoldersTableFound = checkIfTableInDB( pCatalog, L"FoldersToArc" );
	if( ! bFoldersTableFound )
	// "FoldersToArc" table not found, create this table
	{
    g_TheArchive.m_LogFile.AddRecord( L"DATABASE", L"UPDATING", L"Adding FoldersToArc Table" );
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

    g_TheArchive.m_LogFile.AddRecord( L"DATABASE", L"UPDATING", L"FoldersToArc Table added" );
  }
}


// (4) Check is there "FolderID" and "Paused" fields in "FilesToArchive" table.
// try block is in the calling procedure
//===============================================================================
void CArchiveDB::CheckFilesTableFolderAndPausedFields( ADOX::_CatalogPtr pCatalog )
{
	ADOX::_TablePtr pTable = NULL;
	pTable = pCatalog->Tables->Item["FilesToArchive"];

  if( ! doesFieldExist( pTable, L"FolderID" ) )
  // "FolderID" field not found, add it
  //=====================================
  {
    g_TheArchive.m_LogFile.AddRecord( L"DATABASE", L"UPDATING", L"FToA: Adding FolderID field" );
    pTable->Columns->Append( "FolderID", ADOX::adInteger, 0 );
// TO DO:zz			// Initial value of DeleteIt will be FALSE
    g_TheArchive.m_LogFile.AddRecord( L"DATABASE", L"UPDATING", L"FolderID field added" );
  }

  if( ! doesFieldExist( pTable, L"Paused" ) )
  // "Paused" field not found, add it
  //=====================================
  {
    g_TheArchive.m_LogFile.AddRecord( L"DATABASE", L"UPDATING", L"FToA: Adding Paused field" );
    pTable->Columns->Append( "Paused", ADOX::adBoolean, 0 );
      // Initial value of DeleteIt will be FALSE
    g_TheArchive.m_LogFile.AddRecord( L"DATABASE", L"UPDATING", L"Paused field added" );
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
    showADOErrors( e, m_pConnection );
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
  bool bOptionsTableFound = checkIfTableInDB( pCatalog, L"ProgramOptions" );
	if( ! bOptionsTableFound )
	// "Options" table not found, create this table
	{
    g_TheArchive.m_LogFile.AddRecord( L"DATABASE", L"UPDATING", L"Adding Options Table" );
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
		
    g_TheArchive.m_LogFile.AddRecord( L"DATABASE", L"UPDATING", L"Options Table added" );
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
    CString sCmd = L"DELETE FROM ProgramOptions"
                   L" WHERE SectionName='" + sSection + L"'";
    sCmd += L" AND OptionName='" + sOptionName + L"'";
    bSuccess = execSQL( sCmd );

  // Write the new option value into DB 
    if( bSuccess )
    {
      CString cmd;
      cmd.Format( L"INSERT INTO ProgramOptions\n"
                  L" ( SectionName, OptionName, OptionValue, OptionValue2 )\n"
                  L" VALUES ( '%s', '%s', '%s', '%s' )",
                  sSection, sOptionName, sValue, sValue2 );
      bSuccess = execSQL( cmd );
    }
  }
  catch(_com_error &e)
  {
    showADOErrors( e, m_pConnection );
  }
  catch(...)
  {
    AfxMessageBox( L"Some error occured\n"
                   L" in CArchiveDB::::OptionSave()." );
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
    CString select = L"SELECT * FROM ProgramOptions"
                     L" WHERE SectionName=\"Archive\" AND OptionName=\"" + i_sOptionName + L"\"";
		bstr_t converted = select;
		hr = rsOptions->Open( converted, m_pConnection, adOpenStatic,
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
    showADOErrors( e, m_pConnection );
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
    bool bOk = execSQL( L"DELETE FROM ProgramOptions"
                        L" WHERE SectionName=\"Locator\"" ); 
            // (14) Was: OptionName
    if( bOk )
      for( int i=0; fileTypes[i] != NULL; i++ )
      {
        CString cmd;
        cmd.Format( L"INSERT INTO ProgramOptions"
                    L" (SectionName, OptionName, OptionValue, OptionValue2) "
                    L" VALUES (\"Locator\", \"exclFileType\", \"%s\", \"\" )",
                    fileTypes[i] );
        bOk = execSQL( cmd );
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
        bOk = execSQL( cmd );
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
  if( ! doesFieldExist( pFToA_Table, L"CompressIt" ))
  // The field not found, add it
  {
    g_TheArchive.m_LogFile.AddRecord( L"DATABASE", L"UPDATING", L"FToA: Adding CompressIt field" );
    pFToA_Table->Columns->Append( "CompressIt", ADOX::adBoolean, 0 );
                    // Initial value of CompressIt will be FALSE
    g_TheArchive.m_LogFile.AddRecord( L"DATABASE", L"UPDATING", L"CompressIt field added" );
  }

// "Rooms" table, "CompressionMode" field
//-----------------------------------------------------------------------------
  ADOX::_TablePtr pRoomTable = pCatalog->Tables->Item["Rooms"];
  if( ! doesFieldExist( pRoomTable, L"CompressionMode" ))
  // Field not found, add it
  {
    g_TheArchive.m_LogFile.AddRecord( L"DATABASE", L"UPDATING", L"Rooms: Adding CompressionMode field" );
    pRoomTable->Columns->Append( "CompressionMode", ADOX::adInteger, 0 );
              // Initial value will be NULL! Be careful
    g_TheArchive.m_LogFile.AddRecord( L"DATABASE", L"UPDATING", L"CompressionMode field added" );
  }
}


// Check that file sizes are big, big numbers.
// try block is in the calling procedure.
//-----------------------------------------------------------------------------
void CArchiveDB::checkFileSizeIs64( ADOX::_CatalogPtr pCatalog )
{
  // "FileCopies" table, "SourceSizeHiBits" field
  //-----------------------------------------------------------------------------
  ADOX::_TablePtr pFCtable = pCatalog->Tables->Item["FileCopies"];
  if( ! doesFieldExist( pFCtable, L"SourceSizeHiBits" ))
  // The field not found, add it
  {
    g_TheArchive.m_LogFile.AddRecord( L"DATABASE", L"UPDATING", L"FileCopies: Adding SourceSizeHiBits field" );
    pFCtable->Columns->Append( "SourceSizeHiBits", ADOX::adInteger, 0 );
    g_TheArchive.m_LogFile.AddRecord( L"DATABASE", L"UPDATING", L"FC::SourceSizeHiBits field added" );
  }

  // "FileCopies" table, "PackedSizeHiBits" field
  //-----------------------------------------------------------------------------
  if( ! doesFieldExist( pFCtable, L"PackedSizeHiBits" ))
  // The field not found, add it
  {
    g_TheArchive.m_LogFile.AddRecord( L"DATABASE", L"UPDATING", L"FileCopies: Adding PackedSizeHiBits field" );
    pFCtable->Columns->Append( "PackedSizeHiBits", ADOX::adInteger, 0 );
    g_TheArchive.m_LogFile.AddRecord( L"DATABASE", L"UPDATING", L"FC::PackedSizeHiBits field added" );
  }

}