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
  TGeoBBox *worldbox = new TGeoBBox("WorldBox",3500,3500,3500);
  TGeoVolume *wvolume = new TGeoVolume("TWorld",worldbox,wmedium);
  
  // Build the detector
  NDConstruction *det = new NDConstruction();
  det->initializeTPC(fVesselLength, fVesselRadius, ftpcdensity, ftpcmaterial);
  det->initializeVessel(fVesselThickness, fVesselDensity, fVesselMaterial);
  det->initializeEcal(fEcalDensity, fEcalThickness, fDsEcalLayers, fEcalAbsDensity, fEcalAbsThickness, fEcalMaterial, fEcalAbsMaterial, fEcalBuildLayers);
  det->initializeMagnet(fMagnetDensity,fMagnetThickness,fMagnetMaterial);
  det->construction(materialList, fbuildecal, fbuildmagnet);
  //                              ecal,       magnet 
  TGeoVolume * detvol = det->getVolume();
  wvolume->AddNodeOverlap(detvol,1);

  //set the top volume
  geom->SetTopVolume(wvolume);

  /*
  // Build TPC
  if(fbuildtpc){
    tpc = new TpcConstruction();
    tpc->setXYZ(ftpcx, ftpcy, ftpcz);
    tpc->setDensity(ftpcdensity);
    tpc->setMatName(ftpcmaterial);
    tpc->initialize();
    tpc->construct(materialList);
    tpc->getVolume()->SetLineColor(11);
    tpc->print();
  }

  if(fbuildvessel){
    vessel = new TpcVesselConstruction();
    vessel->setLength(fVesselLength);
    vessel->setRadius(fVesselRadius);
    vessel->setRadiusRatio(fVesselRadiusRatio);
    vessel->setDensity(fVesselDensity);
    vessel->setThickness(fVesselThickness);
    vessel->setMatName(fVesselMaterial);
    vessel->initialize();
    if(fbuildtpc)
      vessel->construct(materialList,tpc, true);
    else
      vessel->construct(materialList,ftpcmaterial,ftpcdensity,true);
    //vessel->getVolume()->SetLineColor(kOrange+2);
    vessel->print();
    // Add the volume
    TGeoVolume * vesselvol = vessel->getVolume();
    world->AddNodeOverlap(vesselvol,1);
    
    EcalConstruction     *ecal = new EcalConstruction();
    ecal->setThickness(fEcalThickness);
    ecal->setLayers(fDsEcalLayers);
    ecal->setDensity(fEcalDensity);
    ecal->setMatName(fEcalMaterial);
    ecal->setAbsDensity(fEcalAbsDensity);
    ecal->setAbsMatName(fEcalAbsMaterial);
    ecal->setGap(fEcalGap);
    ecal->setAbsThickness(0.1);
    ecal->initialize();
    ecal->construction(materialList,vessel);
    ecal->print();
    TGeoVolume * ecalvol1 = ecal->getVolume();
    world->AddNodeOverlap(ecalvol1,2);
    
  }
  
  if(fbuildecal && fbuildvessel){
 
    ecalZ1 = new EcalConstruction();
    ecalZ1->setThickness(fEcalThickness);
    ecalZ1->setLayers(fDsEcalLayers);
    ecalZ1->setDensity(fEcalDensity);
    ecalZ1->setMatName(fEcalMaterial);
    ecalZ1->setAbsDensity(fEcalAbsDensity);
    ecalZ1->setAbsMatName(fEcalAbsMaterial);
    ecalZ1->setGap(fEcalGap);
    if(fEcalAbsBuild){
      ecalZ1->setAbsThickness(fEcalAbsThickness);
    }
    else{
       ecalZ1->setAbsThickness(0.0);
    }
    ecalZ1->initialize();
 
    for(int i=0;i<fDsEcalLayers;i++){
      ecalZ1->construct(materialList,vessel,"+z",true,i);
      ecalZ1->print();
      TGeoVolume * ecalvol1 = ecalZ1->getVolume();
      world->AddNodeOverlap(ecalvol1,1);
    }
    
  
 
  if(fbuildmagnet && fbuildvessel){
    magnet = new MagnetConstruction();
    magnet->setThickness(fMagnetThickness);
    magnet->setDensity(fMagnetDensity);
    magnet->setMatName(fMagnetMaterial);
    if(fEcalHadLayers)
      magnet->setGap(fEcalLayers*fEcalThickness+fEcalHadLayers*fEcalHadThickness+10);
    else
      magnet->setGap(fEcalLayers*fEcalThickness+10);
    magnet->initialize();
    magnet->construct(materialList,vessel,false);
    magnet->print();
    // Add the volume
    TGeoVolume * magnetvol = magnet->getVolume();
    world->AddNodeOverlap(magnetvol,1);
  }

  if(fbuildrock){
    rock = new RockConstruction();
    rock->setXYZ(fRockX,fRockY,fRockZ);
    rock->setDensity(fRockDensity);
    rock->setMatName(fRockMaterial);
    rock->initialize();
    rock->construct(materialList);
    rock->getVolume()->SetLineColor(42);
    rock->print();
    // Add the volume
    TGeoVolume * rockvol = rock->getVolume();
    world->AddNodeOverlap(rockvol,1);
  }

  if(fbuildcavity){
    cavity = new CavityConstruction();
    cavity->setGap(fCavityGap);
    cavity->setDensity(fCavityDensity);
    cavity->initialize();
    cavity->construct(materialList,vessel,magnet,rock,fdetectorYAxisOffset);
    cavity->getVolume()->SetLineColor(52);
    cavity->print();
    // Add the volume
    TGeoVolume * cavvol = cavity->getVolume();
    world->AddNodeOverlap(cavvol,1);
  }
  */
  //set the top volume
  //geom->SetTopVolume(world);
  
  /*
  //---- Placements ----//
  
  TGeoTranslation* cavityPlacementTrans = new TGeoTranslation(0.0,rock->getY()/(2.0*CLHEP::cm) - cavity->getLength()/(2.0*CLHEP::cm),0.0);

  TGeoRotation *cavityPlacementRot = new TGeoRotation();
  cavityPlacementRot->SetAngles(0, 90, 0);
  TGeoCombiTrans *cavityPlacementCombi = new TGeoCombiTrans(*cavityPlacementTrans, *cavityPlacementRot);
  
  //now the axis are rotated so need to allow for this in  daughter positions
  // y axis is now the z axis
  TGeoTranslation* magnetPlacementTrans = new TGeoTranslation(0.0,0.0,cavity->getLength()/(2.0*CLHEP::cm) - magnet->getRadius()/(CLHEP::cm));
  TGeoRotation * magnetPlacementRot = new TGeoRotation();
  magnetPlacementRot->SetAngles(0, -90, 0);
  TGeoCombiTrans *magnetPlacementCombi = new TGeoCombiTrans(*magnetPlacementTrans, *magnetPlacementRot);

  TGeoTranslation* origin = new TGeoTranslation(0,0,0);

  world->AddNode(cavity->getVolume(),1,cavityPlacementCombi);
  //world->AddNode(vessel->getVolume(),1,origin);
  //cavity->getVolume()->AddNodeOverlap(magnet->getVolume(),2,magnetPlacementCombi);
  //cavity->getVolume()->AddNodeOverlap(vessel->getVolume(),3,magnetPlacementCombi);
  //magnet->getVolume()->AddNodeOverlap(vessel->getVolume(),3,origin);
  //vessel->getInnerVolume()->AddNodeOverlap(scint->getVolume(),4,new TGeoTranslation(0,0,tpcPlacementZ_/CLHEP::cm));
  //scint->getVolume()->AddNode(tpc->getVolume(),9,origin);
  */

  
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
