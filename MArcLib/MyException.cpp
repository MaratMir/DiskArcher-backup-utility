// DiskArcher.
// My exception.
// (C) Marat Mirgaleev, 2014.
// Started 22.01.2014.
//==============================================================================
// LATER: Seems to be a good article:
// http://www.codeproject.com/Articles/207464/Exception-Handling-in-Visual-Cplusplus
//   but I haven't read it yet

#include "stdafx.h"
#include "MyException.h"


int CMyException::ReportError( UINT nType, UINT nMessageID )
{
  UNREFERENCED_PARAMETER( nType );
  UNREFERENCED_PARAMETER( nMessageID );
  MessageBox( NULL, m_message + L".\nThe program will be closed", L"Fatal Error", MB_OK );

  return 0;
}
