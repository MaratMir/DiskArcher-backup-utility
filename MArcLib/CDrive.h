// DiskArcher.
// CDrive class - a disk drive.
//   Now I would call it simply Drive, but that's a historical style comming from VS.
// (C) Marat Mirgaleev, 2004-2014.
//================================================================================

#pragma once

#include <string>
#include <Windows.h>

namespace MArcLib
{

  class CDrive
  {
  public:

    CDrive( const std::wstring& name ); // Don't create it with an empty string

    std::wstring m_name;  // For network drives - long name: \\comp\share
    UINT m_driveType;     // See WinAPI GetDriveType() description
    
    /* They were here, but not used:
    CFolder* m_rootFolder;
    CDiskItem *m_items;
    */

  };

}