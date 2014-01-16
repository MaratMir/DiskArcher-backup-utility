// DiskArcher.
// Interface of the CNewFilesLocatorView class.
// (C) Marat Mirgaleev, 2002.
// Created 25.07.2002.
// Modifications:
//	(1) 16.01.2003. EraseList() and ShowPopumMenu() moved into the base class.
//  (3) 06.06-25.07.2004. Reconstruction of Locator.
//============================================================================

#pragma once

#include "../CMyListView.h"

class CNewFilesLocator;
class CFileOnDisk;


class CNewFilesLocatorView : public CMyListView
{
protected: // create from serialization only
	CNewFilesLocatorView();
	DECLARE_DYNCREATE(CNewFilesLocatorView)

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewFilesLocatorView)
protected:
	virtual void OnInitialUpdate(); // called first time after construct
	//}}AFX_VIRTUAL

public:

 	CNewFilesLocator* m_pLocator;   // (3)
// (3)	CNewFilesLocatorDlg* m_pDlg;

	void AddFileToListCtrl( const CFileOnDisk* pFile );
    int  RemoveFilesOfType( const CString sExtension );     // (3)
    void RemoveFilesOfFolder( const CFileOnDisk* pFile );   // (3)
// (1)	void EraseList();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	int m_nPathColumn;	// Current position of Path column
	int m_nNameColumn;	// Current position of Name column
	int m_nTimeColumn;	// Current position of Time column
	int m_nSizeColumn;	// (3) Current position of the Size column

    CFileOnDisk* GetFileFromRow( const int nRow ) const;
        // (3) Constructs a new object!

	void OnRightClick( NMHDR *pNotifyStruct, LRESULT *result );

// Generated message map functions
protected:
// (1)	void ShowPopupMenu( int nItemSel, CPoint curPoint );
	//{{AFX_MSG(CNewFilesLocatorView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKeyDown(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLocatorAdd();
	afx_msg void OnUpdateLocatorAdd(CCmdUI* pCmdUI);
	afx_msg void OnLocatorRemove();
	afx_msg void OnUpdateLocatorRemove(CCmdUI* pCmdUI);
	afx_msg void OnDestroy();
	afx_msg void OnLocatorAddExclFolder();
	afx_msg void OnUpdateLocatorAddExclFolder(CCmdUI* pCmdUI);
	afx_msg void OnLocatorAddExclType();
	afx_msg void OnUpdateLocatorAddExclType(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
