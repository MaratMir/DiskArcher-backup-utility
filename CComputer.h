// CComputer.h
//

#if !defined(CComputer_h)
#define CComputer_h

#include "resource.h"

#include <afxtempl.h> // M

class CDrive;
//class CString;

class CComputer
{
public:

    /*
     * The sequence is from MSDN 2000 Jan Samples - Filer
     */
    void GetDrives();
    CDrive* FindDrive(CString name) const;
    ~CComputer();
    CList < CDrive*, CDrive* > m_Drives;
		// M CDrive	*m_Drives;

protected:


private:


};

#endif /* CComputer_h */
