// DiskArcher.
// Interface of CRoomLabel class.
// (C) Marat Mirgaleev, 2001-2002.
// Modifications:
//	(1) 20.02.2002. Erase() and GetFileName() added.
//====================================================================

#if !defined(AFX_CROOMLABEL_H__FA164463_A54C_11D5_A440_000000000000__INCLUDED_)
#define AFX_CROOMLABEL_H__FA164463_A54C_11D5_A440_000000000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRoom;

class CRoomLabel  
{
public:
	CRoom* m_pRoom;

	CRoomLabel()  {};
	virtual ~CRoomLabel()  {};

	bool Check();
	bool Write();
	bool Erase();
	CString GetFileName();
};

#endif // !defined(AFX_CROOMLABEL_H__FA164463_A54C_11D5_A440_000000000000__INCLUDED_)
