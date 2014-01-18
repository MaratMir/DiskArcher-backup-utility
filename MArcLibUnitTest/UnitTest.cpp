#include "stdafx.h"
#include "CppUnitTest.h"

#include "CDrive.h"
#include "CFileOnDisk.h"
#include "CFilesOnDisk.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MArcLibUnitTest
{
  TEST_CLASS(MArcLibUnitTest)
  {
  public:
    
    TEST_METHOD(CDrive_TestMethod)
    {
      CDrive drive("C:");
      Logger::WriteMessage( "CDrive_TestMethod: " + drive.m_strName );
      Assert::AreEqual(drive.m_nDriveType, (UINT)DRIVE_FIXED);
    }


    TEST_METHOD(CFilesOnDisk_TestMethod)
    {
      CFilesOnDisk files;
      Logger::WriteMessage( "CFilesOnDisk_TestMethod" );

      // !!! CFileOnDisk aFile( "X:\\WWW\\YYY.UUU" );
      //   Creating an object on stack leads to an exception in the collection destruction
      CFileOnDisk* aFile = new CFileOnDisk( "X:\\WWW\\YYY.UUU" );
      files.AddTail( aFile );
      CFileOnDisk* found = files.Find( "X:\\WWW\\", "YYY.UUU" );

      // VS test somehow can't compare two CFileOnDisk*, let's cast to void* then
      void* ptr1 = aFile;
      void* ptr2 = found;
      Assert::AreEqual( ptr1, ptr2 );
    }

  };
}