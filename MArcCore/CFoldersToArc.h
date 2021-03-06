// DiskArcher.
// Interface of CFoldersToArc class.
// (C) Marat Mirgaleev, 2002.
// Created 03.03.2002.
// Modifications:
//=================================================================

#if !defined(CFoldersToArc_h)
#define CFoldersToArc_h

#include <afxtempl.h>
#include "../MArcLib/error.h"

class CFolderToArc;

class CFoldersToArc : public CList < CFolderToArc*, CFolderToArc* >
{
public:

  const MArcLib::error* AddFolder( CString name );
  bool Load();
  bool DeleteFolder(CFolderToArc* pFolder);
  CFolderToArc* FindFolder( CString strFolderName ) const;

protected:


private:


};

#endif /* CFoldersToArc_h */
