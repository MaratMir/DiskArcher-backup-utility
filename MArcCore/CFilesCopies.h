// DiskArcher.
// Interface of CFilesCopies class.
// (C) Marat Mirgaleev, 2001-2002.
// Created 10.02.2002.
// Modifications:
//	(1) GetFileCopies() added.
//=================================================================

#if !defined(CFilesCopies_h)
#define CFilesCopies_h

#include <afxtempl.h>
#include "CFileCopy.h"

class CFileCopy;
class CFileToArc;

class CFilesCopies : public CList< CFileCopy*, CFileCopy* >
{
public:

    CFileCopy* FindCopy(ID nID) const;
    bool DeleteCopy(CFileCopy* pFileCopy);
    CFileCopy* GetLatestCopy(CFileToArc* pFile) const;
    CFileCopy* GetOldestCopy(CFileToArc* pFile) const;
    /*
     * Return the number of Copies of given File
     */
    int GetCopiesCount( CFileToArc* pFile, ID nRoomID = -1 ) const;
	CFilesCopies* GetFileCopies( CFileToArc* pFile ) const;

protected:


private:


};

#endif /* CFilesCopies_h */
