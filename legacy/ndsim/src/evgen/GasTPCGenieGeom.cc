//____________________________________________________________________________
/*!

\class    GasTPCGenieGeom

\author	  Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

author   Georgios Christodoulou <georgios at hep.ph.liv.ac.uk>
University of Liverpool
\created  Oct 2013
\last update Nov 2013

*/
//____________________________________________________________________________
#include "GasTPCGenieGeom.hh"

GasTPCGenieGeom::GasTPCGenieGeom(TGeoManager * geoMgr) {

  geoMgr_ = geoMgr;
  targetVolume_    = DataCards::getInstance()->fetchValueString("targetVolume");

}

GasTPCGenieGeom::~GasTPCGenieGeom() {

}

void GasTPCGenieGeom::initialize(){

  if(geoMgr_) {
	std::cout << "\n\nTop volume is: " << geoMgr_->GetTopVolume()->GetName() << std::endl;
  }

  root_geom_ = new geometry::ROOTGeomAnalyzer(geoMgr_);
  //root_geom_->SetLengthUnits(genie::units::meter/CLHEP::meter);
  //root_geom_->SetDensityUnits(genie::units::kg_m3/(CLHEP::kg/CLHEP::m3));
  root_geom_->SetLengthUnits(genie::utils::units::UnitFromString("cm"));
  root_geom_->SetDensityUnits(genie::utils::units::UnitFromString("g_cm3"));

  double zmin=0. , zmax=0.;

  if(geoMgr_){
	  std::cout << "Getting input geometry from root file \"" 
		    << geoMgr_->GetName() << "\"..." << std::endl;
  }

  else {
    string mesage = "Geometry file not found.";
    string location = "GenieNuEventAlgorithm::initGenieGeomDriver(";
    throw GasTPCException(mesage, location, GasTPCException::FATAL);
  }
  

  topVol_ = root_geom_->GetGeometry()->GetTopVolume();

  TGeoShape* bounding_box = topVol_->GetShape();
  bounding_box->GetAxisRange(3, zmin, zmax);
  zmin *= root_geom_->LengthUnits();
  zmax *= root_geom_->LengthUnits();

  TGeoVolume* det=0;
  if (targetVolume_ != "top") {
    det = root_geom_->GetGeometry()->FindVolumeFast(targetVolume_.c_str());

    if(!det) {
      string mesage = "No volume " + targetVolume_ + " in the geometry !";
      string location = "GenieNuEventAlgorithm::initGenieGeomDriver";
      throw GasTPCException(mesage, location, GasTPCException::FATAL);
    }
    std::cout << "\nTarget volume is: " << det->GetName() << std::endl;
    root_geom_->SetTopVolName(targetVolume_);
  }

  geom_anal_ = dynamic_cast<GeomAnalyzerI *> (root_geom_);


}
