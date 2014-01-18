#include "stdafx_unittest.h"
#include "CppUnitTest.h"

#include <afxcview.h>
#include "..\FileLocator\CNewFilesLocator.h"
#include "..\FileLocator\CNewFilesLocatorDoc.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MArc2UnitTest
{
  TEST_CLASS(UnitTest)
  {
  public:

    TEST_METHOD(CNewFilesLocator_constructorDestructor_TestMethod)
    {
      CNewFilesLocator locator( NULL );
        // locator.init(); It would be an exception accessing NULL
    }

    // This is not a big deal, but at least it doesn't crash and doesn't return a pointer to trash.
    // I'd like to test getView() more seriously, but it's not easy to create a real document with views.
    TEST_METHOD(CNewFilesLocatorView_getView_TestMethod)
    {
      CNewFilesLocatorDoc doc;
      CNewFilesLocatorView* view = doc.getView();
      Assert::IsNull( view );
    }

  };
}