// DiskArcher.
// IProgressCtrl - shows the progress and lets stop it.
// (C) Marat Mirgaleev, 2001-2014.
//==================================================================

#pragma once

#include <afxstr.h>

class IProgressCtrl
{

public:

  virtual void setPos( int newPos ) = 0;
  virtual void setMessage( CString strMessage ) = 0;
  virtual void advance( int nAmount ) = 0;
  virtual void setMaxRange( int nMax ) = 0;
  virtual void resetAndShow() = 0;
  virtual bool isAborted() = 0; // Check if the user pressed "Stop"
  virtual void analysisDone() = 0;
  virtual void finished( bool i_stopWorking, OpResult i_result ) = 0;

/*zzz
bool m_bIsAborted;	// Flag, is "Stop" button pressed
	
*/

};
