// DiskArcher.
// CLogFile.h - Interface of the CLogFile class.
// (C) Marat Mirgaleev, 2001-2002.
// Created 19.01.2001.
//=====================================================================

#if !defined(CLogFile_h)
#define CLogFile_h

/*
 * Log file.
 * Added 12 Jan 2002.
 */

#include "resource.h"


//class CString;
class CMyArchive;

class CLogFile
{
public:

    CLogFile();
    bool AddRecord(CString strFilePath, CString strFilename, CString strMessage);
    void EraseAll();
    CMyArchive	*m_pArchive;

protected:


private:


};

#endif /* CLogFile_h */
