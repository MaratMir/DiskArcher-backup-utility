#if !defined(CBundles_h)
#define CBundles_h

#include <afxtempl.h>
#include "MyCommon.h"
#include "CBundle.h"

class CRoom;

class CBundles : public CList< CBundle*, CBundle* >
{
public:

  CBundle* BundleCreate( CRoom *pRoom, CBundle::bundleType nBundleType );
  CBundle* BundleFind( ID nBundleID );
  bool DeleteBundle( CBundle* pBundle );

protected:


private:


};

#endif /* CBundles_h */
