// DiskArcher.
// CFileOnDisk.h - Implementation of CFileOnDisk class.
// (C) Marat Mirgaleev, 2001-2002.
// Modifications:
//	(1) 21.01.2002. GetType() removed.
//	(2) 21.08.2002. GetType() added :-) 
//=======================================================

#if !defined(CFileOnDisk_h)
#define CFileOnDisk_h

#include "CDiskItem.h"

class CString;


class CFileOnDisk : public CDiskItem
{
public:

  CFileOnDisk() {};
  CFileOnDisk( const CString& strFullName );

  virtual int getType() const // (2)
		{ return DI_FILE; };	// (2)
      // TODO: There is getType() in CDiskItem, but it is non-virtual! And has
      //       different ret.type!

};

#endif /* CFileOnDisk_h */
