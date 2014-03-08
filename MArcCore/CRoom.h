// DiskArcher.
// A folder to place several files of copies.
// (C) Marat Mirgaleev, 2001-2002.
// Modifications:
//  (1) 10.02.2002. m_Bundles deleted.
//  (3) 11.02.2002. m_nPrognosisFree, GetOccupiedSpace(),
//                  DeleteMarkedCopies() added.
//                  GetSpaceFree() renamed to GetDiskSpaceFree()
//  (4) 13.02.2002. Changes in CountFiles().
//  (5) 19.02.2002. GetCopies() and GetBundles() added.
//  (6) 20.02.2002. Delete(), DeleteLabel() and IsAvailable() added.
// (13) 24.05.2006. Error tracking improved.
//=======================================================================

#if !defined(CRoom_h)
#define CRoom_h

#include "../MArcLib/CFolder.h"
#include "MyCommon.h"
#include "enums.h"  // (7)

class CMyArchive;
class CFilesCopies;
class CBundles;

class CRoom : public CFolder
{
public:

  enum roomCompressionMode
  {
    rcmAllowed = 0, // Compress a file if it is defined for the file. Otherwise don't compress
    rcmNever   = 1, // Never compress files on the Room because this Room is big and fast
    rcmAlways  = 2  // Compress all files on this Room, except non-compressable files (jpg, zip, ...)
  };

  bool GetDiskSpaceFree();
  CRoom() {};
  CRoom(const CString& strName);
  OpResult/*(7)*/ doCopying();
  bool CheckLabel();
  unsigned int CountAllFiles() const;
  unsigned int CountFilesBeingCopied() const;
  bool DeleteMarkedCopies();
  bool IsAvailable(); // (6)
  bool DeleteLabel(); // (6)
  __int64 GetOccupiedSpace() const;
  __int64 GetPrognosis();
  bool Delete();
  CFilesCopies* GetCopies() const;
  CBundles* GetBundles() const;

  ID m_nRoomID;
  bool m_bRemovable;

  __int64 m_sizeLimit; // The quota for the Room on the respective disk. If 0, it has not been set
  __int64 m_nDiskSpaceFree;
  __int64 m_nPrognosisFree;

  CMyArchive* m_pArchive;

// When does this Room compress files
  roomCompressionMode m_nCompressionMode;


protected:

  bool writeContents();

};

#endif /* CRoom_h */
