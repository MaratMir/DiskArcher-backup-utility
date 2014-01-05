// TO DO: Rename It into xxxFilesToArcXxx


// LeftView.h : interface of the CLeftView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_LEFTVIEW_H__4F828FEC_1314_47C5_BB21_3BA1F959F602__INCLUDED_)
#define AFX_LEFTVIEW_H__4F828FEC_1314_47C5_BB21_3BA1F959F602__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFilesToArcDoc;

class CLeftView : public CTreeView
{
protected: // create from serialization only
	CLeftView();
	DECLARE_DYNCREATE(CLeftView)

// Attributes
public:
	CFilesToArcDoc* GetDocument();

	enum ImageIDS{	// M
		IID_MYCOMPUTER = 0,
		IID_FLOPPYDRIVE,
		IID_HARDDISK,
		IID_CDDRIVE,
		IID_FOLDER
	};

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLeftView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	//}}AFX_VIRTUAL

// Implementation
public:
	void EraseTree();
	void UpdateTree();
	void TrackSelChange();
	CString GetFullPath( HTREEITEM hItem );	// M
	virtual ~CLeftView();
	HTREEITEM GetChildItem( HTREEITEM hParenItem, CString itemName ); // M
	HTREEITEM GetCompItem( CString compName );// M Find a computer in the Tree
	HTREEITEM GetDriveItem( HTREEITEM hCompItem, CString driveName ); // M
	HTREEITEM GetDirItem( HTREEITEM hParenDirItem, CString dirName ); // M

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CImageList m_ImageList; // M

// Generated message map functions
protected:
	//{{AFX_MSG(CLeftView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSelChanged(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in LeftView.cpp
inline CFilesToArcDoc* CLeftView::GetDocument()
   { return (CFilesToArcDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LEFTVIEW_H__4F828FEC_1314_47C5_BB21_3BA1F959F602__INCLUDED_)
