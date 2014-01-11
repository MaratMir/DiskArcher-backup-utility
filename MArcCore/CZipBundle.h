// DiskArcher.
// CZipBundle class - a bundle of copies in zip-format.
// (C) Marat Mirgaleev, 2001-2006.
// Modifications:
//==============================================================================

#if !defined(CZipBundle_h)
#define CZipBundle_h

#include "CBundle.h"

//class CString;
class CFileToArc;
class CFileCompressor;


class CZipBundle : public CBundle
{

public:

  CZipBundle()  {}
  CZipBundle(CString strFilename);

// Write a copy of the given file to the Bundle
  virtual bool addCopy(CFileToArc* pFile);

// Extract the file given by strGetFile from the Bundle and write
//  it into strWriteToFilename file
	virtual bool extractFile( const CString strGetFile,
							              const CString strWriteToPath,	// (6)
							              const CString strWriteToFilename=L"" ) const;

	virtual bool deleteFileFromBundle( CFileCopy* pFileCopy );


	static CFileCompressor* m_pCompressor;

};

#endif /* CZipBundle_h */
