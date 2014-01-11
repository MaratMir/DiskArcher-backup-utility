// DiskArcher.
// CDiskItem.h - Implementation of CDiskItem class.
// (C) Marat Mirgaleev, 2001-2002.
// Modifications:
//	(1) 21.01.2002. ItemType name of enum.
//	                m_nType added.
//	                GetType() changed.
//	(2) 25.05.2002. Definition of AddWithSlash() added.
//	(3) 25.08.2002. m_nType corrections.
//  (4) 26.07.2004. GetExtension() added.
//=======================================================

#if !defined(CDiskItem_h)
#define CDiskItem_h

#include <ATLComTime.h>

CString AddWithSlash( const CString& str1, const CString& str2 );	// (2)

class CFolder;


class CDiskItem
{

public:

	enum ItemType/*(1)*/ { DI_FOLDER=1, DI_FILE };  // M

	CDiskItem() {}
	CDiskItem( const CString& strFullName );
  virtual ~CDiskItem() {}

  ItemType getType() const { return m_nType; }	// (1)
	CString getFullPath() const;
  CString getFullName() const;
  CString getExtension() const;   // (4)

	bool checkExistence();
	bool getInfo();


  DWORD m_nSize;
  CString	m_strComputer;
  CString	m_strDrive;
  CString	m_strDir;
  CString	m_strName;

// Item's last write time
  COleDateTime m_LastWriteTime;

  CFolder	*m_pParentFolder;

protected:

	void setType( ItemType nType )  
		{ m_nType = nType; };	// (3)

private:

	ItemType m_nType;	// (1)

};

#endif /* CDiskItem_h */
