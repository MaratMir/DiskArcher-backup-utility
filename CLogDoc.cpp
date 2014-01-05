// DiskArcher.
// CLogDoc.cpp - Implementation of the CLogDoc class.
// (C) Marat Mirgaleev, 2001-2002.
// Created 15.01.2002.
//==================================================================

#include "stdafx.h"
//#include "MArc2.h"

#include "CLogDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLogDoc

IMPLEMENT_DYNCREATE(CLogDoc, CDocument)

BEGIN_MESSAGE_MAP(CLogDoc, CDocument)
	//{{AFX_MSG_MAP(CLogDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogDoc construction/destruction

CLogDoc::CLogDoc()
{
}

CLogDoc::~CLogDoc()
{
}

BOOL CLogDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	SetTitle( _T("Log View") ); // M

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CLogDoc serialization

void CLogDoc::Serialize(CArchive& ar)
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
// CLogDoc diagnostics

#ifdef _DEBUG
void CLogDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CLogDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLogDoc commands


