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

  //set the world volume
  //TGeoVolume * world = rock->getVolume();
  double origin[3] = {0,0,0};
  TGeoMaterial *material = (TGeoMixture*)materialList->FindMaterial("Vacuum");
  TGeoMedium *medium = new TGeoMedium("Vacuum",1,material);
  TGeoBBox *worldbox = new TGeoBBox("WorldBox",350,350,350,origin);
  TGeoVolume * world = new TGeoVolume("World",worldbox,medium);

  //construct detector parts here
  RockConstruction      *rock   = NULL;
  CavityConstruction    *cavity = NULL;
  MagnetConstruction    *magnet = NULL;
  TpcVesselConstruction *vessel = NULL;
  TpcConstruction       *tpc    = NULL;
  // Six Ecals surrounding the vessel volume
  EcalConstruction     *ecal1   = NULL;
  EcalConstruction     *ecal2   = NULL;
  EcalConstruction     *ecal3   = NULL;
  EcalConstruction     *ecal4   = NULL;
  EcalConstruction     *ecal5   = NULL;
  EcalConstruction     *ecal6   = NULL;  
  //ScintillatorConstruction * scint  = new ScintillatorConstruction();
 
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
      vessel->construct(materialList,tpc, false);
    else
      vessel->construct(materialList,ftpcmaterial,ftpcdensity,true);
    vessel->getVolume()->SetLineColor(kOrange+2);
    vessel->print();
    // Add the volume
    TGeoVolume * vesselvol = vessel->getVolume();
    world->AddNodeOverlap(vesselvol,1);
  }
  
  if(fbuildecal){
    /*
    scint->setNoOfLayers(fEcalLayers);
    scint->setThickness(fEcalThickness);
    scint->setDensity(fEcalDensity);
    scint->setMatName(fEcalMaterial);
    scint->setGap(fEcalGap);
    scint->initialize();
    scint->construct(materialList,tpc,vessel);
    scint->print();
    */

    ecal1 = new EcalConstruction();
    ecal1->setThickness(fEcalThickness);
    ecal1->setLayers(fEcalLayers);
    ecal1->setDensity(fEcalDensity);
    ecal1->setMatName(fEcalMaterial);
    ecal1->setAbsDensity(fEcalAbsDensity);
    ecal1->setAbsMatName(fEcalAbsMaterial);
    if(fEcalAbsBuild){
      ecal1->setAbsThickness(fEcalAbsThickness);
    }
    else{
       ecal1->setAbsThickness(0.0);
    }
    ecal1->initialize();
    
    ecal1->construct(materialList,vessel,"+z");
    ecal1->print();
    // Add the volume
    TGeoVolume * ecalvol1 = ecal1->getVolume();
    world->AddNodeOverlap(ecalvol1,1);

    /*
    for(int i=0;i<fEcalLayers;i++){
      ecal1->construct(materialList,vessel,"+z",true,i);
      ecal1->print();
      TGeoVolume * ecalvol1 = ecal1->getVolume();
      world->AddNodeOverlap(ecalvol1,1);
    }
    */

    ecal2 = new EcalConstruction();
    ecal2->setThickness(fEcalThickness);
    ecal2->setLayers(fEcalLayers);
    ecal2->setDensity(fEcalDensity);
    ecal2->setMatName(fEcalMaterial);
    ecal2->setAbsDensity(fEcalAbsDensity);
    ecal2->setAbsMatName(fEcalAbsMaterial);
    if(fEcalAbsBuild){
      ecal2->setAbsThickness(fEcalAbsThickness);
    }
    else{
       ecal2->setAbsThickness(0.0);
    }
    ecal2->initialize();

    ecal2->construct(materialList,vessel,"-z");
    ecal2->print();
    // Add the volume
    TGeoVolume * ecalvol2 = ecal2->getVolume();
    world->AddNodeOverlap(ecalvol2,1);

    /*
    for(int i=0;i<fEcalLayers;i++){
      ecal2->construct(materialList,vessel,"-z",true,i);
      ecal2->print();
      TGeoVolume * ecalvol2 = ecal2->getVolume();
      world->AddNodeOverlap(ecalvol2,1);
    }
    */

    ecal3 = new EcalConstruction();
    ecal3->setThickness(fEcalThickness);
    ecal3->setLayers(fEcalLayers);
    ecal3->setDensity(fEcalDensity);
    ecal3->setMatName(fEcalMaterial);
    ecal3->setAbsDensity(fEcalAbsDensity);
    ecal3->setAbsMatName(fEcalAbsMaterial);
    if(fEcalAbsBuild){
      ecal3->setAbsThickness(fEcalAbsThickness);
    }
    else{
       ecal3->setAbsThickness(0.0);
    }
    ecal3->initialize();

    ecal3->construct(materialList,vessel,"+y");
    ecal3->print();
    // Add the volume
    TGeoVolume * ecalvol3 = ecal3->getVolume();
    world->AddNodeOverlap(ecalvol3,1);

    /*
    for(int i=0;i<fEcalLayers;i++){
      ecal3->construct(materialList,vessel,"+y",true,i);
      ecal3->print();
      TGeoVolume * ecalvol3 = ecal3->getVolume();
      world->AddNodeOverlap(ecalvol3,1);
    }
    */

    ecal4 = new EcalConstruction();
    ecal4->setThickness(fEcalThickness);
    ecal4->setLayers(fEcalLayers);
    ecal4->setDensity(fEcalDensity);
    ecal4->setMatName(fEcalMaterial);
    ecal4->setAbsDensity(fEcalAbsDensity);
    ecal4->setAbsMatName(fEcalAbsMaterial);
    if(fEcalAbsBuild){
      ecal4->setAbsThickness(fEcalAbsThickness);
    }
    else{
       ecal4->setAbsThickness(0.0);
    }
    ecal4->initialize();

    ecal4->construct(materialList,vessel,"-y");
    ecal4->print();
    // Add the volume
    TGeoVolume * ecalvol4 = ecal4->getVolume();
    world->AddNodeOverlap(ecalvol4,1);

    /*
    for(int i=0;i<fEcalLayers;i++){
      ecal4->construct(materialList,vessel,"-y",true,i);
      ecal4->print();
      TGeoVolume * ecalvol4 = ecal4->getVolume();
      world->AddNodeOverlap(ecalvol4,1);
    }
    */

    ecal5 = new EcalConstruction();
    ecal5->setThickness(fEcalThickness);
    ecal5->setLayers(fEcalLayers);
    ecal5->setDensity(fEcalDensity);
    ecal5->setMatName(fEcalMaterial);
    ecal5->setAbsDensity(fEcalAbsDensity);
    ecal5->setAbsMatName(fEcalAbsMaterial);
    if(fEcalAbsBuild){
      ecal5->setAbsThickness(fEcalAbsThickness);
    }
    else{
       ecal5->setAbsThickness(0.0);
    }
    ecal5->initialize();

    ecal5->construct(materialList,vessel,"+x");
    ecal5->print();
    // Add the volume
    TGeoVolume * ecalvol5 = ecal5->getVolume();
    world->AddNodeOverlap(ecalvol5,1);

    /*
    for(int i=0;i<fEcalLayers;i++){
      ecal5->construct(materialList,vessel,"+x",true,i);
      ecal5->print();
      TGeoVolume * ecalvol5 = ecal5->getVolume();
      world->AddNodeOverlap(ecalvol5,1);
    }
    */

    ecal6 = new EcalConstruction();
    ecal6->setThickness(fEcalThickness);
    ecal6->setLayers(fEcalLayers);
    ecal6->setDensity(fEcalDensity);
    ecal6->setMatName(fEcalMaterial);
    ecal6->setAbsDensity(fEcalAbsDensity);
    ecal6->setAbsMatName(fEcalAbsMaterial);
    if(fEcalAbsBuild){
      ecal6->setAbsThickness(fEcalAbsThickness);
    }
    else{
       ecal6->setAbsThickness(0.0);
    }
    ecal6->initialize();

    ecal6->construct(materialList,vessel,"-x");
    ecal6->print();
    // Add the volume
    TGeoVolume * ecalvol6 = ecal6->getVolume();
    world->AddNodeOverlap(ecalvol6,1);

    /*
    for(int i=0;i<fEcalLayers;i++){
      ecal6->construct(materialList,vessel,"-x",true,i);
      ecal6->print();
      TGeoVolume * ecalvol6 = ecal6->getVolume();
      world->AddNodeOverlap(ecalvol6,1);
    }

    */
  }
  
  if(fbuildmagnet && fbuildvessel){
    magnet = new MagnetConstruction();
    magnet->setThickness(fMagnetThickness);
    magnet->setDensity(fMagnetDensity);
    magnet->setMatName(fMagnetMaterial);
    magnet->setGap(4*fEcalLayers*fEcalThickness);
    magnet->initialize();
    magnet->construct(materialList,vessel);
    magnet->getVolume()->SetLineColor(12);
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
  }

  if(fbuildcavity){
    cavity = new CavityConstruction();
    cavity->setGap(fCavityGap);
    cavity->setDensity(fCavityDensity);
    cavity->initialize();
    cavity->construct(materialList,vessel,magnet,rock,fdetectorYAxisOffset);
    cavity->getVolume()->SetLineColor(52);
    cavity->print();
  }

  //set the top volume
  geom->SetTopVolume(world);
  
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
  fEcalThickness        = util::XML2Dbl(xml,root_node,"GeometryBuilder/EcalConstruction/EcalThickness");
  fEcalGap              = util::XML2Dbl(xml,root_node,"GeometryBuilder/EcalConstruction/EcalGap");
  fEcalMaterial         = util::XML2String(xml,root_node,"GeometryBuilder/EcalConstruction/EcalMaterial");
  fEcalDensity          = util::XML2Dbl(xml,root_node,"GeometryBuilder/EcalConstruction/EcalDensity");
  fEcalAbsThickness     = util::XML2Dbl(xml,root_node,"GeometryBuilder/EcalConstruction/EcalAbsThickness");
  fEcalAbsMaterial      = util::XML2String(xml,root_node,"GeometryBuilder/EcalConstruction/EcalAbsMaterial");
  fEcalAbsDensity       = util::XML2Dbl(xml,root_node,"GeometryBuilder/EcalConstruction/EcalAbsDensity");

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
