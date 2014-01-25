#include "stdafx.h"
#include "CppUnitTest.h"

#include "../../MArcLib/MyException.h"

#include "../CFoldersToArc.h"
#include "../CMyArchive.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MArcCoreUnitTest
{
  TEST_CLASS(UnitTest1)
  {
  public:

    // Attempt to add a folder must fail.
    // We also test that no any popup dialog is shown
    TEST_METHOD(CFoldersToArc_TestMethod)
    {
      Logger::WriteMessage( "CFoldersToArc_TestMethod" );
      CFoldersToArc folders;
      const MArcLib::error* res = folders.AddFolder( "XYZ_ABC" );
      Assert::AreEqual( true, res->getSeverity() == MArcLib::error::unsuccessfulOperation );
    }


    //========================================
    TEST_METHOD(CMyArchive_TestMethod)
    {
      // Let's try to create a CMyArchive object
      Logger::WriteMessage( "CMyArchive_TestMethod" );
      CMyArchive archive;
      Assert::AreEqual( archive.m_nDefaultCopies, 5 );

      // Attempt to add a folder must fail 
      const MArcLib::error* res = archive.addFile( "C:\\Temp" );
      Assert::AreEqual( true, res->getSeverity() == MArcLib::error::unsuccessfulOperation );
    }


    //========================================
    TEST_METHOD(CFilesToArc_TestMethod)
    {
      // Let's try to create a CFilesToArc object
      Logger::WriteMessage( "CFilesToArc_TestMethod" );
      CFilesToArc files;

      try
      {
        // It requires a initialized database, but we don't have any, so an exception will be thrown
        files.FileAdd( "XYZ" );
      }
      catch( CMyException* ex )
      {
        CString msg;
        msg.Format( L"  Expected exception: %s", ex->getMessage() );
        Logger::WriteMessage( msg );
      }

    }

  };
}