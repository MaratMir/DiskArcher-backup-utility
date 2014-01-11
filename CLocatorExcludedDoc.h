// DiskArcher.
// CLocatorExcludedDoc class interface file.
// (C) Marat Mirgaleev, 2003.
// Created 12.01.2003.
// Modifications:
//  (1) Reconstruction of the Locator.
//==========================================================================

#pragma once

#include "MArcCore/enums.h"

class CNewFilesLocator;


class CLocatorExcludedDoc : public CDocument
{
protected: // create from serialization only
	DECLARE_DYNCREATE(CLocatorExcludedDoc)

// Attributes
public:
	CWnd* m_pParentWindow;			// M
	CNewFilesLocator* m_pLocator;	// M
	LocatorWhatToExclude m_nShowType;

	CStringList* GetSourceList();

// Operations
public:
	bool Save();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLocatorExcludedDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL CanCloseFrame(CFrameWnd* pFrame);
	protected:
	virtual BOOL SaveModified();
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
	//{{AFX_MSG(CLocatorExcludedDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
