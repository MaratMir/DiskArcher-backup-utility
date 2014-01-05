// DiskArcher.
// CLocatorFolder class interface file.
// Folder which CNewFilesLocator scans for files that are not in Archive yet
//		(recursively with subfolders).
// (C) Marat Mirgaleev, 2002.
// Created 22.10.2002 - extracted from CNewFilesLocator.
// Modifications:
//==========================================================================

#if !defined(CLocatorFolder_h)
#define CLocatorFolder_h

#include "CFolder.h"

class CNewFilesLocator;


class CLocatorFolder : public CFolder
{
public:
    CLocatorFolder( const CString& strFullName,
					CNewFilesLocator* pLocator );
    CLocatorFolder( const CFolder& folder,
					CNewFilesLocator* pLocator );

    void Analyze();

protected:
	CNewFilesLocator*  m_pLocator; // The Locator that controls this object

};

#endif /* CLocatorFolder_h */
