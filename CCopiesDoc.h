// CCopiesDoc.h : interface of the CCopiesDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CopiesDoc_H__78730A13_C692_4E5E_B663_AB73797F46C9__INCLUDED_)
#define AFX_CopiesDoc_H__78730A13_C692_4E5E_B663_AB73797F46C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CCopiesView;	// M


class CCopiesDoc : public CDocument
{
protected: // create from serialization only
	CCopiesDoc();
	DECLARE_DYNCREATE(CCopiesDoc)

// Attributes
public:
	CCopiesView* m_pListView;	// M

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCopiesDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCopiesDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CCopiesDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CopiesDoc_H__78730A13_C692_4E5E_B663_AB73797F46C9__INCLUDED_)
