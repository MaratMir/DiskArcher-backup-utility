// Error description that can be propagated via the callers stack.
// (C) Marat Mirgaleev, 2014.
// Started 25.01.2014.
//==============================================================================

#include "stdafx.h"
#include "error.h"

namespace MArcLib
{

  const error error::s_default; // The always-existing "Everything is fine" instance

}