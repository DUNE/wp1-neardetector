//____________________________________________________________________________
/*!

\class    GeometryLoader

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\created  Dec 2012
\last update Jun 2013

*/
//____________________________________________________________________________
#include "GeometryLoader.hh"

GeometryLoader::GeometryLoader(){

  initDataCards();
  initialize();

}

GeometryLoader::GeometryLoader(std::string filename){

  initialize(filename);
}

GeometryLoader::~GeometryLoader(){

  //this->close();
}

void GeometryLoader::initialize(){

  loadDataCards();

  geoMgr_ = TGeoManager::Import(rootFile_.c_str());
  std::cout<< "\n*****************************"
	   << "\nSetting root geometry manager"
	   << "\n*****************************" << std::endl;

  double min=0.0, max=0.0;
  TGeoShape* bounding_box = geoMgr_->GetTopVolume()->GetShape();
  xRange_ = bounding_box->GetAxisRange(1,min, max);
  yRange_ = bounding_box->GetAxisRange(2,min, max);
  zRange_ = bounding_box->GetAxisRange(3,min, max);
  //root gives units of cm
  xRange_ *= CLHEP::cm;
  yRange_ *= CLHEP::cm;
  zRange_ *= CLHEP::cm;
}

void GeometryLoader::initialize(std::string filename){

  geoMgr_ = TGeoManager::Import(filename.c_str());
  std::cout<< "\n*****************************"
	   << "\nSetting root geometry manager"
	   << "\n*****************************" << std::endl;

  double min=0.0, max=0.0;
  TGeoShape* bounding_box = geoMgr_->GetTopVolume()->GetShape();
  xRange_ = bounding_box->GetAxisRange(1,min, max);
  yRange_ = bounding_box->GetAxisRange(2,min, max);
  zRange_ = bounding_box->GetAxisRange(3,min, max);
  //root gives units of cm
  xRange_ *= CLHEP::cm;
  yRange_ *= CLHEP::cm;
  zRange_ *= CLHEP::cm;
}

void GeometryLoader::initDataCards() {
  //cards_ = DataCards::getInstance();
  char* LBNO = getenv("LBNO");

  cards_->AddDataCardString("geomFileName", string(LBNO) + "/data/geometry/geom.root");
  cards_->AddDataCardString("targetVolume", "top");
  cardsLoaded_ = false;
}

void GeometryLoader::loadDataCards() {

  rootFile_    	= cards_->fetchValueString("geomFileName");
  targetVolume_ = cards_->fetchValueString("targetVolume");
  cardsLoaded_ 	= true;
}

void GeometryLoader::close(){

  geoMgr_->CloseGeometry();

}

TGeoVolume * GeometryLoader::getWorldVolume(){

  World_ = geoMgr_->GetTopVolume();
  return World_;	

}

TGeoVolume * GeometryLoader::getTPCVolume(){

  Tpc_ = geoMgr_->GetVolume("tpcFidVolume");
  return Tpc_;	

}

double GeometryLoader::getVolumeXBoundary(TGeoVolume * boxVolume){
  double min=0, max=0;
  TGeoShape * bounding_box = boxVolume->GetShape();
  double xrange = bounding_box->GetAxisRange(1, min, max);
  return xrange*CLHEP::cm;
}

double GeometryLoader::getVolumeYBoundary(TGeoVolume * boxVolume){
  double min=0, max=0;
  TGeoShape * bounding_box = boxVolume->GetShape();
  double yrange = bounding_box->GetAxisRange(2, min, max);
  return yrange*CLHEP::cm;
}

double GeometryLoader::getVolumeZBoundary(TGeoVolume * boxVolume){
  double min=0, max=0;
  TGeoShape * bounding_box = boxVolume->GetShape();
  double zrange = bounding_box->GetAxisRange(3, min, max);
  return zrange*CLHEP::cm;
}
