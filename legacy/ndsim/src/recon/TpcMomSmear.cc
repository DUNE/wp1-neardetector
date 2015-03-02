//____________________________________________________________________________
/*!

\class    TpcMomSmear

\author   Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool


\created  Jan 2014
\last update June 2014

*/
//____________________________________________________________________________
#include "TpcMomSmear.hh"

TpcMomSmear::TpcMomSmear() : magneticField(0.0),deltaSagitta(0.0){
  Initialize();
}

TpcMomSmear::TpcMomSmear(double magField,double deltaSag) : magneticField(magField),deltaSagitta(deltaSag){
  Initialize();
}

TpcMomSmear::~TpcMomSmear(){
  delete rand;
}

void TpcMomSmear::Initialize(){

  //use random number generator  TRandom3
  rand = new TRandom3(0);
}

const double TpcMomSmear::getTrackLength(HitCollection &track){

	//get the range of the track
	TLorentzVector initPos = track.front().getPosition() * (1./CLHEP::m);
	TLorentzVector lastPos = track.back().getPosition() * (1./CLHEP::m);
	
	//don't count the x positions - only transverse coordiantes to mag field
        double trackLength = TMath::Sqrt( TMath::Power(lastPos.Y() - initPos.Y(),2)  
				  	+ TMath::Power(lastPos.Z() - initPos.Z(),2) );

	trackLength *= CLHEP::m; //in meters
	return trackLength;

}

const double TpcMomSmear::getTruthTransMomentum(HitCollection &track){
	
	double momY = track.front().getP4().Y() * (1./CLHEP::GeV);
	double momZ = track.front().getP4().Z() * (1./CLHEP::GeV);
	double momentumTrans = TMath::Sqrt((momY*momY) + (momZ*momZ));

	momentumTrans *= CLHEP::GeV;
	return momentumTrans;	//in GeV/c
}

const double TpcMomSmear::getTruthMomentum(HitCollection &track){
	
	double momX = track.front().getP4().X() * (1./CLHEP::GeV);
	double momY = track.front().getP4().Y() * (1./CLHEP::GeV);
	double momZ = track.front().getP4().Z() * (1./CLHEP::GeV);
	double momentum = TMath::Sqrt((momX*momX) + (momY*momY) + (momZ*momZ));

	momentum *= CLHEP::GeV;
	return momentum;	//in GeV/c
}

const double TpcMomSmear::getReconTransMomentum(double trackLength,double sagitta){

	//if it is less than the resolution then let the sagitta = resolution
	if(sagitta<deltaSagitta)sagitta = deltaSagitta;

	double reconTransMomentum = (magneticField*(1./CLHEP::tesla) * trackLength*(1./CLHEP::m) * trackLength*(1./CLHEP::m))/(26.7*sagitta*(1./CLHEP::m));
	
	reconTransMomentum *= CLHEP::GeV;
	return reconTransMomentum;	// in GeV/c
}

const double TpcMomSmear::getTrackSagitta(HitCollection &track,double transMom){

	double trackLength = getTrackLength(track);	// in meters
	//trackLength *= CLHEP::m;

	double sagitta = (magneticField*(1./CLHEP::tesla)* trackLength*(1./CLHEP::m) * trackLength*(1./CLHEP::m))/(26.7*transMom*(1./CLHEP::GeV) );
	sagitta *= CLHEP::m; //in meters
	
	return sagitta;
}

const double TpcMomSmear::getReconSagitta(HitCollection &tpcTrack){

	//get the first and last positions of track
	TLorentzVector initPos = tpcTrack.front().getPosition() * (1./CLHEP::m);
	TLorentzVector lastPos = tpcTrack.back().getPosition() * (1./CLHEP::m);

	//get the midpoint
	TLorentzVector midPoint = (initPos + lastPos)*(1./2.);

	double sagitta = 0.;
	//loop over the hits to find best match for sagitta
	for(int j=0;j<tpcTrack.size();j++){
	  TLorentzVector hitPos = tpcTrack.at(j).getPosition() * (1./CLHEP::m);
	  double sag = TMath::Sqrt( (hitPos.X() - midPoint.X())*(hitPos.X() - midPoint.X()) + 
				    (hitPos.Y() - midPoint.Y())*(hitPos.Y() - midPoint.Y()) +
				    (hitPos.Z() - midPoint.Z())*(hitPos.Z() - midPoint.Z()) );
	  if( (sagitta ==0.) || (sagitta<sag) )sagitta==sag;

	}

	sagitta *= CLHEP::m; //in meters
	return sagitta;
}

const double TpcMomSmear::getSmearSagitta(double truthSag){

	double sagittaSmear = rand->Gaus(truthSag*(1./CLHEP::m),deltaSagitta*(1./CLHEP::m));
	sagittaSmear *= CLHEP::m; //in meters

	//if it is less than the resolution then let the sagitta = resolution
	if( (sagittaSmear*(1./CLHEP::m)) < (deltaSagitta*(1./CLHEP::m)) )sagittaSmear = deltaSagitta;

	return sagittaSmear;
}

const double TpcMomSmear::particleMomentum(HitCollection &track){

	//get the range of the track
        double particleTrackLength = getTrackLength(track);// * (1./CLHEP::m);
	//particleTrackLength *= CLHEP::m;

	//get the truth trans momentum
	double particleMomTrans_truth = getTruthTransMomentum(track);// * (1./CLHEP::GeV);
	//particleMomTrans_truth *= CLHEP::GeV;

	double particleMom_truth = getTruthMomentum(track);// * (1./CLHEP::GeV);
	//particleMom_truth *= CLHEP::GeV;
	
	// calculate the sagitta from truth p
	double particleTrackSagitta_cal = getTrackSagitta(track,particleMomTrans_truth);// *(1./CLHEP::m);	
	//particleTrackSagitta_cal *= CLHEP::m;

	// draw from gaussian of recalculated sagitta a random value
	double particleTrackSagitta_rand = getSmearSagitta(particleTrackSagitta_cal);// *(1./CLHEP::m);
	//particleTrackSagitta_rand *= CLHEP::m;
	
	//if(particleTrackSagitta_rand<0.)particleTrackSagitta_rand = -particleTrackSagitta_rand;//*1.0e-6;	//don't allow negative values for the sagitta

	// reconstructed transverse momentum is then recalculated from gaussian values
	double particleMomTrans_recon = getReconTransMomentum(particleTrackLength,particleTrackSagitta_rand);//* (1./CLHEP::GeV);
	//particleMomTrans_recon *= CLHEP::GeV;

	double deltaP_trans = deltaSagitta*particleMomTrans_recon/(particleTrackSagitta_rand);//* (1./CLHEP::GeV);
	//deltaP_trans *= CLHEP::GeV;

	return particleMomTrans_recon;
}
//ccqe defined as a lepton (e,mu,tau) in the final state with a proton only!
//nu_{l} + n -> l + p
const double TpcMomSmear::ccqeReconstruct(ParticleDescrShortRecord * protonTruthInfo,HitCollection &lepton,NeutrinoEvent* nu_event){
	 
	if(!protonTruthInfo)return -1.;
		 			
	TLorentzVector initPos = lepton.at(0).getPosition() * (1./CLHEP::m);
	TLorentzVector nextPos = lepton.at(1).getPosition() * (1./CLHEP::m);
	TLorentzVector leptonDir = (nextPos - initPos);
	TLorentzVector beamDir(0.,0.,1.);

	//angle is the projection of leptonDir onto beam axis (z-axis)
	double cosTheta = leptonDir.Dot(beamDir) / leptonDir.Mag();
	if(cosTheta<0)cosTheta= -cosTheta;	//cos is an even function but only take positive value
	
	//mass difference squared between neutron and proton = m_n*m_n - m_p*m_p in (GeV/c2)2			
	double deltaMassSquared = 2.42867e-3;
	double protonMass = protonTruthInfo->getMass() * (1./CLHEP::GeV);	
	double leptonMass = 0.;
	leptonMass = nu_event->getFspl().getMass() * (1./CLHEP::GeV);
	double leptonMomRecon = particleMomentum(lepton) * (1./CLHEP::GeV);
	/*
		std::cout << "\nPROTON MASS: " << protonMass << " GeV/c2";
		std::cout << "\nLEPTON MASS: " << leptonMass << " GeV/c2";
		std::cout << "\nLEPTON MOM RECON: " << leptonMomRecon << " GeV/c";
	*/

	double leptonEnergyRecon = TMath::Sqrt( (leptonMomRecon*leptonMomRecon) + (leptonMass*leptonMass) );

	//Apply formulae to reconstruct CCQE neutrino using fspl
	double ccqeNeutrinoMom = ( (leptonEnergyRecon*protonMass) -
				(leptonMass*leptonMass/2.0) ) /
				(protonMass - leptonEnergyRecon + (leptonMomRecon*cosTheta) );

	return ccqeNeutrinoMom;
}