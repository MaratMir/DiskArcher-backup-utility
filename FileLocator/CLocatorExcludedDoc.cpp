// DiskArcher.
// CLocatorExcludedDoc class implementation file.
// (C) Marat Mirgaleev, 2003-2014.
// Created 12.01.2003.
// Modifications:
//==========================================================================

#include "stdafx.h"
#include "../MArcCore/CMyArchive.h"
#include "../MArcCore/CArchiveDB.h" // It also includes ADO headers
#include "CNewFilesLocator.h"
#include "CLocatorExcludedDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CLocatorExcludedDoc, CDocument)

BEGIN_MESSAGE_MAP(CLocatorExcludedDoc, CDocument)
	//{{AFX_MSG_MAP(CLocatorExcludedDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CLocatorExcludedDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

// TODO: add reinitialization code here
// (SDI documents will reuse this document)

	return TRUE;
}


void CLocatorExcludedDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


#ifdef _DEBUG
void CLocatorExcludedDoc::AssertValid() const
{
	CDocument::AssertValid();
}


void CLocatorExcludedDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


CStringList* CLocatorExcludedDoc::GetSourceList()
{
  CStringList* pSrcList = NULL;
  switch( m_nShowType )
  {
    case LocExclFileTypes:
      pSrcList = &( m_pLocator->getExcludedFileTypes() );
      break;
    case LocExclFolders:
      pSrcList = &( m_pLocator->getExcludedFolders() );
      break;
  }
  return pSrcList;
}


BOOL CLocatorExcludedDoc::CanCloseFrame(CFrameWnd* pFrame) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDocument::CanCloseFrame(pFrame);
}


BOOL CLocatorExcludedDoc::SaveModified() 
{
// Add your specialized code here and/or call the base class
	if( IsModified() )
		if( AfxMessageBox( _T("Save changes?"), MB_YESNO ) == IDYES )
			Save();
		else
			m_pLocator->LoadOptions();

	m_pParentWindow->ShowWindow( SW_RESTORE );
	return TRUE;
//	return CDocument::SaveModified();
}


bool CLocatorExcludedDoc::Save()
{
	bool bSuccess=false;
	CString sOptionName;
	CStringList* pSrcList = GetSourceList();
	switch( m_nShowType )
	{
		case LocExclFileTypes:
			sOptionName = "exclFileType";
			break;
		case LocExclFolders:
			sOptionName = "exclFolder";
			break;
	}

// Delete existing items from DB
	CString sCmd = "DELETE FROM ProgramOptions"
				   " WHERE SectionName=\"Locator\"";
	sCmd += " AND OptionName=\"" + sOptionName + "\"";
  bSuccess = g_TheArchive.m_pDB->ExecSQL( sCmd );

// Write all items from the list to DB
	if( bSuccess )
	{
		POSITION pos;
		for( pos = pSrcList->GetHeadPosition(); pos != NULL; )
		{
			CString cmd;
			cmd.Format( _T("INSERT INTO ProgramOptions")
				        _T(" (SectionName, OptionName, OptionValue, OptionValue2)")
						_T(" VALUES (\"Locator\", \"%s\", \"%s\", \"\" )"),
						sOptionName, pSrcList->GetNext( pos ) );
			if( ! g_TheArchive.m_pDB->ExecSQL( cmd ) )
			{
				bSuccess = false;
				break;
			}
		}
	}	
	if( bSuccess )
		SetModifiedFlag( FALSE );

	return bSuccess;
}
