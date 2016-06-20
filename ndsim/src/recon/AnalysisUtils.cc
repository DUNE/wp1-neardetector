//____________________________________________________________________________
/*!

author   Georgios Christodoulou <georgios at hep.ph.liv.ac.uk>
         University of Liverpool

\created June 2016
*/
//____________________________________________________________________________
#include "AnalysisUtils.hh"

std::vector<TVector3> AnalysisUtils::FindTPCVertices(std::vector<TrackParticle> Event){

  // Vector to store vertex position
  std::vector<TVector3> Vertices;

  // Event loop
  for(int k=0; k<Event.size(); k++){
    // Get TPC track
    TrackParticle track = Event.at(k);
    // Has to be in FV
    if( !track.GetInFV() ) continue;
    // Do not take tracks crossing the detector
    if( track.GetNEcals() > 1 ) continue;
    
    double track_time = track.GetReconPosition().T();
    TVector3 track_pos(track.GetReconPosition().X(),
		       track.GetReconPosition().Y(),
		       track.GetReconPosition().Z() );

    // If this vertex has already been found then skip
    bool vertexfound = false;
    for(int kk=0; kk<Vertices.size(); kk++){
      TVector3 temp = Vertices.at(kk);
      if( (track_pos - temp).Mag() < 50.0)
	vertexfound = true;
    }

    if( vertexfound ) continue;

    bool time_stamp = false;
    if( track_time > 0 && track_time < 960.0 )
      time_stamp = true;

    int ntracks = 0;
    // Loop again to find nearby tracks
    for(int kk=0; kk<Event.size(); kk++){
      // Not the same track
      if( kk == k ) continue;
      
      TrackParticle temp_track = Event.at(kk);
	
      TVector3 temp_pos(temp_track.GetReconPosition().X(),
			temp_track.GetReconPosition().Y(),
			temp_track.GetReconPosition().Z() );
      
      double dist  = (temp_pos-track_pos).Mag();
      
      if( dist >= 50.0 ) continue;
      ntracks++;

      if( temp_track.GetReconPosition().T() > 0 &&  temp_track.GetReconPosition().T() < 960.0 )
	time_stamp = true;
    }

    // No nearby tracks - skip
    if( ntracks == 0 )  continue;

    if( time_stamp )
      Vertices.push_back(track_pos);

  }

  return Vertices;

}
//____________________________________________________________________________
std::vector<TVector3> AnalysisUtils::FindIsoTPCTracks(std::vector<TrackParticle> Event){
  
  // Vector to store vertex position
  std::vector<TVector3> Vertices;

  // Event loop
  for(int k=0; k<Event.size(); k++){
    // Get TPC track
    TrackParticle track = Event.at(k);
    // Has to be in FV
    if( !track.GetInFV() ) continue;
    // Do not take tracks crossing the detector
    if( track.GetNEcals() > 1 ) continue;
    
    double track_time = track.GetReconPosition().T();
    TVector3 track_pos(track.GetReconPosition().X(),
		       track.GetReconPosition().Y(),
		       track.GetReconPosition().Z() );

    // If this vertex has already been found then skip
    bool vertexfound = false;
    for(int kk=0; kk<Vertices.size(); kk++){
      TVector3 temp = Vertices.at(kk);
      if( (track_pos - temp).Mag() < 50.0)
	vertexfound = true;
    }

    if( vertexfound ) continue;

    bool time_stamp = false;
    if( track_time > 0 && track_time < 960.0 )
      time_stamp = true;

    int ntracks = 0;
    // Loop again to find nearby tracks
    for(int kk=0; kk<Event.size(); kk++){
      // Not the same track
      if( kk == k ) continue;
      
      TrackParticle temp_track = Event.at(kk);
	
      TVector3 temp_pos(temp_track.GetReconPosition().X(),
			temp_track.GetReconPosition().Y(),
			temp_track.GetReconPosition().Z() );
      
      double dist = (temp_pos-track_pos).Mag();
      
      if( dist >= 50.0 ) continue;
      ntracks++;

      if( temp_track.GetReconPosition().T() > 0 &&  temp_track.GetReconPosition().T() < 960.0 )
	time_stamp = true;
    }

    // No nearby tracks - skip
    if( ntracks > 0 )  continue;

    if( time_stamp )
      Vertices.push_back(track_pos);

  }

  return Vertices;

}
//____________________________________________________________________________
int AnalysisUtils::OtherTracksFromEcal(std::vector<EcalTrackParticle> Event, TVector3 track_pos, double track_time){

  int found = 0;
  if( track_time < 0 ) return found;
  
  // Event loop
  for(int k=0; k<Event.size(); k++){
    // Get TPC track
    EcalTrackParticle temp_track = Event.at(k);
    if( temp_track.GetReconPosition().T() < 0 ) continue;

    double time = temp_track.GetReconPosition().T();
    TVector3 temptrack_pos(temp_track.GetReconPosition().X(),
			   temp_track.GetReconPosition().Y(),
			   temp_track.GetReconPosition().Z() );

    // Also check back position
    double btime = temp_track.GetBackPosition().T();
    TVector3 btemptrack_pos(temp_track.GetBackPosition().X(),
			    temp_track.GetBackPosition().Y(),
			    temp_track.GetBackPosition().Z() );
    
    if( (track_pos-temptrack_pos).Mag() < 50 && fabs(time-track_time) < 50 )
      found++;
    else if( (track_pos-btemptrack_pos).Mag() < 50 && fabs(btime-track_time) < 50 )
      found++;
  }

  return found;
}
