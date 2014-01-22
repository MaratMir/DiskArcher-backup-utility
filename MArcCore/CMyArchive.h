// DiskArcher.
// CMyArchive - the main class of the application business logic.
// (C) Marat Mirgaleev, 2001-2014.
// Modifications:
//  (1) 19.01.2002. Log file.
//  (2) 10.02.2002. Class CFileCopies created and some methods was moved there.
//  (6) 16.02.2002. Classes CFilesToArc, CBundles and CRooms created and some methods was moved there.
//  (7) 03.03.2002. m_FoldersToArc added.
//  (9) 03.07.2002. Flags m_bIsWorking and m_bStopWorking added.
// (10) 01.11.2002. theDB changed to *pTheDB.
// (11) 18.11.2002. m_pProgressDlg added (from previos public variable).
// (13) 21.04.2003. Compression utilities. m_pCompressor added.
// (14) 24.07.2004. Locator reconstruction.
// (15) 04.05.2006. More detailed codes of operation success.
//                  Some methods extracted from "spaghetti" code.
// (16) 24.05.2006. pTheDB changed to g_pTheDB everywhere.
// (17) 23.12.2013. Migration to VS 2012 and Unicode.
//                  See the latest modification history on Github.
//==============================================================================

#if !defined(CMyArchive_h)
#define CMyArchive_h

#include "enums.h"
#include "CLogFile.h"
#include "CFilesCopies.h"
#include "CFilesToArc.h"
#include "CRooms.h"
#include "CBundles.h"
#include "CFoldersToArc.h"

class CArchiveDB;
class CFileCompressor;
class IProgressIndicator;
class IProgressCtrl;
class IInsertDisk;
class IFilesLocator;

extern CMyArchive g_TheArchive; // 2014 moved from MArc2.h. TODO: Get rid of global variables
const CString MyDBFilename = L"MArc.MDB"; // M   TODO: Is it Ok? Move to CArchiveDB??


class CMyArchive
{

public:

  CID m_strArchiveID;
  int m_nDefaultCopies;
  bool m_bIsWorking;  // (9) Is archive update working at the moment
  bool m_bStopWorking;  // (9) Flag "Stop working now!"

  CArchiveDB *m_pDB;
  CLogFile m_LogFile;
  CFileCompressor *m_pCompressor;

  CRooms m_Rooms;
  CFilesToArc m_FilesToArc;
  CFoldersToArc m_FoldersToArc;
  IFilesLocator* m_pLocator;
  CFilesCopies m_Copies;
  CBundles m_Bundles;

  CMyArchive();
  ~CMyArchive();
// TODO  CArchiveDB* GetDB()  { return m_pDB; }
  bool open( IProgressIndicator* i_pProgressIndicator ); // Open the Archive
  bool close();
  OpResult update();
  bool saveOptions(); // Store the Archive's parameters into DB
  CString getTempPath()  { return m_sTempPath; }
  bool isCompressorDefined();
  void setProgressDlg( IProgressCtrl* i_pProgressDlg )  { m_pProgressDlg = i_pProgressDlg; }
  IProgressCtrl* getProgressDlg() const  { return m_pProgressDlg; }
  void setInsertDlg( IInsertDisk* i_insertDlg ) { m_insertDiskDlg = i_insertDlg; }
  IInsertDisk* getInsertDlg() const { return m_insertDiskDlg; }

// Events
// void onLocatorStart( void );


protected:

  int m_nFilesToUpdate;
  CString m_sTempPath;
  IProgressIndicator* m_pProgressIndicator; // Just to show a progress bar
  IProgressCtrl* m_pProgressDlg;  // The Dialog for displaying the archiving progress. Lets stop too
  IInsertDisk* m_insertDiskDlg; // "Please, insert removable disk #" dialog

  OpResult doCopying();
  OpResult decideAboutFile( CFileToArc* const i_pFile );
  bool deleteOldestCopyOfFile( CFileToArc* const i_pFile );
  OpResult addCopyOfFile( CFileToArc* const i_pFile );
  bool loadOptions(); // Reads the Archive's parameters from the database

};

#endif /* CMyArchive_h */
