// DiskArcher.
// Interface of CRooms class.
// (C) Marat Mirgaleev, 2001-2002.
// Created 16.02.2002.
//=================================================================

#if !defined(CRooms_h)
#define CRooms_h

#include <afxtempl.h>
//zzz#include "resource.h"
#include "MyCommon.h"


class CRoom;
//class CString;
class CFileCopy;

class CRooms
	: public CList< CRoom*, CRoom* >	// M
{
public:

    bool RoomCreate();
    CRoom* RoomFind(ID roomID);
    CRoom* RoomFind(CString filename);
    CRoom* RoomFind(CFileCopy* pCopy);
    bool RoomsLoad();
	void RoomsUpdate();
    bool Delete( CRoom* pRoom );

protected:

private:


};

#endif /* CRooms_h */
