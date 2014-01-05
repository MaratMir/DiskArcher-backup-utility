// CDrive.cpp
//

#include "stdafx.h"
#include "CDrive.h"
#include "CFolder.h"
#include "CDiskItem.h"

CDrive::CDrive( const CString& name )
{
	m_strName = name;
	m_nDriveType = GetDriveType( name );
}


CDrive::~CDrive()
{
}


