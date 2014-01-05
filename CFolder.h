// DiskArcher.
// CFolder class interface file.
// (C) Marat Mirgaleev, 2001-2002.
// Modifications:
//	(1) 25.07.02. Analyze() moved into CNewFilesLocator class.
//==========================================================================

#if !defined(CFolder_h)
#define CFolder_h

#include "resource.h"
#include "CDiskItem.h"

#include <afxtempl.h> // M


class CFolder : public CDiskItem
{
public:
  CFolder() {};
  CFolder( const CString& strFullName );
  ~CFolder();
  int GetItems();
// (1)    void Analyze();
// (1)    bool	m_bHasItemsToArchivate;
	CList< CDiskItem*, CDiskItem* > m_Items;
		// M CDiskItem	*m_Items;
	void Clear(void);
  virtual int GetType() const	// M
		{ return DI_FOLDER; };
      // TODO: There is getType() in CDiskItem! It's a mess!
};

#endif /* CFolder_h */
