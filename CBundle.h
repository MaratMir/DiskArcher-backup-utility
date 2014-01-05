// DiskArcher.
// A "cabinet" file.
// The path to the Bundle is the same as its Room's path.
// The Bundle stores one or more files (file copies) and has his
//   own structure. The list of stored files is got from this structure only.
// (C) Marat Mirgaleev, 2001-2006.
// Modifications:
//	(1) 10.02.2002. m_pRoom deleted.
//	                AddCopy() and DeleteFileFromBundle() added.
//	(2) 20.02.2002. Delete() added.
//	(3) 11.05.2003. AddCopy() became virtual.
//	(6) 26.10.2003. ExtractFile() parameters changed.
//	(7) 19.11.2003. DeleteFileFromBundle() became virtual.
//	(8) 24.05.2006. bool Delete() turned into bool deleteThis().
//=================================================================

#if !defined(CBundle_h)
#define CBundle_h

#include "bundleType.h"
#include "CFileOnDisk.h"
#include "MyCommon.h"  // M

class CFileToArc;
class CFileCopy;

class CBundle : public CFileOnDisk
{
public:

	CBundle()  {}
	CBundle( CString strFilename );
	bool deleteThis();  // (8) Was: bool Delete();

// Write a copy of the given file to the Bundle
	virtual bool addCopy( CFileToArc* pFile );
  virtual bool deleteFileFromBundle( CFileCopy* pFileCopy );

// Extract the file given by strGetFile from the Bundle and write
//  it into strWriteToFilename file
	virtual bool extractFile( const CString strGetFile,
							              const CString strWriteToPath,	// (6)
							              const CString strWriteToFilename="" ) const;

// Extract all the files from the Bundle
  virtual bool extractAll(const CString strToPath) const;


  ID	m_nBundleID;
  ID	m_nRoomID;
  bundleType	m_nType;
  CString	m_strExtension;

};

#endif /* CBundle_h */
