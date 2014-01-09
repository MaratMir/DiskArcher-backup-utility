#include "stdafx.h"
#include "CppUnitTest.h"

#include "CDrive.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MArcLibUnitTest
{
  TEST_CLASS(MArcUnitTest)
  {
  public:
    
    TEST_METHOD(CDrive_TestMethod)
    {
      CDrive drive("C:");
      Logger::WriteMessage(drive.m_strName);
      Assert::AreEqual(drive.m_nDriveType, (UINT)DRIVE_FIXED);
    }

  };
}