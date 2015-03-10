//____________________________________________________________________________
/*!

\class    PiZeroFinder

\author   Tom Stainer <tstainer \at liv.ac.uk>
          University of Liverpool

\author   Georgios Christodoulou <georgios at hep.ph.liv.ac.uk>
          University of Liverpool

\created  May 2014
\last update June 2014

*/
//____________________________________________________________________________
#include "Tracker.hh"

Tracker::Tracker(){
}

Tracker::Tracker(double scintRes) : scintResolution_(scintRes){

  sphereRad_ = scintRes;

}

Tracker::~Tracker(){
}

tpcTracks Tracker::MakeFromHits(HitCollection &hitsVector,bool primaryOnly){

  //the track vector which will be returned
  tpcTracks trackVector;
  trackVector.resize(0);
  int trackVtrSize = trackVector.size();

  //first sort the hits by trackID
  //SortHitsByTrackID(hitsVector); 
  //PrintOutHits(hitsVector);

  //loop over hitsVector
  for(int i=0;i<hitsVector.size();i++){
	SDHit tmpScintHit = hitsVector.at(i);

	bool hitSetToTrack = false;

	//get the track and parent IDs
	int hitTrackID = tmpScintHit.getTrackID();
	int hitParentID = tmpScintHit.getParentID();

	if(primaryOnly && hitParentID != 0)continue;		//skip this hit as only concerned with primary tracks

	//fill the first entry into the track vector
	if(trackVector.size()==0){
	  HitCollection firstTrack;
	  firstTrack.push_back(tmpScintHit);
	  trackVector.push_back(firstTrack);
	  continue;
	}

	//loop over trackVector to check if trackID or ParentID exists
	for(int j=0;j<trackVector.size();j++){
	  if(hitSetToTrack)break;

	  int trackIDFrontInVector = trackVector.at(j).front().getTrackID();

	  //does the track already exist with the same ID?
	  if(trackIDFrontInVector == hitTrackID){
		trackVector.at(j).push_back(tmpScintHit);
		hitSetToTrack = true;
		break;
	  }

	/* /// DO NOT INCLUDE THIS DEFINITION FOR TPC TRACKS AS WANT SINGLE START AND END POINTS ///
	  //Otherwise must check all parentIDs with trackIDs
	  //loop over hits in track to compare all trackIDs to ParentIDs to 
	  // match daughters to parent
	  for(int k =0;k<trackVector.at(j).size();k++){
	  	int trackIDInVector = trackVector.at(j).at(k).getTrackID();	
	  	int parentIDInVector = trackVector.at(j).at(k).getParentID();	
		if((trackIDInVector == hitParentID) || (trackIDInVector == hitTrackID) ){
		  trackVector.at(j).push_back(tmpScintHit);
		  hitSetToTrack = true;
		  break;
		}
	  }
	*/
	}
	//if cannot match trackID or parentID so must be a new track
	if(!hitSetToTrack){
	  HitCollection newTrack;
	  newTrack.push_back(tmpScintHit);
	  trackVector.push_back(newTrack);  
	}
  }
  
  return trackVector;
}

scintTracks Tracker::MakeFromHits(ScintHitCollection &hitsVector){

  //the track vector which will be returned
  scintTracks trackVector;
  trackVector.resize(0);
  int trackVtrSize = trackVector.size();

  //first sort the hits by trackID
  SortByTrackID(hitsVector); 
  //PrintOutHits(hitsVector);

  //loop over hitsVector
  for(int i=0;i<hitsVector.size();i++){
	scintHit tmpScintHit = hitsVector.at(i);

	bool hitSetToTrack = false;

	//get the track and parent IDs
	int hitTrackID = tmpScintHit.getTrackID();
	int hitParentID = tmpScintHit.getParentID();

	//fill the first entry into the track vector
	if(i==0){
	  ScintHitCollection firstTrack;
	  firstTrack.push_back(tmpScintHit);
	  trackVector.push_back(firstTrack);
	  continue;
	}

	//loop over trackVector to check if trackID or ParentID exists
	for(int j=0;j<trackVector.size();j++){
	  if(hitSetToTrack)break;

	  int trackIDFrontInVector = trackVector.at(j).front().getTrackID();

	  //does the track already exist with the same ID?
	  if(trackIDFrontInVector == hitTrackID){
		trackVector.at(j).push_back(tmpScintHit);
		hitSetToTrack = true;
		break;
	  }
	
	  //Otherwise must check all parentIDs with trackIDs
	  //loop over hits in track to compare all trackIDs to ParentIDs to 
	  // match daughters to parent
	  for(int k =0;k<trackVector.at(j).size();k++){
	  	int trackIDInVector = trackVector.at(j).at(k).getTrackID();	
	  	int parentIDInVector = trackVector.at(j).at(k).getParentID();	
		if( (trackIDInVector == hitParentID) || (trackIDInVector == hitTrackID) ){// || ((parentIDInVector == hitParentID) && hitParentID!=0) ){
		  trackVector.at(j).push_back(tmpScintHit);
		  hitSetToTrack = true;
		  break;
		}
	  }
	}
	//if cannot match trackID or parentID so must be a new track
	if(!hitSetToTrack){
	  ScintHitCollection newTrack;
	  newTrack.push_back(tmpScintHit);
	  trackVector.push_back(newTrack);  
	}
  }
  

  return trackVector;

}

int Tracker::MatchScintToTpcTracks(ScintHitCollection &scintTrack,tpcTracks &tpcTracksVector){

	int matchedTrackPos = -1;
	
	//get the position and momentum of the first scint hit
	TLorentzVector scintFirstHitPos = scintTrack.front().getPosition() * (1./CLHEP::m);
	TLorentzVector scintFirstHitMom = scintTrack.front().getP4() * (1./CLHEP::GeV);

	TLorentzVector diffVector;
	double magnitude = 0.;
	double radius = TMath::Abs(5.0*scintResolution_* (1./CLHEP::m));
	//std::cout << "\nRADIUS = " << radius;
	double thetaCut = 1.0;//in degrees

	int tracksMatched = 0;
	
	//loop over tpc tracks
	while(true){
	  for(int i=0;i<tpcTracksVector.size();i++){
		HitCollection tempTpcTrackVtr = tpcTracksVector.at(i);

		//loop over hits on each track - slow but hits may not be time ordered
		// - can check if hit left volume
		for(int j=0;j<tempTpcTrackVtr.size();j++){
		  int leftVolume = tempTpcTrackVtr.at(j).getTrackLeftVolume();
		  if(leftVolume == 0)continue;

		  TLorentzVector tpcLastHitPos = tempTpcTrackVtr.at(j).getPosition() * (1./CLHEP::m);
		  TLorentzVector tpcLastHitMom = tempTpcTrackVtr.at(j).getP4() * (1./CLHEP::GeV);

		  diffVector = tpcLastHitPos - scintFirstHitPos;
		  magnitude = TMath::Sqrt(diffVector.X()*diffVector.X() + diffVector.Y()*diffVector.Y() +
				 diffVector.Z()*diffVector.Z());
		  //std::cout << "\nMAGNITUDE = " << magnitude;
	
		  //check if its in the sphere
		  if(magnitude<=radius){
		  	tracksMatched++;
		  
		  	//theta must be less than 5 deg
		  	double dotProd = tpcLastHitMom.X()*scintFirstHitMom.X() +
				     tpcLastHitMom.Y()*scintFirstHitMom.Y() +
				     tpcLastHitMom.Z()*scintFirstHitMom.Z();
		  	double magTpcMom = TMath::Sqrt(tpcLastHitMom.X()*tpcLastHitMom.X() +
					tpcLastHitMom.Y()*tpcLastHitMom.Y() +
					tpcLastHitMom.Z()*tpcLastHitMom.Z());
		  	double magScintMom = TMath::Sqrt(scintFirstHitMom.X()*scintFirstHitMom.X() +
					scintFirstHitMom.Y()*scintFirstHitMom.Y() +
					scintFirstHitMom.Z()*scintFirstHitMom.Z());
		  	double cosTheta = dotProd / (magTpcMom*magScintMom);
		  	double theta = TMath::Abs(TMath::ACos(cosTheta)*180/TMath::Pi());
		  	//std::cout << "\nTHETA = " << theta;
		  	//match to the momentum of the track as well
		  	if(theta<=thetaCut){
			  matchedTrackPos = i;
			  break;
		 	  //std::cout << "\nTRACK MATCHED TO TPC!" << std::endl;
		  	}
		  }
		}
	    }
	  if(tracksMatched==1)break;
	  //if more than one is matched increase the restriction on the angle
	  else if(tracksMatched>1){
		thetaCut = thetaCut-0.1;
		radius = radius-1.0;
		if(radius<0 || thetaCut<0)break;
	  }
	  //loop over again with wider radius and wider theta
	  else if(tracksMatched==0){
		thetaCut = thetaCut+0.1;
		radius = radius+1.0;
		if(radius>10. || thetaCut>10.)break;		
	  }
	}

	return matchedTrackPos;
}

scintTracks Tracker::SphereTrackReconstruction(ScintHitCollection &hitsVector){

  ///needs to be completed - abandoned sphere model - CURRENTLY USING TRUTH ID's OF TRACKS///

  //first sort the hits by trackID ascending
  SortByTrackID(hitsVector);

  scintTracks trackVector;
  trackVector.resize(0);
  int trackVtrSize = trackVector.size();

  double radius = sphereRad_;

  TVector3 origin;
  bool trackSet = true;

  //hit vector to make up a track
  ScintHitCollection scint_track;

  //loop over hits once
  for(int i=0;i<hitsVector.size();i++){

  	//draw 4 spheres around origin sphere
  	Sphere originSphere(radius,0);
  	Sphere rightSphere(radius,1);
  	Sphere topSphere(radius,2);
  	Sphere leftSphere(radius,3);
  	Sphere bottomSphere(radius,4);

	//create a basis scintHit
	scintHit basisSDHit(hitsVector.at(i));
	TVector3 hitOrigin;

	//if the track had been set previously it means that must get new origin from hit
	if(trackSet){
	  scint_track.resize(0);
	  hitOrigin.SetXYZ(basisSDHit.getPosition().X(),basisSDHit.getPosition().Y(),basisSDHit.getPosition().Z());
	}
	else hitOrigin = origin;

	//loop over hits again this time starting from i
	for(int j=i;j<hitsVector.size();j++){

	  //create a temp scintHit
	  scintHit tempSDHit(hitsVector.at(j));
	  //get the position of the hit
	  TVector3 hitPos(tempSDHit.getPosition().X(),tempSDHit.getPosition().Y(),tempSDHit.getPosition().Z());
	  //energy deposition
	  double eDep = tempSDHit.getEdep();

	  //skip the matching hits - if the position is the same it must be the same hit
	  if(hitPos==hitOrigin)continue;

	  //check which hits are in intersections with adjacent spheres to determine which sphere will form new origin
	  if(IsInSphereIntersection(hitOrigin,hitPos,rightSphere.getPosition())){
		rightSphere.incrementHitCount();
		rightSphere.addToHitCollection(tempSDHit);
	  }
	  if(IsInSphereIntersection(hitOrigin,hitPos,leftSphere.getPosition())){
		leftSphere.incrementHitCount();
		leftSphere.addToHitCollection(tempSDHit);
	  }
	  if(IsInSphereIntersection(hitOrigin,hitPos,topSphere.getPosition())){
		topSphere.incrementHitCount();
		topSphere.addToHitCollection(tempSDHit);
	  }
	  if(IsInSphereIntersection(hitOrigin,hitPos,bottomSphere.getPosition())){
		bottomSphere.incrementHitCount();
		bottomSphere.addToHitCollection(tempSDHit);
	  }	
	  
	  //check if hit is in sphere and add it to its hits collection
	  // while also removing it from the hitsVector
	  if(IsInSphere(hitOrigin,hitPos,originSphere.getRadius())){
		originSphere.incrementHitCount();
		originSphere.addEdep(eDep);
		originSphere.addToHitCollection(tempSDHit);
		//erase the entries to avoid double counting
		hitsVector.erase(hitsVector.begin()+i);
		//remember to decrement i if this happens as it will shift the positions in the vector by 1
		// i.e position 45 will be removed so position 46 is actually now position 47
		i--;
	  }
	  
	}

	//put into a vector to analise
	std::vector<Sphere *> spheresVtr;
	spheresVtr.push_back(&rightSphere);
	spheresVtr.push_back(&topSphere);
	spheresVtr.push_back(&leftSphere);
	spheresVtr.push_back(&bottomSphere);
	//sort the vector
	SortSpheresByCount(spheresVtr);
	
	//is the last one bigger than or equal to previous (k-1)?
	if(spheresVtr.at(spheresVtr.size()-1)->getHitCount() > spheresVtr.at(spheresVtr.size()-2)->getHitCount()){
		origin.SetXYZ(spheresVtr.at(spheresVtr.size()-1)->getPosition().X(),
				spheresVtr.at(spheresVtr.size()-1)->getPosition().Y(),
				spheresVtr.at(spheresVtr.size()-1)->getPosition().Z());
		trackSet = true;
	}
	//is it equal to or bigger than previous previous (k-2)? - rotate the spheres by 45deg
	else if(spheresVtr.at(spheresVtr.size()-1)->getHitCount() > spheresVtr.at(spheresVtr.size()-3)->getHitCount()){
	  for(int k=0;k<spheresVtr.size();k++){
		spheresVtr.at(k)->RotateAndReset();
	  }
	  //add the hits back into the hits collection
	  for(int k=0;k<originSphere.getHitCollection().size();k++){
		hitsVector.push_back(originSphere.getHitCollection().at(k));
	  }
	  originSphere.clearHitCollection();
	  continue;
	}
	//if it is equal to 3 or more spheres then increase the radius by 10% each time
	else{
	  radius *= 1.1;
	  i--;
	  for(int k=0;k<spheresVtr.size();k++){
	    spheresVtr.at(k)->Reset();
	  }
	  //add the hits back into the hits collection
	  for(int k=0;k<originSphere.getHitCollection().size();k++){
	    hitsVector.push_back(originSphere.getHitCollection().at(k));
    	  }
	  originSphere.clearHitCollection();
	  if(radius > 5.*sphereRad_) trackSet = true;
	  else continue;
	}

	//variables for new Hit to be used from 3 remaining spheres
	double totalEDep = basisSDHit.getEdep();

	if(trackSet){
	  scintHit newSDHit;
	  newSDHit.setEdep(totalEDep);
	  newSDHit.setPosition(TLorentzVector(origin.X(),origin.Y(),origin.Z(),0));

	  scint_track.push_back(newSDHit);					//add this hit to the vector
	  trackSet = false;
	}

	if(trackSet)trackVector.push_back(scint_track);			//first hit in event should be added

  }

  return trackVector;

}

scintTracks Tracker::PiZeroTracks(ScintHitCollection &hitsVector,std::vector<GeantPrimaryParticle> *primaries){

  scintTracks trackVector;
  trackVector.resize(0);

  //loop over primaries
  for(int k =0;k<primaries->size();k++){
  
    GeantPrimaryParticle * prim = &primaries->at(k);
   
    //skip anything other than pi0s
    if(prim->getPDG()!=111)continue;
 
    //get the daughters/descendants
    std::vector<GeantBasicParticle> descendants = prim->getDaughterVtr();

    //sort them by trackID
    SortByTrackID(descendants);

    //create ScintHitCollection for each track
    ScintHitCollection piZeroTrack;
    piZeroTrack.resize(0);

    int lastTrackID = -1;

    //loop over descendants first to find trackID
    for(int i=0;i<descendants.size();i++){
  
	GeantBasicParticle * p = &descendants.at(i);
	int trackID = p->getTrackID();

	//only want to count each track once
	if(lastTrackID == trackID)continue;

	//then loop over hits to match to trackIDs
	for(int j=0;j<hitsVector.size();j++){
	  scintHit * tempHit = &hitsVector.at(j);

	  int hitTrackID = tempHit->getTrackID();
	  int hitParentID = tempHit->getParentID();

	  if( (hitTrackID == trackID) || (hitParentID == trackID) ){
		piZeroTrack.push_back(*tempHit);
		//p->printToStream(std::cout);
		//std::cout << "\nTrackID: " << tempHit->getTrackID() <<std::endl;
	  }

	}
	
	lastTrackID = trackID;
    }

    trackVector.push_back(piZeroTrack);

  }

  return trackVector;
}

//get tracks of photons originating from pi Zeros
scintTracks Tracker::PiZeroPhotonTracks( const ScintHitCollection &hitsVector ){

  scintTracks trackVector;

  std::vector<int> photonIDVtr;

  for( int i=0;i<hitsVector.size();++i){
    if( hitsVector[i].getParent().getPDG() == 22 ){	

	const int photonID = hitsVector[i].getParent().getTrackID();

	int position = -1;
	
	//check if photon id is already found
	for(int j=0;j<photonIDVtr.size();++j){
	  if( photonID == photonIDVtr.at(j)){
		position = j;
		break;
	  }
	}	

	//if not found add new photon track
	if(position<0){
	  ScintHitCollection photon;

	  photonIDVtr.push_back(photonID);

	  photon.push_back(hitsVector[i]);
	  trackVector.push_back(photon);
	}
	else{
	 trackVector[position].push_back(hitsVector[i]);

	}
    }
  }

  return trackVector;

}


//get tracks of photons originating from pi Zeros
scintTracks Tracker::PiZeroPhotonTracks(ScintHitCollection &hitsVector,std::vector<GeantPrimaryParticle> *primaries){

  scintTracks trackVector;
  trackVector.resize(0);

  //sort the hits by trackID
  //SortByTrackID(hitsVector);
 
  //loop over primaries
  for(int i =0;i<primaries->size();i++){
  
    GeantPrimaryParticle * prim = &primaries->at(i);

    //skip anything other than pi0s
    if(prim->getPDG()!=111)continue;
 
    //get the daughters/descendants
    std::vector<GeantBasicParticle> descendants = primaries->at(i).getDaughterVtr();

    //sort them by trackID
    SortByTrackID(descendants);

    //get the trackID of the pi zero
    int primTrackID = prim->getTrackID();
	
    //secondary photons trackID
    int photonOneTrackID = 0;
    int photonTwoTrackID = 0;
    int count = 0;
    int noMatchCount = 0;
    int lastTrackID = -1;

    ScintHitCollection photonOneHits;
    ScintHitCollection photonTwoHits;
    photonOneHits.resize(0);
    photonTwoHits.resize(0);

    //loop over descendants
    for(int j=0;j<descendants.size();j++){

	GeantBasicParticle * p = &descendants.at(j);

	//only want to count each track once
	if(p->getTrackID() == lastTrackID)continue;
	lastTrackID = p->getTrackID();

	if( (p->getParentID() == primTrackID)  && (p->getPDG() ==22) ){

		//p->printToStream(std::cout);
		count++;

		if(count==1){
		  photonOneTrackID = p->getTrackID();
		  continue;
		}
		if(count==2){
		  photonTwoTrackID = p->getTrackID();
		  continue;
		}
	}

	//check if it is a descendant
	bool IsPhotonOneDescendant = IsDescendant(photonOneTrackID,p,descendants);
	bool IsPhotonTwoDescendant = IsDescendant(photonTwoTrackID,p,descendants);
	
	//std::cout << "\nIs Photon One descendant: " << IsPhotonOneDescendant;
	//std::cout << "\nIs Photon Two descendant: " << IsPhotonTwoDescendant;

	//check to make sure this should not happen - must be associated with one and only one!
	if( (!IsPhotonOneDescendant && !IsPhotonTwoDescendant) ){
		if(noMatchCount>1)continue;
		std::cout << "\nCannot match to either photon track!";
		p->printToStream(std::cout);
		noMatchCount++;
	}

	//find matching hit(s)
	for(int k =0;k<hitsVector.size();k++){
		
	  scintHit * tempHit = &hitsVector.at(k);

	  int hitTrackID = tempHit->getTrackID();
	  int hitParentID = tempHit->getParentID();
	
	  //match descendant to hit
	  if( (hitTrackID == p->getTrackID()) || (hitParentID == p->getTrackID()) ){
		
		if( IsPhotonOneDescendant ){
		  photonOneHits.push_back(*tempHit);
		  //remove from hits collection
		  hitsVector.erase( hitsVector.begin()+k );
		  k--;
		}
	  	else if( IsPhotonTwoDescendant ){
		  photonTwoHits.push_back(*tempHit);
		  //remove from hits collection
		  hitsVector.erase( hitsVector.begin()+k );
		  k--;
	  	}
	  }
	
	}

     }
     trackVector.push_back(photonOneHits);
     trackVector.push_back(photonTwoHits);
  }

  return trackVector;
}

//method to select if the hit exists in the intersection of the two sphere volumes
// hitCenterPos - is the position of the hit that the first sphere is drawn around
// nextHitPos - is the position of the hit to compare, if it is part of the track?
// spherePosition - is the position vector of the sphere to draw relative to original hit - local position
// -- 4 spheres at (r,0),(-r,0),(0,r) and (0,-r) relative to hitCenterPos will cover all space around hit
bool Tracker::IsInSphereIntersection(const TVector3 &hitCenterPos,const TVector3 &nextHitPos,const TVector3 &spherePosition){

  bool inIntersection = false;

  //vector of hitCenterPos to nextHitPos - call it AC
  TVector3 AC = nextHitPos - hitCenterPos;
  //vector of global spherePosition - call it OB
  TVector3 OB = nextHitPos + spherePosition;
  TVector3 AB = spherePosition;
  //vector of global spherePosition to nextHitPos - call it BC
  TVector3 BC = nextHitPos - OB;

  //condition must be mag(AB) <=R and mag(BC) <=R
  if( (AC.Mag() <= AB.Mag()) && (BC.Mag() <=AB.Mag()) ){
	inIntersection = true;
  }
  else inIntersection = false;

  return inIntersection;

}

bool Tracker::IsInSphere(const TVector3 &hitCenterPos,const TVector3 &nextHitPos,const double radius){

  bool inIntersection = false;

  //vector of hitCenterPos to nextHitPos - call it AC
  TVector3 AC = nextHitPos - hitCenterPos;

  //condition must be mag(AB) <=R
  if( (AC.Mag() <= radius) ){
	inIntersection = true;
  }
  else inIntersection = false;

  return inIntersection;

}

template<class dataTypeOne,class dataTypeTwo>
bool Tracker::IsDescendant(int parentTrackId,dataTypeOne* &daughter,dataTypeTwo &vector){

  int trackIDOfParent = parentTrackId;
  int parentIDOfDaughter = daughter->getParentID();

  //SortByTrackID(vector);

  int sizeVtr = vector.size();

  //loop over vector 
  for(int i=0;i<sizeVtr;i++){
	int trackIDInVtr = vector.at(i).getTrackID();
	int parentIDInVtr = vector.at(i).getParentID();

	if(trackIDOfParent > parentIDOfDaughter)return false;
	if(trackIDOfParent == parentIDOfDaughter)return true;

 	if(parentIDOfDaughter == trackIDInVtr){
	  if( (trackIDInVtr == trackIDOfParent) || (parentIDInVtr == trackIDOfParent) )return true;
	  else{
		parentIDOfDaughter = parentIDInVtr;
		sizeVtr = i;
		i=-1;
	  }
	}
  }
    
  return false;
}

template<class dataType>
void Tracker::SortByTrackID(dataType &vector){

  dataType sortedVtr;
  sortedVtr.resize(0);

  for(int i=0;i<vector.size();i++){
	int trackIDToCompare = vector.at(i).getTrackID();
	//add the first as its an empty vector to begin with
        if(i==0){
                sortedVtr.push_back(vector.at(i));
                continue;
        }
	for(int j=0;j<sortedVtr.size();j++){
                if(trackIDToCompare >= sortedVtr.at(j).getTrackID()){
                        if(j==sortedVtr.size()-1){
                                sortedVtr.push_back(vector.at(i));
                                break;
                        }
                        else continue;
                }
                else{
                        sortedVtr.insert(sortedVtr.begin() +j,vector.at(i));
                        break;
                }
        }
  }

  sortedVtr.swap(vector);
  sortedVtr.resize(0);
}

void Tracker::SortSpheresByCount(std::vector<Sphere *> &sphereVtr){

  std::vector<Sphere *> sortedVtr;
  sortedVtr.resize(0);

  for(int i=0;i<sphereVtr.size();i++){
	Sphere * tmpSphere = sphereVtr.at(i);
	int countToCompare = tmpSphere->getHitCount();
	//add the first as its an empty vector to begin with
        if(i==0){
                sortedVtr.push_back(tmpSphere);
                continue;
        }
	for(int j=0;j<sortedVtr.size();j++){
                if(countToCompare > sortedVtr.at(j)->getHitCount()){
                        if(j==sortedVtr.size()-1){
                                sortedVtr.push_back(tmpSphere);
                                break;
                        }
                        else continue;
                }
                else{
                        sortedVtr.insert(sortedVtr.begin() +j,tmpSphere);
                        break;
                }
        }
  }

  sortedVtr.swap(sphereVtr);
  sortedVtr.resize(0);
}

void Tracker::PrintOutHits(HitCollection &hitsVector){

  for(int i=0;i<hitsVector.size();i++){
	int trackID = hitsVector.at(i).getTrackID();
	int parentID = hitsVector.at(i).getParentID();
  
	std::cout << "\n------ TPCHits ------" 
		  << "\nEntry: " << i << ", trackID: " << trackID << ", parentID: " << parentID;
  }
}

void Tracker::PrintOutHits(ScintHitCollection &hitsVector){

  for(int i=0;i<hitsVector.size();i++){
	int trackID = hitsVector.at(i).getTrackID();
	int parentID = hitsVector.at(i).getParentID();
  
	std::cout << "\n------ ScintHits ------" 
		  << "\nEntry: " << i << ", trackID: " << trackID << ", parentID: " << parentID;
  }
}


