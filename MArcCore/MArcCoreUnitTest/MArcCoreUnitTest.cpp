#include "stdafx.h"

#include "../../MArcLib/MyException.h"

#include "../CFoldersToArc.h"
#include "../CMyArchive.h"
#include "../CRoom.h"
#include "../CRooms.h"

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
      Assert::AreEqual( true, res->getSeverity() == MArcLib::error::severity::unsuccessfulOperation );
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
      Assert::AreEqual( true, res->getSeverity() == MArcLib::error::severity::unsuccessfulOperation );
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


    //========================================
    TEST_METHOD(CFileCopy_TestMethod)
    {
      CFileCopy fc;
      fc.m_size = 0xaaaabbbbccccdddd;
      Assert::AreEqual( true, fc.getSizeHi() == 0xaaaabbbb );
      Assert::AreEqual( true, fc.getSizeLow() == 0xccccdddd );

      fc.setSize( 0x11112222, 0x33334444 );
      Assert::AreEqual( true, fc.m_size == 0x1111222233334444 );
      Assert::AreEqual( true, fc.getSizeHi() == 0x11112222 );
      Assert::AreEqual( true, fc.getSizeLow() == 0x33334444 );
    }
    TEST_METHOD(CFileCopy_packedSize_TestMethod)
    {
      CFileCopy fc;
      fc.m_packedSize = 0xaaaabbbbccccdddd;
      Assert::AreEqual( true, fc.getPackedSizeHi() == 0xaaaabbbb );
      Assert::AreEqual( true, fc.getPackedSizeLow() == 0xccccdddd );

      fc.setPackedSize( 0x11112222, 0x33334444 );
      Assert::AreEqual( true, fc.m_packedSize == 0x1111222233334444 );
      Assert::AreEqual( true, fc.getPackedSizeHi() == 0x11112222 );
      Assert::AreEqual( true, fc.getPackedSizeLow() == 0x33334444 );
    }


    TEST_METHOD(CRoom_TestMethod)
    {
      Logger::WriteMessage( "CRoom_TestMethod" );
      CRoom room( L"C:\\Temp");
      bool res = room.GetDiskSpaceFree();
      Assert::IsTrue( res );
      Assert::IsTrue( room.m_nDiskSpaceFree >= 0 );
    }


    TEST_METHOD(CRooms_TestMethod)
    {
      Logger::WriteMessage( "CRooms_TestMethod" );

      CRoom *room1 = new CRoom( L"C:\\Temp");
      room1->m_nRoomID = 333;

      CRoom *room2 = new CRoom( L"C:\\Windows");
      room2->m_nRoomID = 777;

      CRooms rooms;
      rooms.m_rooms.push_back( room1 );
      rooms.m_rooms.push_back( room2 );

      CRoom *found = rooms.find( L"C:\\Windows" );
      Assert::IsTrue( room2 == found );

      found = rooms.find( 333 );
      Assert::IsTrue( room1 == found );

    }

  }; // class
} // namespace