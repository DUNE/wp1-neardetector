//____________________________________________________________________________
/*!

\class    ProgressBar

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\created  Jan 2014
\last update Jan 2014

*/
//____________________________________________________________________________
#include "ProgressBar.hh"

ProgressBar::ProgressBar() : terminalWidth(0.){

	struct winsize w;
	ioctl(0, TIOCGWINSZ, &w);

	terminalWidth = w.ws_col;
	terminalHeight = w.ws_row;
}

ProgressBar::~ProgressBar(){
}

void ProgressBar::showPercent(int x,int n,int r,int w){

    //set r to zero to enable default refresh rate for every 10th entry
    if (r == 0) r = n*0.1;
    if (w == 0) w = terminalWidth - 25;

    if (r < n) r =n;

    // Only update r times.
    if ( x % (n/r) != 0 ) return;
 
    // Calculuate the ratio of complete-to-incomplete.
    float ratio = x/(float)n;
    int   c     = ratio * w;
 
    // Show the percentage complete.
    printf("Progress: [%3d%%] [", (int)(ratio*100) );
 
    // Show the load bar.
    for (int x=0; x<c; x++)
       printf("=");
 
    for (int x=c; x<w; x++)
       printf(" ");
 
    // ANSI Control codes to go back to the
    // previous line and clear it.
    printf("]\n\033[F\033[J");
}

inline void ProgressBar::showBar(int x,int n,int r,int w){

} 

inline double ProgressBar::getTerminalWidth(){

	struct winsize w;
	ioctl(0, TIOCGWINSZ, &w);

	terminalWidth = w.ws_col;
	return terminalWidth;

}

