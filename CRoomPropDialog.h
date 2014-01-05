// DiskArcher.
// CRoomPropDialog dialog.
// (C) Marat Mirgaleev, 2001-2003.
// Modifications:
//=====================================================================

#if !defined(AFX_CROOMPROPDIALOG_H__693A5CF9_5554_49EF_9FBC_BFD3075C9A06__INCLUDED_)
#define AFX_CROOMPROPDIALOG_H__693A5CF9_5554_49EF_9FBC_BFD3075C9A06__INCLUDED_

#include "resource.h"

class CRoom;


class CRoomPropDialog : public CDialog
{
// Construction
public:
	static int Edit( CRoom *pRoom );
	CRoomPropDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRoomPropDialog)
	enum { IDD = IDD_ROOM_PROPERTIES_DIALOG };
	CButton	m_CompressionMode;
	int		m_Limit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRoomPropDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CRoom *pTheRoom;

	// Generated message map functions
	//{{AFX_MSG(CRoomPropDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CROOMPROPDIALOG_H__693A5CF9_5554_49EF_9FBC_BFD3075C9A06__INCLUDED_)
