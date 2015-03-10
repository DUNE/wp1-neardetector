//
// Title: Materials
//
// Author: Tom STainer
//
// Description: All materials in the geometry are specified in this class

#include "Materials.hh"

Materials::Materials() {
  elementList.resize(0);
  materialList.resize(0);
  this->initialize();
}

Materials::Materials(TGeoManager* geom){
  elementList.resize(0);
  materialList.resize(0);
  gManager_ = geom;
}

Materials::~Materials(){
  for(std::list<TGeoElement*>::iterator it=elementList.begin(); it!=elementList.end(); ++it){
	delete (*it);
  }
  for(std::list<TGeoMixture*>::iterator it=materialList.begin(); it!=materialList.end(); ++it){
	delete (*it);
  }
  elementList.resize(0);
  materialList.resize(0);
}

void Materials::initialize(){

  DefineElements();
  DefineMaterials();
  
}

TGeoElement * Materials::FindElement(std::string name){

  element_ = NULL;

  //loop through list to find element
  for(std::list<TGeoElement*>::iterator it=elementList.begin(); it!=elementList.end(); ++it){
	//std::cout << "\n--Element: " << (*it)->GetName();
	std::string tmp_name = (*it)->GetName();
	std::size_t found = tmp_name.find(name);
	if( found!=std::string::npos ) {
		element_ = (*it);
		break;
	}	
  }

  if(!element_) std::cout << "\n***Element not found!" <<std::endl;

  return element_;
}


TGeoMixture * Materials::FindMaterial(std::string name,bool deepCopy){

  material_ = NULL;
  //loop through list to find element
  for(std::list<TGeoMixture*>::iterator it=materialList.begin(); it!=materialList.end(); ++it){
	std::string tmp_name = (*it)->GetName();
	std::size_t found = tmp_name.find(name);
	if( found!=std::string::npos ) {
		material_ = (*it);
		break;
	}
  }

  if(!material_){
	std::cout << "\n***Material not found!" <<std::endl;
	return NULL;
  }
 
  //std::cout << "\nMaterial Name: " << name <<std::endl;
  //return new pointer to material
  if(deepCopy)return copy(material_);
  return material_;
}

//method to allow for deep copy of material - TGeoMixture::TGeoMixture(const TGeoMixture&) is protected
TGeoMixture * Materials::copy(const TGeoMixture* mat){

  TString name = mat->GetName();
  name = name.Append("_copy");

  std::cout << "\nMaterial Name: " << name <<std::endl;

  TGeoMixture * tmp_mat = new TGeoMixture(name,mat->GetNelements(),mat->GetDensity()/(CLHEP::g/CLHEP::cm3));
  for(int j=0;j<mat->GetNelements();j++){
  	tmp_mat->AddElement(mat->GetElement(j),mat->GetWmixt()[j]);
  }
  return tmp_mat;
}

void Materials::addMaterial(std::string name, double A, double Z, double density){

  //TGeoMaterial * newMat = new TGeoMaterial(name.c_str(), A,Z,density);

  //materialList.push_back(*newMat);
  //size = materialList.size();
}

void Materials::DefineElements() {

  //common element definitions
  TGeoElement *elHydrogen 	= 	new TGeoElement("Hydrogen","Hydrogen",1,1.01);
  TGeoElement *elBoron 		= 	new TGeoElement("Boron","Boron",5,10.811);
  TGeoElement *elCarbon 	= 	new TGeoElement("Carbon","Carbon",6,12.1);
  TGeoElement *elNitrogen 	= 	new TGeoElement("Nitrogen","Nitrogen",7,14.0);
  TGeoElement *elOxygen 	= 	new TGeoElement("Oxygen","Oxygen",8,16.0);
  TGeoElement *elFluorine 	= 	new TGeoElement("Fluorine","Fluorine",9,19.0);
  TGeoElement *elSodium 	= 	new TGeoElement("Sodium","Sodium",11,23.0);
  TGeoElement *elAluminium 	= 	new TGeoElement("Aluminium","Aluminium",13,26.98);
  TGeoElement *elSilicon 	= 	new TGeoElement("Silicon","Silicon",14,28.09);
  TGeoElement *elArgon 		= 	new TGeoElement("Argon","Argon",18,39.95);
  TGeoElement *elIron 		= 	new TGeoElement("Iron","Iron",26,55.85);
  TGeoElement *elCobalt		= 	new TGeoElement("Cobalt","Cobalt",27,58.93);
  TGeoElement *elCopper		= 	new TGeoElement("Copper","Copper",29,63.01);

  //add to the material list
  elementList.push_back(elHydrogen);
  elementList.push_back(elBoron);
  elementList.push_back(elCarbon);
  elementList.push_back(elNitrogen);
  elementList.push_back(elOxygen);
  elementList.push_back(elFluorine);
  elementList.push_back(elSodium);
  elementList.push_back(elAluminium);
  elementList.push_back(elSilicon);
  elementList.push_back(elArgon);
  elementList.push_back(elIron);
  elementList.push_back(elCobalt);
  elementList.push_back(elCopper);

}

void Materials::DefineMaterials() {

    Double_t a,density;
    std::string name, symbol;
    Double_t temperature, pressure;
    Int_t z,nel, ncomponents, natoms;
    Double_t fractionmass;

    //Vacuum (set as default material)
    density     = 1e-6*(CLHEP::g/CLHEP::cm3);
    pressure    = 3.e-18*CLHEP::pascal;
    temperature = 2.73*CLHEP::kelvin;
    TGeoMixture *vacuum = new TGeoMixture("Vacuum",nel=1,density/(CLHEP::g/CLHEP::cm3));
    if(FindElement("Hydrogen"))vacuum->AddElement(FindElement("Hydrogen"), fractionmass = 1.0);
    else vacuum = NULL;

    if(vacuum){
    	materialList.push_back(vacuum);
    }

    //Air
    density = 1.29*(CLHEP::mg/CLHEP::cm3);
    pressure    = 1*CLHEP::atmosphere;
    temperature = 293.15*CLHEP::kelvin;
    TGeoMixture * air =  new TGeoMixture("Air",nel=2,density/(CLHEP::g/CLHEP::cm3));
    if(FindElement("Nitrogen"))air->AddElement(FindElement("Nitrogen"), fractionmass = 0.76);
    else air = NULL;
    if(FindElement("Oxygen"))air->AddElement(FindElement("Oxygen"), fractionmass = 0.24);
    else air = NULL;

    if(air){
	materialList.push_back(air);
    }

    //Argon Gas for TPC Fiducial
    //TpcFiducial is Gas argon mixture of Ar,CF4,Isobutane
    density = 0.035*(CLHEP::g/CLHEP::cm3);
    pressure    = 1*CLHEP::atmosphere;
    temperature = 293.15*CLHEP::kelvin;
    TGeoMixture * argonGas =  new TGeoMixture("ArgonGas",nel=4,density/(CLHEP::g/CLHEP::cm3));
    if(FindElement("Argon"))argonGas->AddElement(FindElement("Argon"), fractionmass = 0.950);
    else argonGas = NULL;
    //CF4 and Isobutane(C4H10) mass ratios taken
    // for 3% CF4 and 2% Isobutane
    if(FindElement("Carbon"))argonGas->AddElement(FindElement("Carbon"), fractionmass = 0.021);
    else argonGas = NULL;
    if(FindElement("Fluorine"))argonGas->AddElement(FindElement("Fluorine"), fractionmass = 0.026);
    else argonGas = NULL;
    if(FindElement("Hydrogen"))argonGas->AddElement(FindElement("Hydrogen"), fractionmass = 0.003);
    else argonGas = NULL;

    if(argonGas){
	materialList.push_back(argonGas);
    }

    //Scintillator bars
    TGeoMixture * scint =  new TGeoMixture("Scintillator",nel=2);
    if(FindElement("Carbon"))scint->AddElement(FindElement("Carbon"), fractionmass = 0.85);
    else scint = NULL;
    if(FindElement("Hydrogen"))scint->AddElement(FindElement("Hydrogen"), fractionmass = 0.15);
    else scint = NULL;

    if(scint){
	materialList.push_back(scint);
    }

    //Aluminium
    density = 2.70*(CLHEP::g/CLHEP::cm3);
    TGeoMixture * aluminium =  new TGeoMixture("Aluminium",nel=1,density/(CLHEP::g/CLHEP::cm3));
    if(FindElement("Aluminium"))aluminium->AddElement(FindElement("Aluminium"), fractionmass = 1.0);
    else aluminium = NULL;

    if(aluminium){
	materialList.push_back(aluminium);
    }

    //Iron.
    density = 7.87*(CLHEP::g/CLHEP::cm3);
    TGeoMixture * iron = new TGeoMixture("Iron",nel=1,density/(CLHEP::g/CLHEP::cm3));
    if(FindElement("Iron"))iron->AddElement(FindElement("Iron"), fractionmass = 1.0);
    else iron = NULL;

    if(iron){
	materialList.push_back(iron);
    }


    //Rock is Sandstone Si02 
    density = 2.323*(CLHEP::g/CLHEP::cm3);
    TGeoMixture * sandstone =  new TGeoMixture("Sandstone",nel=2,density/(CLHEP::g/CLHEP::cm3));
    if(FindElement("Oxygen"))sandstone->AddElement(FindElement("Oxygen"), fractionmass = 0.5333);
    else sandstone = NULL;
    if(FindElement("Silicon"))sandstone->AddElement(FindElement("Silicon"), fractionmass = 0.4667);
    else sandstone = NULL;

    if(sandstone){
	materialList.push_back(sandstone);
    }

    //Stainless Steel.  The density is taken from average 304 grade SS.
    density = 8.0*(CLHEP::g/CLHEP::cm3);
    TGeoMixture * steel = new TGeoMixture("Steel", nel=3,density/(CLHEP::g/CLHEP::cm3));
    if(FindElement("Carbon"))steel->AddElement(FindElement("Carbon"),fractionmass =  0.04);
    else steel = NULL;
    if(FindElement("Iron"))steel->AddElement(FindElement("Iron"),fractionmass =  0.88);
    else steel = NULL;
    if(FindElement("Cobalt"))steel->AddElement(FindElement("Cobalt"),fractionmass =  0.08);
    else steel = NULL;

    if(steel){
	materialList.push_back(steel);
    }

    // G10 - by volume 57% glass, 43% epoxy (CH2) - name from t2k ND280MC material list
    density = 1.70*(CLHEP::g/CLHEP::cm3);
    TGeoMixture * g10 = new TGeoMixture("G10",nel=6,density/(CLHEP::g/CLHEP::cm3));
    if(FindElement("Carbon"))g10->AddElement(FindElement("Carbon"),fractionmass = 0.368);
    else g10 = NULL;
    if(FindElement("Oxygen"))g10->AddElement(FindElement("Oxygen"),fractionmass = 0.307);
    else g10 = NULL;
    if(FindElement("Silicon"))g10->AddElement(FindElement("Silicon"),fractionmass = 0.219);
    else g10 = NULL;
    if(FindElement("Hydrogen"))g10->AddElement(FindElement("Hydrogen"),fractionmass = 0.062);
    else g10 = NULL;
    if(FindElement("Boron"))g10->AddElement(FindElement("Boron"),fractionmass = 0.022);
    else g10 = NULL;
    if(FindElement("Sodium"))g10->AddElement(FindElement("Sodium"),fractionmass = 0.022);
    else g10 = NULL;

    if(g10){
	materialList.push_back(g10);
    }

    // Rohacell (polymethacrylimide, chemical formula to be confirmed from 
    // "Polymethyl methacrylate" C5O2H8)
    density = 0.052*(CLHEP::g/CLHEP::cm3);
    TGeoMixture* rohacell = new TGeoMixture("Rohacell",nel=3,density/(CLHEP::g/CLHEP::cm3));
    // Rohacell C:H:O=5:8:2
    if(FindElement("Carbon"))rohacell->AddElement(FindElement("Carbon"),5);
    else rohacell = NULL;
    if(FindElement("Hydrogen"))rohacell->AddElement(FindElement("Hydrogen"),8);
    else rohacell = NULL;
    if(FindElement("Oxygen"))rohacell->AddElement(FindElement("Oxygen"),2);
    else rohacell = NULL;

    if(rohacell){
	materialList.push_back(rohacell);
    }
    
    // G10/Roha/Cu TPC cage wall material
    //
    // Inner box 13.2mm G10 / Roha / Cu Panels (2 x 0.76mm G10, 11.68mm
    // Rohacell, and 2* 0.035mm Cu)
    //
    // Density:
    // rho = ( m_G10 + m_Roha + m_Cu ) / ( V_G10 + V_Roha +V_Cu )
    //    = ( rho_G10*A*1.52mm + rho_Roha*A*11.68mm + rho_Cu*A*0.07mm ) 
    //           / ( A*1.52mm +A*11.68mm+A*0.07mm )
    //    = ( 1.7g/cm^3*1.52mm + 0.052g/cm^3*11.68mm + 8.94g/cm^3*0.07mm) 
    //                    / 13.27mm
    //    = 0.2877 g/cm^3
    density = 0.2877*(CLHEP::g/CLHEP::cm3);
    TGeoMixture* g10roha = new TGeoMixture("G10Roha",ncomponents=3,density/(CLHEP::g/CLHEP::cm3));

    //Fraction by weight:
    //  w_G10 = m_G10 / m_total
    //       = (rho_G10*A*1.52mm) / (rho*A*13.27mm)
    //       = (1.7g/cm^3*1.52mm) / (0.2877g/cm^3*13.27mm)
    //       = 0.6769
    //  w_Ro = m_Roha / m_total
    //       = (rho_Roha*A*11.68mm) / (rho*A*13.27mm)
    //       = (0.052g/cm^3*11.68mm) / (0.2877g/cm^3*13.27mm)
    //       = 0.1591
    //  w_Cu = m_Cu / m_total
    //       = (rho_Cu*A*0.07mm) / (rho*A*13.27mm)
    //       = (8.94g/cm^3*0.07mm) / (0.2877g/cm^3*13.27mm)
    //       = 0.1640
    if(FindMaterial("G10",false))g10roha->AddElement(FindMaterial("G10",false), fractionmass = 0.6769);
    else g10roha = NULL;
    if(FindMaterial("Rohacell",false))g10roha->AddElement(FindMaterial("Rohacell",false), fractionmass = 0.1591);
    else g10roha = NULL;
    if(FindElement("Copper"))g10roha->AddElement(FindElement("Copper"), fractionmass = 0.1640);
    else g10roha = NULL;

    if(g10roha){
	materialList.push_back(g10roha);
    }


    //Earth
    density = 2.15*(CLHEP::g/CLHEP::cm3);
    TGeoMixture* earth = new TGeoMixture("Earth",nel=2,density/(CLHEP::g/CLHEP::cm3));
    // Rohacell C:H:O=5:8:2
    if(FindElement("Silicon"))earth->AddElement(FindElement("Silicon"),1);
    else earth = NULL;
    if(FindElement("Oxygen"))earth->AddElement(FindElement("Oxygen"),2);
    else earth = NULL;

    if(earth){
	materialList.push_back(earth);
    }
/* 
    //Cement
    density = 2.5*g/cm3;
    G4Material* cement = new G4Material(name="Cement", density, nel=2);
    cement->AddElement(elSi, natoms=1);
    cement->AddElement(elO, natoms=2);
    gMan->SetDrawAtt(cement,14,0.2);

    //Water
    density = 1.0*g/cm3;
    G4Material* water = new G4Material(name="Water", density, nel=2);
    water->AddElement(elH, natoms=2);
    water->AddElement(elO, natoms=1);
    gMan->SetDrawAtt(water,kBlue);

    //Copper
    density = 8.94*g/cm3;
    G4Material* copper = new G4Material(name="Copper",density, nel=1);
    copper->AddElement(elCu,100.*perCent);
    gMan->SetDrawAtt(copper,kRed+1,0.3);

    //Lead
    density = 11.35*g/cm3;
    G4Material* lead = new G4Material(name="Lead",density, nel=1);
    lead->AddElement(elPb,100.*perCent);
    gMan->SetDrawAtt(lead,kGray+1);

    //Brass -- The density is from simetric.co.uk is 8400 -- 8730 gm/cm3
    density = 8.50*g/cm3;
    G4Material* brass = new G4Material(name="Brass", density, nel=2);
    brass->AddElement(elCu, fractionmass = 90*perCent);
    brass->AddElement(elZn, fractionmass = 10*perCent);
    gMan->SetDrawAtt(brass,kOrange-2);

    //Bronze -- The density is from simetric.co.uk is 7700 -- 8920 gm/cm3
    density = 8.5*g/cm3;
    G4Material* bronze = new G4Material(name="Bronze", density, nel=2);
    bronze->AddElement(elCu, fractionmass = 60*perCent);
    bronze->AddElement(elSn, fractionmass = 40*perCent);
    gMan->SetDrawAtt(bronze,kOrange-3);

    //Argon
    density     = 1.66*mg/cm3;    
    pressure    = 1*atmosphere;
    temperature = 293.15*kelvin;
    G4Material* argon =  new G4Material(name="Ar", density,
                                        nel=1,kStateGas,temperature,
                                        pressure);
    argon->AddElement(elAr, natoms=1);
    gMan->SetDrawAtt(argon,kBlue-10,0.1);

    //Methane
    density     = 0.667*mg/cm3;
    pressure    = 1*atmosphere;
    temperature = 293.15*kelvin;
    G4Material* methane = new G4Material(name="CH4", density,
                                         nel=2,kStateGas,temperature,
                                         pressure);
    methane->AddElement(elC, natoms=1);
    methane->AddElement(elH, natoms=4);
    gMan->SetDrawAtt(methane,kBlue-10,0.1);

    //Argon + 10% Methane
    density     = 2.33*mg/cm3;
    G4Material* gasMixture 
        = new G4Material(name="GasMixture", density, ncomponents=2);
    gasMixture->AddMaterial(argon, fractionmass = 90*perCent);
    gasMixture->AddMaterial(methane, fractionmass = 10*perCent);
    gMan->SetDrawAtt(gasMixture,kBlue-10,0.1);

    // CarbonDioxide
    density     = 1.828*mg/cm3;
    pressure    = 1.*atmosphere;
    temperature = 293.15*kelvin;
    G4Material* CO2 = new G4Material(name="CO2", density,
                                     nel=2,kStateGas,temperature,
                                     pressure);
    CO2->AddElement(elC, natoms=1);
    CO2->AddElement(elO, natoms=2);
    gMan->SetDrawAtt(CO2,kBlue-10,0.1);

    // CarbonTetrafluoride
    density     = 3.66*mg/cm3;
    pressure    = 1.*atmosphere;
    temperature = 293.15*kelvin;
    G4Material* CF4 = new G4Material(name="CF4", density,
                                     nel=2,kStateGas,temperature,
                                     pressure);
    CF4->AddElement(elC, natoms=1);
    CF4->AddElement(elF, natoms=4);
    gMan->SetDrawAtt(CF4,kBlue-10,0.1);

    // Isobutane
    density     = 2.47*mg/cm3;
    pressure    = 1.*atmosphere;
    temperature = 293.15*kelvin;
    G4Material* C4H10 = new G4Material(name="C410", density,
                                     nel=2,kStateGas,temperature,
                                     pressure);
    C4H10->AddElement(elC, natoms=4);
    C4H10->AddElement(elH, natoms=10);
    gMan->SetDrawAtt(C4H10,kBlue-10,0.1);

    // Ar-CO2
    density     = 1.958*mg/cm3;
    G4Material* gasMixtureArCO2
        = new G4Material(name="GasMixtureArCO2", density, ncomponents=2);
    gasMixtureArCO2->AddMaterial(argon, fractionmass = 90*perCent);
    gasMixtureArCO2->AddMaterial(CO2, fractionmass = 10*perCent);
    gMan->SetDrawAtt(gasMixtureArCO2,kBlue-10,0.1);

    // TPC Gas
#ifdef OLD_TPC_DENSITY
    density     = 1.485*mg/cm3;
#else
    // Gas density calculated by R. Openshaw [See bug 402 for a full
    // discussion].  The density is calculated for a Ar/CF4/iBu 95:03:02
    // mixture at Normal Pressure and Temperature (NTP).
    density     = 1.738*mg/cm3;
#endif
    G4Material* gasMixtureTPC
        = new G4Material(name="GasMixtureTPC", density, ncomponents=3);
    gasMixtureTPC->AddMaterial(argon, fractionmass = 95*perCent);
    gasMixtureTPC->AddMaterial(CF4, fractionmass = 3*perCent);
    gasMixtureTPC->AddMaterial(C4H10, fractionmass = 2*perCent);
    gMan->SetDrawAtt(gasMixtureTPC,kBlue-10,0.1);

    // Water-SSteel-Air Mixture
    density     = 2.646*g/cm3;
    G4Material* waterSystem
        = new G4Material(name="WaterSystem", density, ncomponents=3);
    waterSystem->AddMaterial(water, fractionmass = 18*perCent);
    waterSystem->AddMaterial(steel, fractionmass = 32*perCent);
    waterSystem->AddMaterial(CO2, fractionmass = 50*perCent);
    gMan->SetDrawAtt(waterSystem,kBlue-7);
    
    // add TPC field cage mixture NB rough guesses !!!!
    density = 0.221*g/cm3; // this gives 1.4 10-2 X0 for 2.2 cm
    a = 16.*g/mole;
    G4Material* tpcFieldCage 
        = new G4Material(name="TPCWallMaterial",8,a,density);
    gMan->SetDrawAtt(tpcFieldCage,kYellow-7);
    
    // Titanium Dioxide -- Used in coextruded scintillator.
    density     = 4.26*g/cm3;
    G4Material* TIO2 = new G4Material(name="TIO2", density, nel=2);
    TIO2->AddElement(elTi, natoms=1);
    TIO2->AddElement(elO , natoms=2);
    gMan->SetDrawAtt(TIO2,kWhite);
    
    // Polystyrene -- This is polystyrene defined in the PDG C6H5CH=CH2 (this
    // is a net C8H8)
    density = 1.050*g/cm3; // the density of the "extruded polystyrene"
    G4Material* polystyrene 
        = new G4Material(name="Polystyrene", density, nel=2);
    polystyrene->AddElement(elC, 8);
    polystyrene->AddElement(elH, 8);
    gMan->SetDrawAtt(polystyrene,kGray+3);
    
    // Scintillator -- This is the average polystyrene plastic scintillator as
    // defined in the PDG C6H5CH=CH2 (this is a net C8H8).  The SMRD and ECal
    // geometries have been tuned to this value.  The 1.05 value represents a
    // typical extruded polystyrene.
    density = 1.050*g/cm3; 
    G4Material* scintillator 
        = new G4Material(name="Scintillator", density, nel=2);
    scintillator->AddElement(elC, 8);
    scintillator->AddElement(elH, 8);
    gMan->SetDrawAtt(scintillator,kGreen);

    // P0D Scintillator -- This has the average polystyrene plastic
    // scintillator composition as defined in the PDG C6H5CH=CH2 (this is a
    // net C8H8).  The density of the P0D scintillator is taken from the assay
    // of the MINERvA scintillator which is manufactured on the same equipment
    // as the P0D scintillator and is chemically the same.  The measured
    // density of the MINERvA strips are 1.058 +- 0.003, including the Ti02
    // coating.  This corresponds to a scintillator density of 1.0506, which
    // has been rounded to 1.051
    density = 1.051*g/cm3; 
    G4Material* p0dScintillator 
        = new G4Material(name="P0DScintillator", density, nel=2);
    p0dScintillator->AddElement(elC, 8);
    p0dScintillator->AddElement(elH, 8);
    gMan->SetDrawAtt(p0dScintillator,kGreen);

    // FGD Scintillator -- This is the average polystyrene scintillator for
    // the FGD.  The FGD density is based on our measurements of individual
    // bars, in combination with the measurements of the cross-sectional area
    // of the bars.
    density = 1.032*g/cm3; 
    G4Material* fgdScintillator 
        = new G4Material(name="FGDScintillator", density, nel=2);
    fgdScintillator->AddElement(elC, 8);
    fgdScintillator->AddElement(elH, 8);
    gMan->SetDrawAtt(fgdScintillator,kGreen);

    // Scintillator coating.  This is the coating that goes around the average
    // plastic scintillator. It is 15% TiO2 and 85% polystyrene by weight.
    // rho = (m_Sc + m_Ti) / (V_Sc + V_Ti)
    //     = (0.85 + 0.15) / ( 0.85/1.032 + 0.15/4.26 )
    //     = 1.164 g/cm^3
    density = 1.164*g/cm3;
    G4Material* scintillatorCoating
        = new G4Material(name="ScintillatorCoating", density, ncomponents=2);
    scintillatorCoating->AddMaterial(TIO2        ,fractionmass = 15*perCent);
    scintillatorCoating->AddMaterial(scintillator,fractionmass = 85*perCent);
    gMan->SetDrawAtt(scintillatorCoating,kGreen);
    
    // PVC -- Polyvinyl Chloride CH2=CHCl = C3H3Cl
    density = 1.38*g/cm3;
    G4Material* pvc
        = new G4Material(name="PVC", density, nel=3);
    pvc->AddElement(elC, 3);
    pvc->AddElement(elH, 3);
    pvc->AddElement(elCl, 1);
    gMan->SetDrawAtt(pvc,kGray+1);

    // HDPE -- High Density Polyethylene used in P0D water bag
    density = 0.94*g/cm3;
    G4Material* hdpe
        = new G4Material(name="HDPE", density, nel=2);
    hdpe->AddElement(elC, natoms=1);
    hdpe->AddElement(elH, natoms=2);
    gMan->SetDrawAtt(hdpe,kCyan-10);
    
    // P0DuleEpoxy -- The Epoxy used to hold the P0Dule together.  Density is
    // from the Henkel Hysol data sheet.
    density = 1.36*g/cm3;
    G4Material* p0duleEpoxy
        = new G4Material(name="P0DuleEpoxy", density, nel=3);
    p0duleEpoxy->AddElement(elC, 3);
    p0duleEpoxy->AddElement(elH, 3);
    p0duleEpoxy->AddElement(elCl, 1);
    gMan->SetDrawAtt(p0duleEpoxy,kBlue+4);
    
    // FGD Glue
    density = 1.365*g/cm3;
    G4Material* fgdGlue
        = new G4Material(name="FGDGlue", density, nel=4);
    fgdGlue->AddElement(elO,fractionmass=62.5*perCent);
    fgdGlue->AddElement(elC,fractionmass=27.9*perCent);
    fgdGlue->AddElement(elH,fractionmass= 8.4*perCent);
    fgdGlue->AddElement(elN,fractionmass= 1.2*perCent);
    gMan->SetDrawAtt(fgdGlue,kOrange);
    
    // FGD Water Module Epoxy
    density = 0.6573*g/cm3;
    G4Material* fgdWaterModuleEpoxy
        = new G4Material(name="FGDWaterModuleEpoxy", density, nel=4);
    fgdWaterModuleEpoxy->AddElement(elO,fractionmass=62.5*perCent);
    fgdWaterModuleEpoxy->AddElement(elC,fractionmass=27.9*perCent);
    fgdWaterModuleEpoxy->AddElement(elH,fractionmass= 8.4*perCent);
    fgdWaterModuleEpoxy->AddElement(elN,fractionmass= 1.2*perCent);
    gMan->SetDrawAtt(fgdWaterModuleEpoxy,kOrange);
     
    
    // Polypropylene
    density = 0.9*g/cm3;
    G4Material* polypropylene
        = new G4Material(name="Polypropylene", density, nel=2);
    polypropylene->AddElement(elC, 3);
    polypropylene->AddElement(elH, 6);
    gMan->SetDrawAtt(polypropylene,kYellow-5);

    // Polycarbonate
    density = 1.2*g/cm3;
    G4Material* polycarbonate
        = new G4Material(name="Polycarbonate", density, nel=3);
    polycarbonate->AddElement(elH,fractionmass= 5.5491*perCent);
    polycarbonate->AddElement(elC,fractionmass=75.5751*perCent);
    polycarbonate->AddElement(elO,fractionmass=18.8758*perCent);
    gMan->SetDrawAtt(polycarbonate,kYellow-2);

    //carbon fibre
    // NB : chemical composition may not be completely right but  close 
    density = 1.75*g/cm3;
    G4Material* carbonFibre
        = new G4Material(name="CarbonFibre", density, nel=2);
    carbonFibre->AddElement(elC,6);
    carbonFibre->AddElement(elO,1);
    gMan->SetDrawAtt(carbonFibre,kGray+3);

    

    // Diluted G10-FGD1
    density = 0.365*g/cm3;
    G4Material* g10fgd1
        = new G4Material(name="G10FGD1", density, nel=6);
    g10fgd1->AddElement(elH,6.2*perCent);
    g10fgd1->AddElement(elC,36.8*perCent);
    g10fgd1->AddElement(elO,30.7*perCent);
    g10fgd1->AddElement(elSi,21.9*perCent);
    g10fgd1->AddElement(elB,2.2*perCent);
    g10fgd1->AddElement(elNa,2.2*perCent);
    gMan->SetDrawAtt(g10fgd1,kGreen+3);

    // Diluted G10-FGD2
    density = 0.171*g/cm3;
    G4Material* g10fgd2
        = new G4Material(name="G10FGD2", density, nel=6);
    g10fgd2->AddElement(elH,6.2*perCent);
    g10fgd2->AddElement(elC,36.8*perCent);
    g10fgd2->AddElement(elO,30.7*perCent);
    g10fgd2->AddElement(elSi,21.9*perCent);
    g10fgd2->AddElement(elB,2.2*perCent);
    g10fgd2->AddElement(elNa,2.2*perCent);
    gMan->SetDrawAtt(g10fgd2,kGreen+3);

  
    // add Al/Rohacell TPC cage wall material
    // Outer box 15.2mm Al / Roha Panels (2 x 0.76mm Al, and 13.68mm Rohacell)
    // rho = (m_Al + m_Roha)/(V_Al + V_Roha)
    //     = (rho_Al * A*1.52mm + rho_Roha * A*13.68 mm)/(A*1.52mm + A*13.68mm)
    //     = (2.7g/cm3 * 1.52mm + 0.052g/cm3 * 13.68 mm) / 15.2mm
    //     = 0.3168 g/cm3

    density = 0.3168*g/cm3;

    G4Material* alroha
        = new G4Material(name="AlRoha", density, ncomponents=2);

    // Fraction by weight:
    //  w_Al = m_Al / m_total
    //       = (rho_Al*A*1.52mm) / (rho*A*15.2mm)
    //       = (2.7g/cm3*1.52mm) / (0.3168g/cm3*15.2mm)
    //       = 0.8523
    //  w_Ro = m_Roha / m_total
    //       = (rho_Roha*A*1.52mm) / (rho*A*15.2mm)
    //       = (0.052g/cm3*13.68mm) / (0.3168g/cm3*15.2mm)
    //       = 0.1477
    alroha->AddMaterial(aluminum, fractionmass = 85.23*perCent);
    alroha->AddMaterial(rohacell, fractionmass = 14.77*perCent);
    gMan->SetDrawAtt(alroha,kGreen-9);

    // add Al/Rohacell TPC cage wall material
    // (same as above but only 13.2mm thick)
    density = 0.3569*g/cm3;

    G4Material* alroha2
        = new G4Material(name="AlRoha2", density, ncomponents=2);

    alroha2->AddMaterial(aluminum, fractionmass = 87.11*perCent);
    alroha2->AddMaterial(rohacell, fractionmass = 12.89*perCent);
    gMan->SetDrawAtt(alroha2,kGreen-9);

    
    // FGD Electronics card (95mm x 252mm x 8mm) with total mass of 400g. The
    // composition is 50% Al and 50% G10 by weight.  -> density =
    // 400g/191.52cm3
    density = 2.0885*g/cm3;

    G4Material* alg10
        = new G4Material(name="AlG10", density, ncomponents=2);

    alg10->AddMaterial(aluminum, fractionmass = 50.0*perCent);
    alg10->AddMaterial(g10, fractionmass = 50.0*perCent);
    gMan->SetDrawAtt(alg10,kGreen-9);

    // Material for the fiber core.
    density = 1.05*g/cm3;
    G4Material* fiberCore
        = new G4Material(name="FiberCore", density, nel=2);
    fiberCore->AddElement(elC, 9);
    fiberCore->AddElement(elH, 10);
    gMan->SetDrawAtt(fiberCore,kCyan-4);
    
    // Material for the fiber cladding
    density = 1.19*g/cm3;
    G4Material* fiberCladding
        = new G4Material(name="FiberCladding", density, nel=3);
    fiberCladding->AddElement(elH,8);
    fiberCladding->AddElement(elC,5);
    fiberCladding->AddElement(elO,2);
    gMan->SetDrawAtt(fiberCladding,kBlue);

    // Fluorinated fiber cladding
    density = 1.43*g/cm3;
    G4Material* fiberCladdingFluor
        = new G4Material(name="FiberCladdingFluor", density, nel=2);
    fiberCladdingFluor->AddElement(elH,4);
    fiberCladdingFluor->AddElement(elC,2);
    gMan->SetDrawAtt(fiberCladdingFluor,kBlue);
    
    // ActiveWater for the FGD.
    //
    // It is not necessarily activated, it is just a combination of water and
    // polycarbonate that holds the water in place.  Daniel's document says
    // the panel is 342mg/cm2 of PC (for most of the panels), and 2.2226 g/cm2
    // of water (average of the 6 deployed panels).  water+PC layer by mass is
    // 0.342/(0.342+2.2226)=0.1335 polycarbonate, so rho = 1 / (0.1335/1.2 +
    // 0.8666/1.0) = 1.0226 g/cm^3
    density = 1.0226*g/cm3;
    G4Material* activeWater 
        = new G4Material("ActiveWater", density, ncomponents=2);
    activeWater->AddMaterial(water        ,fractionmass = 86.672*perCent);
    activeWater->AddMaterial(polycarbonate,fractionmass = 13.328*perCent);
    gMan->SetDrawAtt(activeWater,kAzure+8);

    // Print all the materials defined.
    ND280Log(*(G4Material::GetMaterialTable()));
*/
}