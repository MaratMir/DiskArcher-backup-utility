#include "stdafx.h"
#include <afx.h>
#include "CppUnitTest.h"

#include "CDrive.h"
#include "CFileOnDisk.h"
#include "CFilesOnDisk.h"
#include "CFolder.h"
#include "error.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MArcLibUnitTest
{
  TEST_CLASS(MArcLibUnitTest)
  {
  public:
    
    TEST_METHOD(CDrive_TestMethod)
    {
      {
        MArcLib::CDrive drive1(L"C:"); // A slash will be added
        Logger::WriteMessage( ( L"CDrive_TestMethod: " + drive1.m_name ).c_str() );
        Assert::IsTrue( drive1.m_name == L"C:\\" );
        Assert::AreEqual(drive1.m_driveType, (UINT)DRIVE_FIXED);
      }
      {
        MArcLib::CDrive drive2(L"D:\\"); // Don't need to add a slash, it's already there
        Assert::IsTrue( drive2.m_name == L"D:\\" );
      }
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


    TEST_METHOD(CFolder_TestMethod)
    {
      // TODO: Make the path configurable or relative
      CFolder folder( "Z:\\_Marat\\DiskArcher\\src\\MArcLibUnitTest\\ForCFolderUnitTest" );
      folder.getInfo();
      folder.getItems();
      Assert::IsTrue( folder.m_items.size() == 1 );

      CDiskItem *item = folder.m_items[0]; // There is only one file
      item->getInfo();
      long long size = item->getSize(); 
      Assert::IsTrue( size == 55 );

      folder.clear();
      Assert::IsTrue( folder.m_items.size() == 0 );
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
      Assert::AreEqual( true, result.getSeverity()==MArcLib::error::severity::everythingIsFine );

      const MArcLib::error* defResult = MArcLib::error::getDefault();
      Assert::AreEqual( true, defResult->getSeverity()==MArcLib::error::severity::everythingIsFine );

      Assert::IsTrue( MArcLib::error::severity::forInformation > MArcLib::error::severity::everythingIsFine );
    }

  };
}