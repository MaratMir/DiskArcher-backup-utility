// DiskArcher.
// CNewFilesLocatorDoc class interface file.
// (C) Marat Mirgaleev, 2002.
// Created 25.07.2002.
// Modifications:
//==========================================================================

#if !defined(AFX_NewFilesLocatorDoc_H__78730A13_C692_4E5E_B663_AB73797F46C9__INCLUDED_)
#define AFX_NewFilesLocatorDoc_H__78730A13_C692_4E5E_B663_AB73797F46C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CNewFilesLocatorView;	// M


class CNewFilesLocatorDoc : public CDocument
{
protected: // create from serialization only
	DECLARE_DYNCREATE(CNewFilesLocatorDoc)

// Attributes
public:
//	CNewFilesLocatorView* m_pListView;	// M

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewFilesLocatorDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CNewFilesLocatorDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NewFilesLocatorDoc_H__78730A13_C692_4E5E_B663_AB73797F46C9__INCLUDED_)
