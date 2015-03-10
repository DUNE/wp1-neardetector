//____________________________________________________________________________
/*!

\class    EventFilterAlgorithm

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

author   Georgios Christodoulou <georgios at hep.ph.liv.ac.uk>
University of Liverpool
\created  Sep 2012
\last update Jan 2014

*/
//____________________________________________________________________________
#include "EventFilterAlgorithm.hh"

bool EventFilterAlgorithm::Process(NeutrinoEvent* event,bool filterOn) {

  ParticleDescrShortRecord fspl = event->getFspl();
  std::vector<ParticleDescrShortRecord> fssVector = event->getFssVector();
  int fssSize = fssVector.size();
  //if (fspl.getPDG() == 13)
  //  return true;

  bool ccqe = false;
  bool protonSet = false;

  //loop over secondaries
  if(event->getQELCC() == true && fssSize <= 2 && fspl.getPDG() == 13){
    for(int index=0;index<fssSize; index++){
	int nuEvFssPDG = fssVector.at(index).getPDG();
	
	//ccqe event should have one proton and possibly a recoil nucleus
 	if(nuEvFssPDG == 2212 && !protonSet){
		ccqe = true;
		protonSet = true;
	}
	else if(protonSet && nuEvFssPDG == 2212)ccqe = false;
    }
  }

  if(filterOn){
  	if(ccqe)return true;
  	else return false;
  }
  else return true;

  //event->printToStream(std::cout);
  //std::cout << "*** This event will be skipped ***" << std::endl;

}

