//____________________________________________________________________________
/*!

\class    ProgressBar

\brief    Class for progress bar

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\created  Jan 2014
\last update Jan 2014

*/
//____________________________________________________________________________
#ifndef UTILS_PROGRESSBAR_HH
#define UTILS_PROGRESSBAR_HH 1

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <sys/ioctl.h>

class ProgressBar {

  public:
	ProgressBar();
    	~ProgressBar();

	void showPercent(int x,int n,int r,int w);
	inline void showBar(int x,int n,int r,int w);
	inline double getTerminalWidth();

  private:
	double terminalWidth;
	double terminalHeight;

};

#endif
