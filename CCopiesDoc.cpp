// CCopiesDoc.cpp : implementation of the CCopiesDoc class
//

#include "stdafx.h"
#include "MArc2.h"

#include "CCopiesDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCopiesDoc

IMPLEMENT_DYNCREATE(CCopiesDoc, CDocument)

BEGIN_MESSAGE_MAP(CCopiesDoc, CDocument)
	//{{AFX_MSG_MAP(CCopiesDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCopiesDoc construction/destruction

CCopiesDoc::CCopiesDoc()
{
}

CCopiesDoc::~CCopiesDoc()
{
}

BOOL CCopiesDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	SetTitle( "File Copies" ); // M

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CCopiesDoc serialization

void CCopiesDoc::Serialize(CArchive& ar)
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
// CCopiesDoc diagnostics

#ifdef _DEBUG
void CCopiesDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCopiesDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCopiesDoc commands
