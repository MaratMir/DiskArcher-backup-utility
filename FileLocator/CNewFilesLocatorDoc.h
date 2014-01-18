// DiskArcher.
// CNewFilesLocatorDoc class interface file.
// (C) Marat Mirgaleev, 2002-2014.
// Created 25.07.2002.
// Modifications:
//==========================================================================

#pragma once

class CNewFilesLocatorView;


class CNewFilesLocatorDoc : public CDocument
{
protected: // create from serialization only
	DECLARE_DYNCREATE(CNewFilesLocatorDoc)

// Attributes
public:
  CNewFilesLocatorView* getView(); // 2014. Theoretically, it's one-to-many relationship, but here we limit it to 1-1

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
