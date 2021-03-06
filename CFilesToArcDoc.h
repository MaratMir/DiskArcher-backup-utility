// CFilesToArcDoc.h : interface of the CFilesToArcDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MARC2DOC_H__78730A13_C692_4E5E_B663_AB73797F46C9__INCLUDED_)
#define AFX_MARC2DOC_H__78730A13_C692_4E5E_B663_AB73797F46C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CFilesToArcView;	// M


class CFilesToArcDoc : public CDocument
{
protected: // create from serialization only
	CFilesToArcDoc();
	DECLARE_DYNCREATE(CFilesToArcDoc)

// Attributes
public:
	CFilesToArcView* m_pListView;	// M

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFilesToArcDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFilesToArcDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CFilesToArcDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MARC2DOC_H__78730A13_C692_4E5E_B663_AB73797F46C9__INCLUDED_)
