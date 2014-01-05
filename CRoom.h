// DiskArcher.
// A folder to place several files of copies.
// (C) Marat Mirgaleev, 2001-2002.
// Modifications:
//	(1) 10.02.2002. m_Bundles deleted.
//	(3) 11.02.2002. m_nPrognosisFree, GetOccupiedSpace(),
//						DeleteMarkedCopies() added.
//					GetSpaceFree() renamed to GetDiskSpaceFree()
//	(4) 13.02.2002. Changes in CountFiles().
//	(5) 19.02.2002. GetCopies() and GetBundles() added.
//	(6) 20.02.2002. Delete(), DeleteLabel() and IsAvailable() added.
// (13) 24.05.2006. Error tracking improved.
//=======================================================================

#if !defined(CRoom_h)
#define CRoom_h

#include "roomCompressionMode.h"
#include "CFolder.h"

#include "MyCommon.h"
#include "enums.h"  // (7)

class CMyArchive;
class CFilesCopies;
class CBundles;

class CRoom : public CFolder
{
public:

  bool GetDiskSpaceFree();
  CRoom() {};
  CRoom(const CString& strName);
  OpResult/*(7)*/ doCopying();
  bool CheckLabel();
  enum countType { countAll, countForCopying };	// (4)
	int CountFiles( enum countType nType );
		// (4) Was: int CountFilesForCopying();
  bool DeleteMarkedCopies();
	bool IsAvailable();		// (6)
	bool DeleteLabel();		// (6)
  _int64 GetOccupiedSpace() const;
	_int64 GetPrognosis();
	bool Delete();
  CFilesCopies* GetCopies() const;
  CBundles* GetBundles() const;

  ID	m_nRoomID;
  bool	m_bRemovable;
  /* TODO: ?? If this Room is a removable media, this variable is
   * the capacity of the media?
   * If 0, it has not been set */
  int	m_nSizeLimit;
  _int64	m_nDiskSpaceFree;
  _int64	m_nPrognosisFree;

  CMyArchive* m_pArchive;

// When does this Room compress files
  roomCompressionMode	m_nCompressionMode;


protected:

  bool writeContents();

};

#endif /* CRoom_h */
