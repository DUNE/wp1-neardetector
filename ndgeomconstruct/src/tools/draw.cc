//
// Title: Draw root geometry
//
// Description: to be written
//
// Author: Tom Stainer
//

#include "draw.hh"

//___________________________________________________________________
int main(int argc, char ** argv)
{

  //load the geometry library
  TSystem * system = new TSystem("system");
  system->Load("libGeom"); 

  //initiate data cards
  initDataCards();

  //get the data card
  getRunTimeArguments(argc,argv);

  //load data cards
  loadDataCards();

  TGeoManager::Import(rootFilename_.c_str());

  //TGeoVolume * Rock_Volume = gGeoManager->GetVolume("Rock_Volume");
  //TGeoVolume * Cavity_Volume = gGeoManager->GetVolume("Cavity_MIND_Volume");
  gGeoManager->SetTopVisible(kFALSE);
  gGeoManager->SetVisOption(0);
  gGeoManager->SetVisLevel(10);
  gGeoManager->GetMasterVolume()->Draw("gl");
}

void getRunTimeArguments(int argc, char ** argv) {
  for (int i=1;i<argc;i++)
    if (argv[i]) {
      std::string cardsfile(argv[i]);
      cards_->readKeys(cardsfile);
    }
    cards_->printToStream(std::cout);
}

void initDataCards() {
  cards_ = DataCards::getInstance();
  cards_->AddDataCardString("rootFile","geometry.root");
}

void loadDataCards() {
  rootFilename_ = cards_->fetchValueString("rootFile");

}