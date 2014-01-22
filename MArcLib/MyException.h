// DiskArcher.
// My exception.
// (C) Marat Mirgaleev, 2014.
// Started 22.01.2014.
//==============================================================================

#pragma once

#include "afx.h"

class CMyException : public CException
{
public:

  CMyException( CString i_message ) : CException()
    { m_message = i_message; }

  virtual ~CMyException(void) {}

  virtual int ReportError(UINT nType = MB_OK, UINT nMessageID = 0);

private:
  CString m_message; 
};
