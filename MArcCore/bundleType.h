// bundleType.h
//

#if !defined(bundleType_h)
#define bundleType_h

/*
 * Bundle Types
 */

//zzz#include "resource.h"


enum bundleType
{
    /*
     * Compressed file in ZIP format
     */
    btZipFile = 1,
    /*
     * Uncompressed, unchanged file
     */
    btSingleFile = 0

};

#endif /* bundleType_h */
