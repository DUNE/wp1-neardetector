//____________________________________________________________________________
/*!

\class    NuMuCCAnalysis

\brief    Selection of numu CC candidates

\author   Georgios Christodoulou <georgios at hep.ph.liv.ac.uk>
          University of Liverpool

\created  June 2016
\last update June 2016
*/
//____________________________________________________________________________
#ifndef NUMUCCANALYSIS_HH
#define NUMUCCANALYSIS_HH

#include "UtlXML.hh"

#include <TTree.h>

#include <Ntuple/NtpMCEventRecord.h>
#include <EVGCore/EventRecord.h>

void ReadDatacard(std::string datacard);

NeutrinoEvent  	        *fnuEvent;
SimulDetectorEvent      *fevent;


// Selection criteria
double MinTrackLength;
double MuonPull, ElecPull;

#endif
