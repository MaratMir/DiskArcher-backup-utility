// Something to show a progress.
//=======================================

#pragma once

class IProgressIndicator
{
public:
// zzz  IProgressIndicator(void);
  virtual ~IProgressIndicator(void) {}

  virtual void init() = 0;
  virtual void stepIt() = 0;
};
