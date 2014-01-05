// DiskArcher.
// CFilesToArcDoc.cpp - Implementation of the CFilesToArcDoc class.
// (C) Marat Mirgaleev, 2001-2002.
//==================================================================

#include "stdafx.h"
#include "MArc2.h"

#include "CFilesToArcDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFilesToArcDoc

IMPLEMENT_DYNCREATE(CFilesToArcDoc, CDocument)

BEGIN_MESSAGE_MAP(CFilesToArcDoc, CDocument)
	//{{AFX_MSG_MAP(CFilesToArcDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFilesToArcDoc construction/destruction

CFilesToArcDoc::CFilesToArcDoc()
{
}

CFilesToArcDoc::~CFilesToArcDoc()
{
}

BOOL CFilesToArcDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	SetTitle( "Files To Archive" ); // M

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CFilesToArcDoc serialization

void CFilesToArcDoc::Serialize(CArchive& ar)
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
// CFilesToArcDoc diagnostics

#ifdef _DEBUG
void CFilesToArcDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CFilesToArcDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFilesToArcDoc commands


