// DiskArcher.
// CLogDoc.h - Interface of the CLogDoc class.
// (C) Marat Mirgaleev, 2001-2002.
// Created 15.01.2002.
//==================================================================

#if !defined(AFX_LogDoc_H__INCLUDED_)
#define AFX_LogDoc_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CLogDoc : public CDocument
{
protected: // create from serialization only
	CLogDoc();
	DECLARE_DYNCREATE(CLogDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLogDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CLogDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LogDoc_H__INCLUDED_)
