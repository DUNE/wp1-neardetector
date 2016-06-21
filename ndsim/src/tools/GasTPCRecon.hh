//____________________________________________________________________________
/*!

\class    GasTPCRecon

\brief    

\author   Georgios Christodoulou <georgios at hep.ph.liv.ac.uk>
          University of Liverpool

\created  June 2016
\last update June 2016
*/
//____________________________________________________________________________
#ifndef GASTPCRECON_HH
#define GASTPCRECON_HH

#include "UtlXML.hh"

#include <TTree.h>

#include <Ntuple/NtpMCEventRecord.h>
#include <EVGCore/EventRecord.h>

void ReadDatacard(std::string datacard);
void ReadOutputTree(bool truetree = false);

double deltaSagitta, tpcresolution, magfield, angresolution, scintresolution, scintTimeRes, dedxCorMIP, dedxCorElec, gaspressure, fiducialXY, fiducialZ, ecalTimeRes;
int fNSpillsSimulated;
const int kMaxTrack = 1000;

genie::NtpMCEventRecord *fAnalNtpMCEventRecord;
genie::NtpMCEventRecord *fgrec;

NeutrinoEvent  	        *fnuEvent;
SimulData      	        *fsimData;
GeantTrackingTruth	*ftrackingRecord;
SimulDetectorEvent      *fevent;

TTree                   *fOutData;
// Tree with all the truth
TTree                   *fTrueData;

template <class dataType>
void makeOutTreeBranches(dataType* &dataObject) {
  dataObject = new dataType();
  fOutData->Branch(dataObject->getRecordName().c_str(), &dataObject);
}

template <class dataType>
void makeOutTreeBranches2(dataType* &dataObject) { 
  dataObject = new dataType();
  fTrueData->Branch("gmcrec","genie::NtpMCEventRecord", &fAnalNtpMCEventRecord);
  fTrueData->Branch(dataObject->getRecordName().c_str(), &dataObject);
}

#endif
