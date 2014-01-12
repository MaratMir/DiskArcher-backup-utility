// DiskArcher.
// Compression Utility.
// (C) Marat Mirgaleev, 2001-2003.
//	Created 20.09.2003.
//==================================================================

#if !defined(CFileCompressor_h)
#define CFileCompressor_h

#include <afx.h>


class CFileCompressor
{
public:

    /*
     * The name of the Compression Utility
     */
    CString	m_strName;
    /*
     * Which file type supports the Compression Utility
     */
    CString	m_strExtension;
    CString	m_strExePath;

protected:


private:

    bool	bSupportsLongNames;

};

#endif /* CFileCompressor_h */
