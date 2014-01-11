// A dialog to ask to insert a disk.
//===================================

#pragma once

class CRoom;

class IInsertDisk
{
public:
//zzz  IInsertDisk(void) {}
  virtual ~IInsertDisk(void) {}

  virtual int askInsertDiskForCopy( const CRoom* const i_pRoom )=0;
  virtual int askInsertDiskForExtract( const CRoom* const i_pRoom )=0;
};
