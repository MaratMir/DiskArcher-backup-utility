// DiskArcher.
// CArchiveDB class - the interface to the program database.
// (C) Marat Mirgaleev, 2001-2014.
// Modifications:
//  (1) 04.02.2002. FilesToArc: Field "IsSystem" added.
//  (2) 09.02.2002. FilesToArc: A record about the Database.
//  (3) 10.02.2002. IsOpen() added.
//  (4) 10.02.2002. FileCopies: Field "DeleteIt" added.
//  (5) 11.02.2002. CopyDelete() added.
//  (6) 18.02.2002. FileDelete() added.
//  (7) 20.02.2002. BundleDelete() and RoomDelete() added.
//  (8) 03.03.2002. "FoldersToArc" table added.
//                  In "FilesToArchive" table "FolderID" and "Paused" fields added.
//  (9) 07.03.2002. FileUpdate() added.
// (12) 15.10.2002. Table "Options" added. 
//                  ExecSQL() method added.
// (13) 25.01.2003. ExecSQL() turned into public from protected.
// (17) 11.05-26.10.2003. Compression.
// (18) 02.05.2006. ADO compilation warning suppressed.
//                  optionRead() added.
//======================================================================================

#pragma once
/*zzz
//#pragma warning (disable:4146)  // (18)
#import "C:\Program Files\Common Files\system\ado\msadox.dll" // no_namespace
#import "C:\Program Files\Common Files\system\ado\msado15.dll" no_namespace rename( "EOF", "adoEOF" )
//#pragma warning (default:4146)  // (18)
*/
//#include <afx.h>

#include "ADO_database.h"

class CFileToArc;
class CMyArchive;
class CRoom;
class CFileCopy;
class CBundle;


class CArchiveDB : public ADO_database
{
public:

  static const CString MyDBFilename;
  static const unsigned int MyArcUserLen = 10;

  CArchiveDB( CString i_DBfilename ) : ADO_database( i_DBfilename ) {}
	int  BundleGetMaxID();
	bool BundleAdd( CBundle* pBundle );
	bool BundlesLoad();
	bool BundleDelete( CBundle* pBundle );	// (7)

	bool CopiesLoad();
	int  CopyGetMaxID();
	bool CopyAdd( CFileCopy *pCopy );
	bool CopyUpdate( CFileCopy *pCopy );
	bool CopyDelete( CFileCopy *pCopy );	// (5)

	int  RoomGetMaxID();
	bool RoomUpdate( CRoom* pRoom );
	bool RoomsLoad();
	bool RoomDelete( CRoom* pRoom );

  bool Create();  // Create a new Archive Database
  bool Open();
	bool IsOpen();	// (3)
  bool Close();

  bool FileAdd(CFileToArc* pFile);
	bool FileDelete( CFileToArc* pFile );
	bool FileUpdate( CFileToArc* pFile );	// (9)
	bool FilesLoad();	// M

	bool RoomAdd( CRoom* pNewRoom );	// M

  CMyArchive	*m_pArchive;

	bool FoldersLoad();

	bool optionSave( CString sSection, CString sOptionName,
                   CString sValue, CString sValue2=L"" ); // (17)
	bool optionRead( CString i_sSection, CString i_sOptionName, 
                   CString& o_sValue );		// (18)
	void LocatorRestoreDefaultOptions( int nOptionsSection );	// (12)


protected:

	void DBStructureModifications();	// (1)
	void DBStructureModifications2();	// (8)
	void CreateArcLogTable( ADOX::_CatalogPtr pCatalog );
	void CheckLogTable( ADOX::_CatalogPtr pCatalog );
	void CheckIsSystemField( ADOX::_CatalogPtr pCatalog );	// (1)
	void CheckRecordAboutDB();			// (2)
	bool CheckIsFileInArc( CFileToArc* pFile );
	void CheckDeleteItField( ADOX::_CatalogPtr pCatalog );	// (4)
	void CheckFoldersTable( ADOX::_CatalogPtr pCatalog );	// (8)
	void CheckFilesTableFolderAndPausedFields( ADOX::_CatalogPtr pCatalog );
	void CheckOptionsTable( ADOX::_CatalogPtr pCatalog );	// (12)
	void CheckFieldsForCompress( ADOX::_CatalogPtr pCatalog );	// (17)
  void checkFileSizeIs64( ADOX::_CatalogPtr pCatalog );

};
