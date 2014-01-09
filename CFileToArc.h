// DiskArcher.
// CFileToArc class - a file that the program shall copy to the Archive.
// (C) Marat Mirgaleev, 2001-2014.
// Modifications:
//  (1) 04.02.2002. m_bSystem added.
//  (2) 18.02.2002. Delete() added.
//  (3) 03.03.2002. m_nFolderID, m_bPaused added.
//  (4) 08.03.2002. GetStatus() added.
//  (5) 28.03.2002. Init() added.
//  (6) 19.04.2003. Compressing added.
//  (9) 22.11.2003. ResetRuntimeData() added.
// (10) 24.05.2006. getRequiredCopiesNum() added.
//============================================================================

#if !defined(CFileToArc_h)
#define CFileToArc_h

#include <list>
#include "fileStatus.h"
#include "CFileOnDisk.h"
#include "MyCommon.h"

class CRoom;
class CBundle;

class CFileToArc : public CFileOnDisk
{

public:

  enum fileCommand
  {
    fcAddCopy,
    fcNothing
  };

  CFileToArc();
  CFileToArc(CString fullName);
  bool Delete();
  bool GetStatus(); // Decide, must the file be updated or not
  bool PreCompress();
  CString GetCompressedFileName() const;// With the full path!
  bool IsPreCompressed() { return ( m_sCompressedFilename != "" ); }
  void ResetRuntimeData(); // Clear some non-actual runtime information
  unsigned int getRequiredCopiesNum() const;  // (10)

  int m_nPriority;
  int m_nUpToCopies;
  fileStatus m_nStatus;

// In which Rooms to create copies.
// Only for temporary assignments during the Archive update. Isn't stored in DB.
  std::list<CRoom*> m_CopyToRooms;
  unsigned int CountCopies( const CRoom* const i_pRoom ) const;

// Only for temporary assignment during updating the Archive. Isn't stored in DB.
  CBundle* m_pBundle;

// zzz  CFileCopy* m_pCopyToReplace; // It is only run-time information
  fileCommand m_nCommand; // It is only run-time information

// Is this file for the program use or a regular user's file
  bool m_bSystem;

// (3) if != 0, that this File is got by mask
  ID m_nFolderID;

// Archiving of this file is paused by user
  bool m_bPaused;

// Use compression when is storing the backup copy
  bool m_bCompressIt; // (6)

// Calculated size of File's copy if store it into the m_pRoom
  _int64 m_nPredictedCompressedSize;

protected:

  CString m_sCompressedFilename; // Without path!

private:

  void Init();

};

#endif /* CFileToArc_h */
