// DiskArcher.
// Interface of CFilesToArc class.
// (C) Marat Mirgaleev, 2001-2014.
// Created 16.02.2002.
// Modifications:
//	(1) 18.02.2002. FileDelete() added.
//  (9) 22.11.2003. ResetRuntimeData() added.
//=================================================================

#pragma once

namespace MArcLib { class error; }
class CFileToArc;

class CFilesToArc : public CList< CFileToArc*, CFileToArc* >
{
public:

  const MArcLib::error* FileAdd(CString name);
  CFileToArc* FileFind(CString fullName) const;
  int FilesGetStatus( int& o_systemFileToUpdate ) const;
  bool FileDelete( CFileToArc* pFile );

// Clear some non-actual runtime information
  void ResetRuntimeData();

};
