// DiskArcher.
// Interface of the CFileCopy class.
// (C) Marat Mirgaleev, 2001-2003.
// Modifications:
//	(1) 09.02.2002. m_bDeleteIt added.
//	(2) 19.02.2002. GetRoom() added.
//	(4) 26.03.2003. Extract() implemented - the code moved from CCopiesView.
//===========================================================================

#if !defined(CFileCopy_h)
#define CFileCopy_h

/*
 * There can be many copies of a file in Archive
 */

//zzz#include "resource.h"
#include <afx.h>
// ? #include <afxdisp.h>
#include <ATLComTime.h>

#include "MyCommon.h"

class CFileToArc;
class CRoom;

class CFileCopy
{
public:

    ID	m_nCopyID;
    CString	m_strPath;
    /*
     * With path?
     */
    CString	m_strFilename;
    COleDateTime	m_FileDateTime;
    int m_nSize;
    /*
     * How can I get it from zip-file?
     */
    int m_nPackedSize;
    ID m_nBundleID;	// (2) It was wrong:  int m_nBundleID;
    CString	m_strUser;
    /*
     * 09.02.2002. "Delete" flag. If it is true, the program must
     * delete this File from Rooms during Archive Update process.
     */
    bool	m_bDeleteIt;	// (1)

    bool Extract( const CString strExtractToPath ) const; 
									// (4) Was: void Extract();
	bool MarkForDeletion();	// (1)
	CRoom* GetRoom() const;	// (2)

protected:


private:


};

#endif /* CFileCopy_h */
