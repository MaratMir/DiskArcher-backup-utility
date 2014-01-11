// Show a progress.
//=======================================

#pragma once

#include "MArcCore\IProgressIndicator.h"

class CProgressBar : public IProgressIndicator
{
public:
  CProgressBar(void);
  ~CProgressBar(void);

  virtual void init();
  virtual void stepIt();

protected:
  CProgressCtrl wndProgress;
};
