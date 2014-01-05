// DiskArcher.
// MainFrm.h - Interface of the CMainFrame class.
// (C) Marat Mirgaleev, 2001-2002.
// Modifications:
//	(1) 15.01.2002. Log added.
//	(2) 12.02.2002. CopiesWindowUpdate() added.
//	(5) 03.07.2002. m_wndStatusBar moved to public from protected.
//	(6) 02.09.2002. Locator added.
//	(8) 12.01.2003. "Excluded files" window added.
//  (9) 25.07.2004. Locator reconstructed.
//==================================================================

#if !defined(AFX_MAINFRM_H__0A4F0330_7771_4695_99BE_A8425A274CF0__INCLUDED_)
#define AFX_MAINFRM_H__0A4F0330_7771_4695_99BE_A8425A274CF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CFilesToArcFrame;
class CRoomsFrame;
class CCopiesFrame;
class CLogFrame;	// (1)
class CNewFilesLocatorFrame; // (6)

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:
	CFilesToArcFrame *m_pFilesToArcFrame;	// M
	CRoomsFrame		 *m_pRoomsFrame;		// M
	CCopiesFrame	 *m_pCopiesFrame;		// M
	CLogFrame		 *m_pLogFrame;			// M
// (9) CNewFilesLocatorFrame *m_pLocatorFrame;		// (6)
//	CExcludedFoldersFrame *m_pExclFoldersFrame;	// (8)

	CStatusBar  m_wndStatusBar;	// (5)

// Operations
public:
	void EnableControls( bool bOn );	// M

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	//}}AFX_VIRTUAL

// Implementation
public:
	void WindowLog();
	void ShowRooms();
	void WindowCopies();
	void CopiesWindowUpdate();	// (2)
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
// (5) CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
//	CToolBar    m_wndToolBar2; //M

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnUpdateWindowRooms(CCmdUI* pCmdUI);
	afx_msg void OnWindowRooms();
	afx_msg void OnUpdateWindowFilesToArchive(CCmdUI* pCmdUI);
	afx_msg void OnWindowFilesToArchive();
	afx_msg void OnUpdateArchiveUpdate(CCmdUI* pCmdUI);
	afx_msg void OnArchiveUpdate();
	afx_msg void OnUpdateWindowCopies(CCmdUI* pCmdUI);
	afx_msg void OnWindowCopies();
	afx_msg void OnClose();
	afx_msg void OnViewLog();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnUpdateStartLocator(CCmdUI* pCmdUI);
	afx_msg void OnStartLocator();
	afx_msg void OnFileOptions();
	afx_msg void OnUpdateRegister(CCmdUI* pCmdUI);
	afx_msg void OnRegister();
	//}}AFX_MSG
	afx_msg void OnUpdateFileCount(CCmdUI *pCmdUI);	// M
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__0A4F0330_7771_4695_99BE_A8425A274CF0__INCLUDED_)
