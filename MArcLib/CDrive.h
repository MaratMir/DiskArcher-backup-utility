#pragma once

#include <afx.h>

class CFolder;
class CDiskItem;

class CDrive
{
public:

    CDrive( const CString& name );
    ~CDrive();
    /*
     * for network drives - long name: \\comp\share
     */
    CString	m_strName;
    /*
     * See WinAPI GetDriveType() description
     */
    UINT	m_nDriveType;
    CFolder*	m_RootFolder;
    CDiskItem	*m_Items;

protected:


private:


};
