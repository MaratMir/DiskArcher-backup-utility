// DiskArcher.
// CNewFilesLocatorDoc class implementation file.
// (C) Marat Mirgaleev, 2002.
// Created 25.07.2002.
// Modifications:
//==========================================================================

#include "stdafx.h"
#include "MArc2.h"

#include "CNewFilesLocatorDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CNewFilesLocatorDoc, CDocument)

BEGIN_MESSAGE_MAP(CNewFilesLocatorDoc, CDocument)
	//{{AFX_MSG_MAP(CNewFilesLocatorDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//DEL CNewFilesLocatorDoc::CNewFilesLocatorDoc()
//DEL {
//DEL }


//DEL CNewFilesLocatorDoc::~CNewFilesLocatorDoc()
//DEL {
//DEL }


BOOL CNewFilesLocatorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

// TODO: add reinitialization code here
// (SDI documents will reuse this document)

	SetTitle( _T("Files found by Locator") ); // M

	return TRUE;
}


void CNewFilesLocatorDoc::Serialize(CArchive& ar)
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
void CNewFilesLocatorDoc::AssertValid() const
{
	CDocument::AssertValid();
}


void CNewFilesLocatorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

