// DiskArcher.
// CDrive class - a disk drive.
//   Now I would call it simply Drive, but that's a historical style comming from VS.
// (C) Marat Mirgaleev, 2004-2014.
//===================================================================================

#include "stdafx.h"
#include "CDrive.h"
#include "CFolder.h"
#include "CDiskItem.h"

namespace MArcLib
{

  CDrive::CDrive( const std::wstring& name )
  {
    m_name = name;

    if( m_name[ m_name.length()-1 ] != '\\' ) // Add a slash at the end, if there isn't
      m_name = m_name + L"\\";
        // Without this slash drive type works in NT, but in 98 it says "DRIVE_NO_ROOT_DIR 1"

    m_driveType = GetDriveType( name.c_str() );
  }

}
