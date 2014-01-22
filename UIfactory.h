// DiskArcher.
// A place to create dialogs etc.
// (C) Marat Mirgaleev, 2001-2014.
// Started as a separate file 22.01.2014
//==============================================================================

#pragma once

#include "MArcCore/enums.h"

class CMyArchive;

class UIfactory
{
public:
  void init( CMyArchive* i_archive );
  OpResult initProgressDialog();

private:
  CMyArchive* m_archive;

};
