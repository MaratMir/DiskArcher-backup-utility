// DiskArcher.
// CFolder class - a directory on a disk.
// (C) Marat Mirgaleev, 2001-2014.
// Modifications:
//  (1) 25.07.02. Analyze() moved into CNewFilesLocator class.
//==========================================================================

#pragma once

#include <vector>
#include "CDiskItem.h"


class CFolder : public CDiskItem
{

public:

  CFolder() {};
  CFolder( const CString& strFullName );
  ~CFolder();

  int getItems();
  // (1) void Analyze();
  // (1) bool m_bHasItemsToArchivate;
  void clear(void);
  virtual int getType() const
    { return DI_FOLDER; };
      // TODO: There is getType() in CDiskItem! It's a mess!

public:

  std::vector<CDiskItem*> m_items;  // Making it private doesn't make my life any easier

};
