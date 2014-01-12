// DiskArcher.
// Log file.
// (C) Marat Mirgaleev.
// Added 12 Jan 2002.
//=====================================================================

#if !defined(CLogFile_h)
#define CLogFile_h

class CMyArchive;

class CLogFile
{
public:

  CLogFile();
  bool AddRecord(CString strFilePath, CString strFilename, CString strMessage);
  void EraseAll();
  CMyArchive *m_pArchive;

protected:


private:


};

#endif /* CLogFile_h */
