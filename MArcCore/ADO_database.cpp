// A database that uses ADO.
// (C) Marat Mirgaleev, 2001-2014.
//======================================================================================

#include "stdafx.h"
#include "ADO_database.h"


/*ADO_database::~ADO_database(void)
{
}
*/


// Returns a "TRUE" or "FALSE" string.
//======================================
const CString ADO_database::trueFalse( bool value )
{
  return ( value ? L"TRUE" : L"FALSE" );
}


// Display a description of the ADO error.
// Parameters: - pConnection is not required, default is NULL.
// LATER: The text of this window must be able to copy to clipboard!!!
//======================================================================
void ADO_database::showADOErrors( _com_error &e, _ConnectionPtr pConnection, CString strAdditionalInfo )
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
  else
    mess += "pConnection = NULL.\n";

// Print COM errors
  _bstr_t bstrSource( e.Source() );
  _bstr_t bstrDescription( e.Description() );
  tmp.Format( _T("COM Error %08lx\n%s"), e.Error(), e.ErrorMessage() );
  mess += tmp;
  CString src = bstrSource;
  mess += L"\nSource: " + src;
  CString des = bstrDescription;
  mess += L"\nDescription: " + des;

  if( strAdditionalInfo != "" )
    mess += L"\n" + strAdditionalInfo;

  AfxMessageBox( mess );
}


//================================================
bool ADO_database::execSQL( CString cmd )
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
    showADOErrors( e, m_pConnection, cmd );
  }
  catch(...)
  {
    AfxMessageBox( L"Some error occured in CArchiveDB::ExecuteSQL().\n" + cmd );
  }
  return bSuccess;
}


// Create a local copy of the database before open it:
//  some compression utilities cannot copy opened files,
//  also if the DB is damaged, program cannot start
//==========================================================
bool ADO_database::preserveDB()
{
  BOOL bOk = CopyFile( m_DB_filename, m_DB_filename + "~", FALSE/*Overwrite existing*/ );
  return( bOk == TRUE );
}


// Check is there a field with a given name in the given table
//==================================================================================
bool ADO_database::doesFieldExist( ADOX::_TablePtr i_pTable, CString i_strFieldName )
{
  ADOX::_ColumnPtr fieldFound = findField( i_pTable, i_strFieldName );
  return ( fieldFound != NULL );
}


// Returns true, if there is a table with given name in the database
//==============================================================================
bool ADO_database::checkIfTableInDB( ADOX::_CatalogPtr pCatalog, CString strTableName )
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


// Get max ID from Database.
// Parameters: - Table name;
//             - Field name;
//             - additional SQL clauses.
// Returns -1 if any error occurs.
//====================================================================================================
int ADO_database::getMaxID( CString strTableName, CString strFieldName, CString strAdditionalClauses )
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
    if( vtTmp.vt == VT_NULL ) // No any record found in the table
      nMax = 0;
    else
      nMax = (long)vtTmp;
    hr = rsMax->Close();
    TESTHR( hr );
  }
  catch(_com_error &e)
  {
    showADOErrors( e, m_pConnection );
  }
  catch(...)
  {
    AfxMessageBox( _T("Some error occured in CArchiveDB::GetMaxID().") );
  }
  return nMax;
}


// Finds a field with a given name in a given table.
//==========================================================================================
ADOX::_ColumnPtr ADO_database::findField( ADOX::_TablePtr i_pTable, CString i_strFieldName )
{
  ADOX::_ColumnPtr fieldFound = NULL;
  _variant_t vIndex;
  ADOX::ColumnsPtr pColumns = i_pTable->Columns;
  ADOX::_ColumnPtr pCurColumn = NULL;
  for ( long lIndex = 0; lIndex < pColumns->Count; lIndex ++ )
  {
    vIndex = lIndex;
    pCurColumn = pColumns->GetItem( vIndex );
    CString strCurColumnName = pCurColumn->Name.GetBSTR(); // Was (LPCSTR)pCurColumn->Name;
    if( strCurColumnName == i_strFieldName )
    {
      fieldFound = pCurColumn;
      break;
    }
  }
  return fieldFound;
}

