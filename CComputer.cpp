// CComputer.cpp
//

#include "stdafx.h"
#include "CComputer.h"
#include "CDrive.h"

// M
#include "CFolder.h"


// The sequence is from MSDN 2000 Jan Samples - Filer
void CComputer::GetDrives()
{
	DWORD dwCount;
	const int bufLen = 255;
	char pDriveStrings[ bufLen ];	// It was LPTSTR lpDriveStrings
	bool bContinue=true;
	
	dwCount = GetLogicalDriveStrings( bufLen-1, pDriveStrings);
	if( dwCount > bufLen-1 )
	{
		AfxMessageBox( "Error: dwCount > bufLen-1" );
		bContinue = false;
	}

	if( bContinue )
	{
		int pos=0, prevBegin=0;
		while( true )
		{
			char end = (*(pDriveStrings+pos)), end2 = (*(pDriveStrings+pos+1));
			pos++;
			if( end == 0 )
			{
				CString rootName = pDriveStrings+prevBegin;
				CDrive* drv = new CDrive( rootName );
				CFolder* root = new CFolder( rootName );
				drv->m_RootFolder = root;
		        drv->m_nDriveType = GetDriveType( pDriveStrings+prevBegin );
				m_Drives.AddTail( drv );
				prevBegin = pos;

				if( end2 == 0 )	// Full stop
					break;
			}	
		}
	}
}


CDrive* CComputer::FindDrive(CString name) const
{
	CDrive *found = NULL;
	POSITION pos;
	for( pos = m_Drives.GetHeadPosition(); pos != NULL; )
    {
		CDrive *curDrive = m_Drives.GetNext( pos );
		if( curDrive->m_strName == name )
		{
			found = curDrive;
			break;
		}
	}
	return found;
}


CComputer::~CComputer()
{
// Free all Drives memory
	POSITION pos;
	for( pos = m_Drives.GetHeadPosition(); pos != NULL; )
    {
		CDrive *curDrive = m_Drives.GetNext( pos );
		delete curDrive;
	}
}
