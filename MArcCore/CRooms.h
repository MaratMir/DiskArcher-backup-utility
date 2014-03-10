// DiskArcher.
// CRooms class - collection of Rooms.
// (C) Marat Mirgaleev, 2001-2014.
// Created 16.02.2002.
//=================================================================

#pragma once

#include <vector>
#include "MyCommon.h"

class CRoom;
class CFileCopy;

class CRooms
{

public:

  bool createRoom();                // Create a new Archive Room

  // Find a Room in the collection
  CRoom* find( ID roomID );         // by ID
  CRoom* find( CString filename );  // by the Room folder name
  CRoom* find( CFileCopy* pCopy );  // by one of the copies from this Room

  bool load();                // Load the Room list from the DB into the memory
  void update();              // Get info about the Rooms: disk space etc.
  bool erase( CRoom* pRoom ); // Get rid of the Room - delete copies etc.

  void free();                // Just cleans, i.e. removes all the Rooms from the memory


public:

  std::vector<CRoom*> m_rooms;  // Making it private doesn't make my life easier

};
