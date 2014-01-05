// DiskArcher.
// MArc2.h : main header file for the MARC2 application.
// See MArc2.cpp for the implementation of this class
// (C) Marat Mirgaleev, 2001-2002.
// Modifications:
//	(1) 15.01.2002. m_pLogTemplate variable for Log window.
//	(2) 20.01.2002. Definition of BrowseForFolder() and GetErrDescription() 
//					moved to Miscelaneous.h.
//	(3) 10.05.2002. CheckRegDataConformity() and CheckRegistration() added.
//					Tip of the Day added.
//	(4) 02.09.2002. Locator added.
//	(6) 12.01.2003. "Excluded files" template.
//	(8) 03.03.2004. GetPassedDays() added.
//  (9) 24.05.2006. theArchive renamed into g_TheArchive.
//                  Copy protection changed.
//================================================================================

#if !defined(AFX_MARC2_H__008A0CD9_A5BD_4DC3_B419_12505B062B3B__INCLUDED_)
#define AFX_MARC2_H__008A0CD9_A5BD_4DC3_B419_12505B062B3B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

class CMArc2App;
class CMyArchive;

extern CMArc2App theApp;
extern CMyArchive g_TheArchive;	// (9) Was theArchive
extern wchar_t* strMyComp;


#define MyArcUserLen 10


class CMArc2App : public CWinApp
{
public:
	CMArc2App();

	bool CheckRegDataConformity( CString strName, CString strCode );	//(3)
	bool CheckRegistration();	  // (3)
	bool checkRegistration2();	// (9)
	bool checkRegistration3();	// (9)
	int GetPassedDays( void );	// (8)

	void UpdateTreeAndList();
	void FilesGetStatus();
	bool AddFiles();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMArc2App)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	CMultiDocTemplate* m_pFilesToArcTemplate;	// M. FilesToArc window
	CMultiDocTemplate* m_pRoomsTemplate;		// M. Archive Rooms window
	CMultiDocTemplate* m_pCopiesTemplate;		// M. File Copies window
	CMultiDocTemplate* m_pLogTemplate;		    // (1) Log window
	CMultiDocTemplate* m_pLocatorTemplate;	    // (4) Locator window
	CMultiDocTemplate* m_pLocatorExclTemplate;  // (6) "Excluded folders" window

	//{{AFX_MSG(CMArc2App)
	afx_msg void OnAppAbout();
	afx_msg void OnFileOpen();
	afx_msg void OnUpdateHelpTipOfTheDay(CCmdUI* pCmdUI);
	afx_msg void OnHelpTipOfTheDay();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void ShowTipAtStartup(void);	// (3)
//	void ShowTipOfTheDay(void);		// (3)
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MARC2_H__008A0CD9_A5BD_4DC3_B419_12505B062B3B__INCLUDED_)
