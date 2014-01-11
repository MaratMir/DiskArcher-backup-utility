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

//zzz#include "resource.h"


enum fileStatus
{
    fsOlder = 4,
    fsChanged = 3,
    fsNew = 2,
    fsUpToDate = 1,
    fsNotFound = 0,

	fsSystem = 5,	// (1)
	fsPaused = 6	// (2)
};

#endif /* fileStatus_h */
