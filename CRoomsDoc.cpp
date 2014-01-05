// CRoomsDoc.cpp : implementation of the CRoomsDoc class
//

#include "stdafx.h"
#include "MArc2.h"

#include "CRoomsDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRoomsDoc

IMPLEMENT_DYNCREATE(CRoomsDoc, CDocument)

BEGIN_MESSAGE_MAP(CRoomsDoc, CDocument)
	//{{AFX_MSG_MAP(CRoomsDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRoomsDoc construction/destruction

CRoomsDoc::CRoomsDoc()
{
}

CRoomsDoc::~CRoomsDoc()
{
}

BOOL CRoomsDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	SetTitle( "Archive Rooms" ); // M

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CRoomsDoc serialization

void CRoomsDoc::Serialize(CArchive& ar)
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

/////////////////////////////////////////////////////////////////////////////
// CRoomsDoc diagnostics

#ifdef _DEBUG
void CRoomsDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CRoomsDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRoomsDoc commands
