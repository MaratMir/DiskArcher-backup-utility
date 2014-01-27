// DiskArcher.
// CFileCopy class - a copy of a file.
//    There can be many copies of a File in the Archive.
// (C) Marat Mirgaleev, 2001-2014.
// Modifications:
//  (1) 09.02.2002. m_bDeleteIt added.
//  (2) 19.02.2002. GetRoom() added.
//  (4) 26.03.2003. Extract() implemented - the code moved from CCopiesView.
//===========================================================================

#if !defined(CFileCopy_h)
#define CFileCopy_h

#include <afx.h>
#include <ATLComTime.h>

#include "MyCommon.h"

class CFileToArc;
class CRoom;


class CFileCopy
{
public:

  ID  m_nCopyID;
  CString m_strPath;
  CString m_strFilename; // With path?
  COleDateTime  m_FileDateTime;
  __int64 m_size;
  __int64 m_packedSize; // How can I get it from zip-file?
  ID m_nBundleID;
  CString m_strUser;
  bool  m_bDeleteIt;  // "Delete" flag. If it is true, the program must
            // delete this File from Rooms during Archive Update process
  bool Extract( const CString strExtractToPath ) const; 
  bool MarkForDeletion();
  CRoom* GetRoom() const;

public:

  int getSizeHi()  { LARGE_INTEGER li; li.QuadPart = m_size; return li.HighPart; }
  int getSizeLow() { LARGE_INTEGER li; li.QuadPart = m_size; return li.LowPart; }
  void setSize( int hi, int low )  { LARGE_INTEGER li; li.HighPart = hi; li.LowPart = low; m_size = li.QuadPart; }

  int getPackedSizeHi()  { LARGE_INTEGER li; li.QuadPart = m_packedSize; return li.HighPart; }
  int getPackedSizeLow() { LARGE_INTEGER li; li.QuadPart = m_packedSize; return li.LowPart; }
  void setPackedSize( int hi, int low )
    { LARGE_INTEGER li; li.HighPart = hi; li.LowPart = low; m_packedSize = li.QuadPart; }

};

#endif /* CFileCopy_h */
