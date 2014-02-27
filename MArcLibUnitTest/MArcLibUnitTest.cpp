#include "stdafx.h"
#include "CppUnitTest.h"

#include "CDrive.h"
#include "CFileOnDisk.h"
#include "CFilesOnDisk.h"
#include "error.h"

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


    TEST_METHOD(CDiskItem_TestMethod)
    {
      {
        CDiskItem item( "readme" ); // No extension
        CString ext = item.getExtension();
        Assert::AreEqual( ext, L"" );
      }
      {
        CDiskItem item2( "readme.txt" );
        CString ext2 = item2.getExtension();
        Assert::AreEqual( ext2, L"txt" );
      }
    }


    TEST_METHOD(CFilesOnDisk_TestMethod)
    {
      CFilesOnDisk files;
      Logger::WriteMessage( "CFilesOnDisk_TestMethod started." );

      // !!! CFileOnDisk aFile( "X:\\WWW\\YYY.UUU" );
      //   Adding an object created on stack leads to an exception in the collection destruction
      CFileOnDisk* aFile = new CFileOnDisk( "X:\\WWW\\YYY.UUU" );
      files.m_files.push_back( aFile );
      files.m_files.push_back( new CFileOnDisk( "A:\\BBB\\CCC.DDD" ) );

      int removed = files.remove( "YOKLMN", "OPRST" );
      Assert::AreEqual( removed, 0 );
      Assert::IsTrue( files.m_files.size() == 2 );
      removed = files.remove( "X:\\WWW\\", "YYY.UUU" );
      Assert::AreEqual( removed, 1 );
      Assert::IsTrue( files.m_files.size() == 1 );

      Logger::WriteMessage( "  CFilesOnDisk_TestMethod ended." );
    }


    TEST_METHOD(error_TestMethod)
    {
      MArcLib::error result;
      Logger::WriteMessage( "error_TestMethod" );
      Assert::AreEqual( true, result.getSeverity()==MArcLib::error::everythingIsFine );

      const MArcLib::error* defResult = MArcLib::error::getDefault();
      Assert::AreEqual( true, defResult->getSeverity()==MArcLib::error::everythingIsFine );
    }

  };
}