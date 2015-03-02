//____________________________________________________________________________
/*!

\class    RunManager

\brief    Class for user run manager in geant4

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\created  Sep 2012
\last update Sep 2012

*/
//____________________________________________________________________________
#ifndef RunManager_h
#define RunManager_h 1

#include "GHEPFileReader.hh"
//#include "HitsFileWriter.hh"

#include <G4RunManager.hh>

class RunManager : public G4RunManager {

 public:
  void setReader(GHEPFileReader* r) { reader = r; };
  //void setWriter(HitsFileWriter* w) { writer = w; };
  GHEPFileReader* getReader() const { return reader; };
  //HitsFileWriter* getWriter() const { return writer; };

 private:
  GHEPFileReader* reader;
  //HitsFileWriter* writer;

};

#endif
