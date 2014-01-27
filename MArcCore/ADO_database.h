// A database that uses ADO.
// (C) Marat Mirgaleev, 2001-2014.
//======================================================================================

#pragma once

#import "C:\Program Files\Common Files\system\ado\msadox.dll" // no_namespace
#import "C:\Program Files\Common Files\system\ado\msado15.dll" no_namespace rename( "EOF", "adoEOF" )

class ADO_database
{
public:

  ADO_database( CString i_DBfilename ) { m_DB_filename = i_DBfilename; }
//  ~ADO_database(void);

  void showADOErrors( _com_error &e, _ConnectionPtr pConnection = NULL, CString strAdditionalInfo = L"" );
  inline void TESTHR(HRESULT x) {if FAILED(x) _com_issue_error(x);};

  const CString& getDBfilename() const { return m_DB_filename; }
  bool preserveDB();

  bool execSQL( CString cmd );

  bool checkIfTableInDB( ADOX::_CatalogPtr pCatalog, CString strTableName );
  int  getMaxID( CString strTableName, CString strFieldName, CString strAdditionalClauses=L"" );


protected:

  static const CString ADO_database::trueFalse( bool value );
  static ADOX::_ColumnPtr findField( ADOX::_TablePtr i_pTable, CString i_strFieldName );
  bool doesFieldExist( ADOX::_TablePtr i_pTable, CString i_strFieldName );

public:

  _variant_t m_pConnection;

private:

  CString m_DB_filename;

};

