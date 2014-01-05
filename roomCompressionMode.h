// roomCompressionMode.h
//

#if !defined(roomCompressionMode_h)
#define roomCompressionMode_h

#include "resource.h"


enum roomCompressionMode
{
    /*
     * Compress all files on this Room, except non-compressable files
     * (jpg, zip, ...)
     */
    rcmAlways = 2,
    /*
     * Compress the file if it is defined for the file.
     * Else don't compress
     */
    rcmAllowed = 0,
    /*
     * Never compress files on the Room because this Room is big and
     * fast
     */
    rcmNever = 1

};

#endif /* roomCompressionMode_h */
