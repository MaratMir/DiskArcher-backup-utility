// A lot of samples of bad architecture - everything depends on many things.
//===========================================================================

#include "stdafx_unittest.h"
#include "CppUnitTest.h"

#include <afxcview.h>
//#include "..\FileLocator\CNewFilesLocator.h"
#include "..\CProgressDialog.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MArc2UnitTest
{
  TEST_CLASS(UnitTest)
  {
  public:
/* Has no meaning
    TEST_METHOD(CNewFilesLocator_constructorDestructor_TestMethod)
    {
      CNewFilesLocator locator( NULL );
        // locator.init(); It would be an exception accessing NULL
    }
*/

/* Wants CProgressDialog and InsertDisk
    TEST_METHOD(UIfactory_constructorDestructor_TestMethod)
    {
      UIfactory fac;
    }
*/

/* Wants CMainFrame and CRoomsFrame
    TEST_METHOD(CProgressDialog_constructorDestructor_TestMethod)
    {
      CProgressDialog dlg;
    }
*/
  };
}