//
// Title: Root and gdml Geometry for the Near Detector
//
// Description: to be written
//
// Author: Tom Stainer, Georgios Christodoulou
//

#include "GasTPCGeombuild.hh"
#include "TXMLEngine.h"
#include "Messenger.hh"

using namespace util;

//___________________________________________________________________
int main(int argc, char ** argv)
{

  // Load the xml file
  std::string datacard = "";
  for(int iArg = 1; iArg < argc; iArg++) {
    if((!strcasecmp(argv[iArg],"-i"))||(!strcasecmp(argv[iArg],"-infile"))) {
      datacard = argv[++iArg] ;
    }
  }

  const char* gastpc_dir = getenv("GasTPC");
  if(datacard == "")
    datacard = "GeometryBuild.xml";
  std::string fdatacard = string(gastpc_dir) + "/src/config/" + datacard;

  ReadDatacard(fdatacard);

  //load the geometry library
  //TSystem * system = new TSystem("system");
  //system->Load("libGeom"); 
  
  TGeoManager * geom = new TGeoManager("geom","HP Gas TPC Near Detector");

  //material list constructor will create all necessary materials
  Materials * materialList = new Materials();

  // Define world volume
  TGeoMaterial *wmaterial = (TGeoMixture*)materialList->FindMaterial("Air");
  TGeoMedium *wmedium = new TGeoMedium("Air",1,wmaterial);
  TGeoBBox *worldbox = new TGeoBBox("DetEnclosure",899.16,768.096,1501.61625);
  TGeoVolume *wvolume = new TGeoVolume("volDetEnclosure",worldbox,wmedium);

  // Build the detector
  NDConstruction *det = new NDConstruction();
  det->initializeTPC(fVesselLength, fVesselRadius, ftpcdensity, ftpcmaterial);
  det->initializeVessel(fVesselThickness, fVesselDensity, fVesselMaterial);
  det->initializeEcal(fEcalDensity, fEcalThickness, fDsEcalLayers, fEcalAbsDensity, fEcalAbsThickness, fEcalMaterial, fEcalAbsMaterial, fEcalBuildLayers);
  det->initializeMagnet(fMagnetDensity,fMagnetThickness,fMagnetMaterial);
  det->construction(materialList, fbuildecal, fbuildmagnet);
  //                              ecal,       magnet 
  //std::cout << "Z shift = " << det->getStartVol().Z() << std::endl;
  TGeoVolume * detvol = det->getVolume();
  wvolume->AddNode(detvol,1);

  //set the top volume
  geom->SetTopVolume(wvolume);
  //geom->SetTopVisible(false);
  
  //close geometry
  geom->CloseGeometry();

  geom->Export(frootFilename.c_str());
  geom->Export(fgdmlFilename.c_str());

  delete geom;

  std::cout << "\nRoot file created for geometry.\nOutput file is: "<< frootFilename.c_str()
	    << "\nGDML file created for geometry.\nOutput file is: "<< fgdmlFilename.c_str() << "\n\n";

  return 0;
}

void ReadDatacard(std::string datacard){

  LOG("GeometryConstruction",pNOTICE) << "Reading configuration from XML file [" << datacard << "]";

  TXMLEngine* xml = new TXMLEngine(); 
  XMLDocPointer_t xmldoc = xml->ParseFile(datacard.c_str());
  if (xmldoc==0) {
    std::cerr << "Exp >> " << "Can not parse input: " << datacard << std::endl;
    delete xml;
    return;  
  }
  XMLNodePointer_t root_node = xml->DocGetRootElement(xmldoc);

  frootFilename         = util::XML2String(xml,root_node,"GeometryBuilder/OutputROOTFile");
  fgdmlFilename         = util::XML2String(xml,root_node,"GeometryBuilder/OutputGDMLFile");
  fdetectorYAxisOffset  = util::XML2Dbl(xml,root_node,"GeometryBuilder/YAxisOffset");

  fbuildtpc             = util::XML2Bool(xml,root_node,"GeometryBuilder/TPCConstruction/TPCBuild");
  fbuildvessel          = util::XML2Bool(xml,root_node,"GeometryBuilder/VesselConstruction/VesselBuild");
  fbuildecal            = util::XML2Bool(xml,root_node,"GeometryBuilder/EcalConstruction/EcalBuild");
  fEcalAbsBuild         = util::XML2Bool(xml,root_node,"GeometryBuilder/EcalConstruction/EcalAbsBuild");
  fEcalHadBuild         = util::XML2Bool(xml,root_node,"GeometryBuilder/EcalConstruction/EcalHadBuild"); 
  fbuildrock            = util::XML2Bool(xml,root_node,"GeometryBuilder/RockConstruction/RockBuild");
  fbuildmagnet          = util::XML2Bool(xml,root_node,"GeometryBuilder/MagnetConstruction/MagnetBuild");
  fbuildcavity          = util::XML2Bool(xml,root_node,"GeometryBuilder/CavityConstruction/CavityBuild");

  ftpcx                 = util::XML2Dbl(xml,root_node,"GeometryBuilder/TPCConstruction/TPCSideX");
  ftpcy                 = util::XML2Dbl(xml,root_node,"GeometryBuilder/TPCConstruction/TPCSideY");
  ftpcz                 = util::XML2Dbl(xml,root_node,"GeometryBuilder/TPCConstruction/TPCSideZ");
  ftpcdensity           = util::XML2Dbl(xml,root_node,"GeometryBuilder/TPCConstruction/TPCDensity");
  ftpcPlacementZ        = util::XML2Dbl(xml,root_node,"GeometryBuilder/TPCConstruction/TPCPlacementZ");
  ftpcmaterial          = util::XML2String(xml,root_node,"GeometryBuilder/TPCConstruction/TPCMaterial");

  fVesselRadiusRatio    = util::XML2Dbl(xml,root_node,"GeometryBuilder/VesselConstruction/VesselRadiusRatio");
  fVesselThickness      = util::XML2Dbl(xml,root_node,"GeometryBuilder/VesselConstruction/VesselThickness");
  fVesselRadius         = util::XML2Dbl(xml,root_node,"GeometryBuilder/VesselConstruction/VesselRadius");
  fVesselLength         = util::XML2Dbl(xml,root_node,"GeometryBuilder/VesselConstruction/VesselLength");
  fVesselPlacementZ     = util::XML2Dbl(xml,root_node,"GeometryBuilder/VesselConstruction/VesselPlacementZ");
  fVesselDensity        = util::XML2Dbl(xml,root_node,"GeometryBuilder/VesselConstruction/VesselDensity");
  fVesselMaterial       = util::XML2String(xml,root_node,"GeometryBuilder/VesselConstruction/VesselMaterial");

  fEcalLayers           = util::XML2Dbl(xml,root_node,"GeometryBuilder/EcalConstruction/EcalLayers");
  fEcalBuildLayers      = util::XML2Dbl(xml,root_node,"GeometryBuilder/EcalConstruction/EcalBuildLayers");
  fDsEcalLayers           = util::XML2Dbl(xml,root_node,"GeometryBuilder/EcalConstruction/DsEcalLayers");
  fBrEcalLayers           = util::XML2Dbl(xml,root_node,"GeometryBuilder/EcalConstruction/BrEcalLayers");
  fUsEcalLayers           = util::XML2Dbl(xml,root_node,"GeometryBuilder/EcalConstruction/UsEcalLayers");
  fEcalThickness        = util::XML2Dbl(xml,root_node,"GeometryBuilder/EcalConstruction/EcalThickness");
  fEcalGap              = util::XML2Dbl(xml,root_node,"GeometryBuilder/EcalConstruction/EcalGap");
  fEcalMaterial         = util::XML2String(xml,root_node,"GeometryBuilder/EcalConstruction/EcalMaterial");
  fEcalDensity          = util::XML2Dbl(xml,root_node,"GeometryBuilder/EcalConstruction/EcalDensity");
  fEcalAbsThickness     = util::XML2Dbl(xml,root_node,"GeometryBuilder/EcalConstruction/EcalAbsThickness");
  fEcalAbsMaterial      = util::XML2String(xml,root_node,"GeometryBuilder/EcalConstruction/EcalAbsMaterial");
  fEcalAbsDensity       = util::XML2Dbl(xml,root_node,"GeometryBuilder/EcalConstruction/EcalAbsDensity");
  fEcalHadThickness     = util::XML2Dbl(xml,root_node,"GeometryBuilder/EcalConstruction/EcalHadThickness");
  fEcalHadMaterial      = util::XML2String(xml,root_node,"GeometryBuilder/EcalConstruction/EcalHadMaterial");
  fEcalHadDensity       = util::XML2Dbl(xml,root_node,"GeometryBuilder/EcalConstruction/EcalHadDensity");
  fEcalHadLayers        = util::XML2Dbl(xml,root_node,"GeometryBuilder/EcalConstruction/EcalHadLayers");

  fMagnetThickness      = util::XML2Dbl(xml,root_node,"GeometryBuilder/MagnetConstruction/MagnetThickness");
  fMagnetDensity        = util::XML2Dbl(xml,root_node,"GeometryBuilder/MagnetConstruction/MagnetDensity");
  fMagnetMaterial       = util::XML2String(xml,root_node,"GeometryBuilder/MagnetConstruction/MagnetMaterial");

  fRockRadius           = util::XML2Dbl(xml,root_node,"GeometryBuilder/RockConstruction/RockRadius");
  fRockLength           = util::XML2Dbl(xml,root_node,"GeometryBuilder/RockConstruction/RockLength");
  fRockDensity          = util::XML2Dbl(xml,root_node,"GeometryBuilder/RockConstruction/RockDensity");
  fRockX                = util::XML2Dbl(xml,root_node,"GeometryBuilder/RockConstruction/RockX");
  fRockY                = util::XML2Dbl(xml,root_node,"GeometryBuilder/RockConstruction/RockY");
  fRockZ                = util::XML2Dbl(xml,root_node,"GeometryBuilder/RockConstruction/RockZ");
  fRockMaterial         = util::XML2String(xml,root_node,"GeometryBuilder/RockConstruction/RockMaterial");

  fCavityGap            = util::XML2Dbl(xml,root_node,"GeometryBuilder/CavityConstruction/CavityGap");
  fCavityDensity        = util::XML2Dbl(xml,root_node,"GeometryBuilder/CavityConstruction/CavityDensity");

  // Release memory
  xml->FreeDoc(xmldoc);
  delete xml;

}
