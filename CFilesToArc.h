// DiskArcher.
// Interface of CFilesToArc class.
// (C) Marat Mirgaleev, 2001-2002.
// Created 16.02.2002.
// Modifications:
//	(1) 18.02.2002. FileDelete() added.
//  (9) 22.11.2003. ResetRuntimeData() added.
//=================================================================

#if !defined(CFilesToArc_h)
#define CFilesToArc_h

#include <afxtempl.h>
#include "resource.h"
#include "MyCommon.h"

//class CString;
class CFileToArc;

class CFilesToArc
	: public CList< CFileToArc*, CFileToArc* >	// M
{
public:

// (12) Eliminated:  int CountForCopying() const;
  bool FileAdd(CString name);
  CFileToArc* FileFind(CString fullName);
  int FilesGetStatus();
	bool FileDelete( CFileToArc* pFile );

// Clear some non-actual runtime information
	void ResetRuntimeData();	// (9)

};

#endif /* CFilesToArc_h */
