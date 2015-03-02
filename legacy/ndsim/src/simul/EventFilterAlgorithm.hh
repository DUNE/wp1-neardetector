//____________________________________________________________________________
/*!

\class    EventFilterAlgorithm

\brief    Class to perform cuts on events

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\created  Sep 2012
\last update Jan 2014

*/
//____________________________________________________________________________
#ifndef SIMUL_EVENTFILTERALGORITHM_H
#define SIMUL_EVENTFILTERALGORITHM_H 1

#include <string>

#include "DataCards.hh"
#include "LbnoDataLib.hh"

class EventFilterAlgorithm {
 public:
  EventFilterAlgorithm() {}
  virtual ~EventFilterAlgorithm() {}

  bool Process(NeutrinoEvent* event,bool filterOn);

  void initDataCards() {};
  void loadDataCards() {};

 private:
  DataCards *cards_;
  bool      cardsLoaded_;
};

#endif


