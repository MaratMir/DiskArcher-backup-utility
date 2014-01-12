// DiskArcher.
// Implementation of CFolderToArc class.
// (C) Marat Mirgaleev, 2002.
// Created 03.03.2002.
// Modifications:
//=================================================================

#if !defined(CFolderToArc_h)
#define CFolderToArc_h

#include <afx.h>
#include "CFolder.h"
#include "MyCommon.h"

const int nFolderMasksLen = 50;

class CFolderToArc : public CFolder
{
public:

  bool Delete();
  ID  m_nFolderID;
  CString m_strMasks; // Several masks, delimited with ";" or space
  CString m_strExcludeMasks; // Several masks, delimited with ";" or space
  bool  m_bIncludeSubfolders;
  int m_nUpToCopies;

protected:


private:


};

#endif /* CFolderToArc_h */
