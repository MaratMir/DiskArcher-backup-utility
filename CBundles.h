// CBundles.h
//

#if !defined(CBundles_h)
#define CBundles_h

#include <afxtempl.h>
#include "resource.h"
#include "MyCommon.h"
#include "bundleType.h"


class CBundle;
class CRoom;

class CBundles
	: public CList< CBundle*, CBundle* >	// M
{
public:

	CBundle* BundleCreate( CRoom *pRoom, bundleType nBundleType );
    CBundle* BundleFind( ID nBundleID );
	bool DeleteBundle( CBundle* pBundle );

protected:


private:


};

#endif /* CBundles_h */
