#include "stdafx.h"
#include "CProgressBar.h"

#include "MainFrm.h"

CProgressBar::CProgressBar(void)
{
}

CProgressBar::~CProgressBar(void)
{
}

void CProgressBar::init()
{
  // Create the CProgressCtrl as a child of the status bar positioned over the first pane
  CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
  RECT rc;
  pMainFrame->m_wndStatusBar.GetItemRect (0, &rc);
  rc.top += 2;  rc.bottom -= 2;
  VERIFY( wndProgress.Create( WS_CHILD | WS_VISIBLE, rc, &(pMainFrame->m_wndStatusBar), 1 ));
  wndProgress.SetRange( 0, 8 ); // Yes, we have eight steps at the moment
  wndProgress.SetStep(1);
}


void CProgressBar::stepIt()
{
  wndProgress.StepIt();
}