// DiskArcher.
// CMyArchive.h - Interface of the CMyArchive class.
// (C) Marat Mirgaleev, 2001-2002.
// Modifications:
//	(1) 19.01.2002. Log file.
//	(2) 10.02.2002. Class CFileCopies created and some methods was
//					moved there.
//	(6) 16.02.2002. Classes CFilesToArc, CBundles and CRooms created 
//					and some methods was moved there.
//	(7) 03.03.2002. m_FoldersToArc added.
//	(9) 03.07.2002. Flags m_bIsWorking and m_bStopWorking added.
// (10) 01.11.2002. theDB changed to *pTheDB.
// (11) 18.11.2002. m_pProgressDlg added (from previos public variable).
// (13) 21.04.2003. Compression utilities. m_pCompressor added.
// (14) 24.07.2004. Locator reconstruction.
// (15) 04.05.2006. More detailed codes of operation success.
//                  Some methods extracted from "spaghetti" code.
// (16) 24.05.2006. pTheDB changed to g_pTheDB everywhere.
//==============================================================================

#if !defined(CMyArchive_h)
#define CMyArchive_h

#include "enums.h" // (16)
#include "CLogFile.h"       // (1)
#include "CFilesCopies.h"	// (2)
#include "CFilesToArc.h"	// (3)
#include "CRooms.h"			// (6)
#include "CBundles.h"		// (6)
#include "CFoldersToArc.h"	// (7)

class CArchiveDB;
class CFileCompressor;
class CProgressDialog;
class CNewFilesLocator; // (14)

const CString MyDBFilename = "MArc.MDB"; // M   TO DO: Is it Ok?
extern CArchiveDB* g_pTheDB;	// (16) Was: extern CArchiveDB theDB;	


class CMyArchive
{

public:

	CMyArchive();
  ~CMyArchive();

	bool open();     // Open the Archive
	bool close();
	OpResult update();  // (16) Was: int Update();

  bool saveOptions(); // Store the Archive's parameters into DB
  CString getTempPath()  { return m_sTempPath; }	// (13)
  bool isCompressorDefined();						// (13)

// Events
  void onLocatorStart( void );


  CID	m_strArchiveID;
  int	m_nDefaultCopies;
	bool m_bIsWorking;  // (9) Is archive update working at the moment
	bool m_bStopWorking;  // (9) Flag "Stop working now!"

	CRooms m_Rooms;	// (6) Was CList< CRoom*, CRoom* > m_Rooms;

	// (12)    CExcludedFile	*m_ExcludedFiles;

  CFilesToArc	m_FilesToArc;
    // (3) CList< CFileToArc*, CFileToArc* > m_FilesToArc;

	CFoldersToArc m_FoldersToArc;	// (7)

  CArchiveDB	*m_pDB;
  CNewFilesLocator* m_pLocator;   // (14)
  CFilesCopies m_Copies;

  CBundles m_Bundles;
		// Was (6) CList< CBundle*, CBundle* > m_Bundles;
	CLogFile m_LogFile;	// (1)
  CFileCompressor	*m_pCompressor;
	CProgressDialog *m_pProgressDlg;  
						// The Dialog for displaying the archiving progress.
						//	(11) Moved here from a public variable.

protected:

  OpResult /*(16) Was: bool*/ doCopying();
  OpResult /*(16) Was: bool*/ decideAboutFile( CFileToArc* const i_pFile );
  bool deleteOldestCopyOfFile( CFileToArc* const i_pFile );  // (16)
  OpResult addCopyOfFile( CFileToArc* const i_pFile );  // (16)
  bool loadOptions(); // Read the Archive's parameters from the database

	int m_nFilesToUpdate;
	CString m_sTempPath;

};

#endif /* CMyArchive_h */
