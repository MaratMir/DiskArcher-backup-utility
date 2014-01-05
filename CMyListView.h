// DiskArcher.
// Interface of CMyListView class.
// (C) Marat Mirgaleev, 2002-2003.
// Created 03.03.2002.
// Modifications:
//	(2) 02.09.2002. IsAnythingSelected() added.
//	(3) 16.01.2003. EraseList() added.
//					Typical behaviour of ShowPopupMenu implemented;
//						m_nMenuID and SetPopupMenu() added.
//=================================================================

#if !defined(AFX_CMYLISTVIEW_H__0FD38692_3631_4AF1_A703_7E7BC986A3B1__INCLUDED_)
#define AFX_CMYLISTVIEW_H__0FD38692_3631_4AF1_A703_7E7BC986A3B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMyListView : public CListView
{
protected:
	DECLARE_DYNCREATE(CMyListView)

// Attributes
protected:
	UINT m_nPopupMenuID;	// (3)

// Operations
public:
	bool IsAnythingSelected();			// (2)
	void EraseList();					// (3)
	void SetPopupMenu( UINT nMenuID )	// (3)
	{	m_nPopupMenuID = nMenuID;	}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyListView)
	protected:
	//}}AFX_VIRTUAL

// Implementation
protected:
	void MyOnKeyDown( NMHDR* pNMHDR );
	void MyOnRightClick( NMHDR *pNotifyStruct );
	virtual void ShowPopupMenu( int nItemSel, CPoint curPoint );
	void DoPopupMenu( UINT nMenuID, CPoint curPoint );
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyListView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMYLISTVIEW_H__0FD38692_3631_4AF1_A703_7E7BC986A3B1__INCLUDED_)
