// DiskArcher.
// CNewFilesLocator class implementation file.
// (C) Marat Mirgaleev, 2002.
// Created 25.07.2002.
// Modifications:
//  (1) 02.11.2002. CLocatorFolder class extracted from this class.
//  (2) 01.11.2002. theDB changed to *pTheDB.
//  (3) 15.01.2003. One copy of excludedFolders and one copy of excludedFileTypes for all instances of the class.
//                  Ignore case.
//  (4) 14.06.2004. ShowCurrentFolder() renamed into ShowFolderInDlg().
//                  LocatorLoadOptions() renamed into LoadOptions().
//  (5) 24.07.2004. Total reconstruction.
//==============================================================================

#include "stdafx.h"
#include "..\MArcCore\enums.h"
#include "..\MArcCore\CArchiveDB.h"	// It also includes ADO headers
#include "..\MArcCore\CMyArchive.h"
#include "..\MArcCore\CLocatorFolder.h"
#include "..\MArc2.h"
#include "CNewFilesLocator.h"
#include "CNewFilesLocatorFrame.h"
#include "CNewFilesLocatorView.h"


// Static members of the class.
// I think they will be destroyed automatically when the program exits.
CStringList CNewFilesLocator::m_excludedFolders;	// (3)
CStringList CNewFilesLocator::m_excludedFileTypes;	// (3)


//------------------------------------------------------------------------------
CNewFilesLocator::CNewFilesLocator()
/* (5) Was:
CNewFilesLocator::CNewFilesLocator( const CString& strFolderName,
	CNewFilesLocatorView* pView, CNewFilesLocatorDlg* pLocatorDlg )*/
{
    m_pFrame = NULL;
// (5)	m_pView = pView;
// (5)	m_pDlg = pLocatorDlg;
    m_pStartFolder = NULL;
}


//------------------------------------------------------------------------------
CNewFilesLocator::~CNewFilesLocator()
{
	delete m_pStartFolder;
}


// (5) Totally reconstructed.
//------------------------------------------------------------------------------
bool CNewFilesLocator::Init()
{
    bool bResult = false;
    CDocument* pDoc = theApp.m_pLocatorTemplate->OpenDocumentFile(NULL);
    if( ! pDoc )
        AfxMessageBox( _T("Internal Error in Locator: OpenDoc") );
    else
    {
        POSITION pos = pDoc->GetFirstViewPosition();
        if( pos != NULL )
        {
            m_pView = (CNewFilesLocatorView*)pDoc->GetNextView(pos);
            if( m_pView != NULL )
            {
                m_pView->m_pLocator = this;
                m_pFrame = (CNewFilesLocatorFrame*)m_pView->GetParentFrame();
                if( m_pFrame != NULL )
                {
                    m_pFrame->m_pLocator = this;
                    bResult = true;
                }
            }
        }
    }
    if( bResult )
    {
        m_pDlg = new CNewFilesLocatorDlg( this );
        if( m_pDlg )
        // Load options from DB
	        bResult = LoadOptions();
    }
    if( bResult )
    {
    	if ( ! m_pDlg->Create( IDD_LOCATOR_DIALOG ) )
	    	AfxMessageBox( _T("Cannot create Locator Dialog.") );
	    else
	    {
		    m_pDlg->ShowWindow( SW_NORMAL );
		    m_pDlg->CenterWindow();
	    }
    }
    return bResult;
}


// (5)
void CNewFilesLocator::EnableControls( bool bOnOff )
{
  if( m_pFrame != NULL )
    m_pFrame->EnableWindow( bOnOff );
}


// (1)
//------------------------------------------------------------------------------
bool CNewFilesLocator::LoadOptions()
{
	bool bSuccess = false;

	m_excludedFolders.RemoveAll();	// (3)
	m_excludedFileTypes.RemoveAll();	// (3)
	
	_RecordsetPtr rsOptions;
	rsOptions.CreateInstance(__uuidof(Recordset)); 
	HRESULT hr;
	try
	{
    // Select all Options
    wchar_t* select = L"SELECT * FROM ProgramOptions"
                      L" WHERE SectionName=\"Locator\"";
    hr = rsOptions->Open( select, g_TheArchive.m_pDB->m_pConnection, adOpenStatic, adLockReadOnly, adCmdText );
    TESTHR( hr );
		while( ! rsOptions->adoEOF )
		{
			_bstr_t  bstrTmp; // Temporary string for type conversion
			_variant_t vtTmp;
			
      bstrTmp = rsOptions->Fields->Item["OptionName"]->Value;
			CString strOption = bstrTmp.GetBSTR(); // Was: (LPCSTR)bstrTmp;
			strOption.TrimRight();

      bstrTmp = rsOptions->Fields->Item["OptionValue"]->Value;
			CString strValue = bstrTmp.GetBSTR(); // Was: (LPCSTR)bstrTmp;
			strValue.TrimRight();
			strValue.MakeUpper();	// (3)

			if ( strOption == "exclFileType" )
				m_excludedFileTypes.AddHead( strValue );
			if ( strOption == "exclFolder" )
				m_excludedFolders.AddHead( strValue );
			rsOptions->MoveNext();
		}
		rsOptions->Close();

		bSuccess = true;
	}
	catch(_com_error &e)
	{
    // Notify the user of errors if any
    ShowADOErrors( e, g_TheArchive.m_pDB->m_pConnection );
	}
	catch(...)
	{
		AfxMessageBox( _T("Some error occured")
					   _T(" in CNewFilesLocator::LocatorLoadOptions().") );
	}

	return bSuccess;
}


//------------------------------------------------------------------------------
void CNewFilesLocator::addToList( CFileOnDisk* pFile )
{
  m_foundFiles.AddTail( pFile );
  m_pView->AddFileToListCtrl( pFile );
}


// Check that there isn't this type of files in the Excluded,
//    than add to the Excluded.
// Returns true if anything is removed frow the View.
//-(5)--------------------------------------------------------------------------
bool CNewFilesLocator::CheckAndAddToExclTypes( const CFileOnDisk* const pFile )
{
    bool bResult = false;
    CString sExt = pFile->getExtension();   // Get the type of selected file
    sExt.MakeUpper();

// Is the type in the Excluded already?
    POSITION found = m_excludedFileTypes.Find( sExt );
    if( !found )    // Isn't - add
    {
        m_excludedFileTypes.AddTail( sExt );
    // And to the DB
        bResult = AddExcludedTypeToDB( sExt );

        bResult = ( RemoveFilesOfType( sExt ) > 0 );
    }
    return bResult;
}


// Remove files of the given type from Locator's list of found files and
//   from the View.
// Returns how many rows are removed from the View.
//-(5)--------------------------------------------------------------------------
int CNewFilesLocator::RemoveFilesOfType( const CString& sExtension )
{
// Remove files of this type from Locator's list of found files
    while( true )
    {
        POSITION listPos = m_foundFiles.GetHeadPosition();
        bool bRemoved = false;
        while( listPos != NULL )
        {
            POSITION prevPos = listPos;
            CFileOnDisk* pCurFile = m_foundFiles.GetNext( listPos );
            ASSERT( pCurFile );
            CString curExt = pCurFile->getExtension();
            curExt.MakeUpper();
            if( curExt == sExtension )
            {
                m_foundFiles.RemoveAt( prevPos );
                delete pCurFile; // It is not in use everywhere, delete it
                bRemoved = true;
                break;  // Actually, start from the beginning of the list
            }
        }
        if( ! bRemoved )
            break;  // Now there isn't any appropriate file, stop
        // else start again
	}

// Remove files of this type from the View
    int nRemoved = m_pView->RemoveFilesOfType( sExtension );

    return nRemoved;
}


//------------------------------------------------------------------------------
bool CNewFilesLocator::isAborted() const
{ 
  return ( m_pDlg->IsAborted() || g_TheArchive.m_bStopWorking );
}


//------------------------------------------------------------------------------
void CNewFilesLocator::showFolderInDlg( const CString& strFolder ) const
{
  m_pDlg->m_strCurrentFolder = strFolder;
  m_pDlg->UpdateData( FALSE );
}


//------------------------------------------------------------------------------
void CNewFilesLocator::Analyze( const CString& startPath/*(5)Added*/ )
{
  if( m_pStartFolder != NULL )    // For repeated
      delete m_pStartFolder;      //      use
  m_pStartFolder = new CLocatorFolder( startPath, this );
	m_pStartFolder->GetItems();
	m_pStartFolder->Analyze(); // Recursively
/* (1) Was:
// First of all - check subfolders
	for( POSITION pos=m_Items.GetHeadPosition(); pos != NULL; )
	{
		if( m_pDlg->IsAborted() 	// Interrupted by user
		 || theArchive.m_bStopWorking )
			break;

		CDiskItem *curItem = m_Items.GetNext(pos);
		if( curItem->GetType() == CDiskItem::DI_FOLDER )
		{
			m_pDlg->m_strCurrentFolder = curItem->GetFullName();
			m_pDlg->UpdateData( FALSE );
			CFolder *curFolder = (CFolder*)curItem;
			CNewFilesLocator locator( *curFolder, m_pView, m_pDlg );
			if( locator.Init() )
			{
				locator.GetItems();
				locator.Analyze(); // Recursively
			}
		}
	}


// Then check files in this folder
	for( pos=m_Items.GetHeadPosition(); pos != NULL; )
	{
		if( m_pDlg->IsAborted() 	// Interrupted by user
		 || theArchive.m_bStopWorking )
			break;

		CDiskItem *curItem = m_Items.GetNext(pos);
		if( curItem->GetType() == CDiskItem::DI_FILE )
		{
			m_pDlg->ShowProgress( curItem->GetFullPath() );

			COleDateTime curT = COleDateTime::GetCurrentTime();
			COleDateTimeSpan timeDiff = curT - curItem->m_LastWriteTime;
			if( timeDiff.GetTotalHours() <= m_pDlg->m_nDays * 24 )
			{
			// If this file is not in Archive yet
				CFileToArc* pFound = 
					theArchive.m_FilesToArc.FileFind( curItem->GetFullName() );
				if( ! pFound )
				// Add this file to the ListCtrl
					m_pView->AddFileToListCtrl( (CFileOnDisk*)curItem );
			}
		}
	}
*/
}


//-(5)--------------------------------------------------------------------------
bool CNewFilesLocator::AddExcludedToDB( LocatorWhatToExclude nType, 
                                        const CString& sName )
{
    bool bSuccess = false;
	CString sOptionName;
	switch( nType )
	{
		case LocExclFileTypes:
			sOptionName = "exclFileType";
			break;
		case LocExclFolders:
			sOptionName = "exclFolder";
			break;
	}
  CString cmd;
  cmd.Format( L"INSERT INTO ProgramOptions"
              L" (SectionName, OptionName, OptionValue, OptionValue2)"
              L" VALUES (\"Locator\", \"%s\", \"%s\", \"\" )",
              sOptionName, sName );
  bSuccess = g_TheArchive.m_pDB->ExecSQL( cmd );
  return bSuccess ;
}


//-(5)--------------------------------------------------------------------------
bool CNewFilesLocator::AddExcludedTypeToDB( const CString& sExtension )
{
  bool bResult = AddExcludedToDB( LocExclFileTypes, sExtension );
  return bResult;
}


//-(5)--------------------------------------------------------------------------
bool CNewFilesLocator::AddExcludedFolderToDB( const CString& sFolderName )
{
  bool bResult = AddExcludedToDB( LocExclFolders, sFolderName );
  return bResult;
}
