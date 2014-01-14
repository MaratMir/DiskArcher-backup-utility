// DiskArcher.
// CFileOnDisk.h - Implementation of CFileOnDisk class.
// (C) Marat Mirgaleev, 2001-2002.
// Modifications:
//	(1) 21.01.2002. GetType() removed.
//	(2) 21.08.2002. GetType() added :-) 
//=======================================================

#pragma once

#include "CDiskItem.h"

class CFileOnDisk : public CDiskItem
{
public:

  CFileOnDisk() {};
  CFileOnDisk( const CString& strFullName );

  virtual int getType() const
		{ return CDiskItem::DI_FILE; };
      // TODO: There is getType() in CDiskItem, but it is non-virtual! And has
      //       different ret.type!

};
