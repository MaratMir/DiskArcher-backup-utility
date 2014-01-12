// DiskArcher.
// File status definition.
// (C) Marat Mirgaleev, 2001-2002.
// Modifications:
//	(1) 09.02.2002. fsSystem added.
//	(2) 07.03.2002. fsPaused added.
//=================================================================

#if !defined(fileStatus_h)
#define fileStatus_h

/*
 * !!! Keep the order of constants the same as the order of bitmaps!
 * 
 * BUT WHY MODELER DOES THE MESS IN THEIR ORDER???
 */


enum fileStatus
{
  fsNotFound = 0,
  fsUpToDate = 1,
  fsNew      = 2,
  fsChanged  = 3,
  fsOlder    = 4,
  fsSystem   = 5,
  fsPaused   = 6
};

#endif /* fileStatus_h */
