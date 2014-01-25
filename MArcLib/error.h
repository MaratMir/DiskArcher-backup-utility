// Error description that can be propagated via the callers stack.
// (C) Marat Mirgaleev, 2014.
// Started 25.01.2014.
//==============================================================================

#pragma once

namespace MArcLib
{

  // You create an error object in one place and delete it in another, only when you have
  //   completely processed it.
  //===========================================================================================
  class error
  {
  public:
    enum severity { everythingIsFine, forInformation, warning, unsuccessfulOperation, fatal };

    error()
      { m_severity = everythingIsFine; m_message = "Success"; }
    error( const severity i_severity, const CString i_message )
      { m_severity = i_severity; m_message = i_message; }
    virtual ~error(void) {}

    static const error * const getDefault() { return &s_default; }

    severity getSeverity() const { return m_severity; }
    CString  getMessage()  const { return m_message;  }

  private:
    severity m_severity;
    CString  m_message;

    static const error s_default;
  };

}