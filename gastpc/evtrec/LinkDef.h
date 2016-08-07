#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ namespace gastpc;

#pragma link C++ class gastpc::EventRecord+;
#pragma link C++ class gastpc::NuInteraction+;
#pragma link C++ class gastpc::MCParticle+;
#pragma link C++ class gastpc::MCHit+;
#pragma link C++ class gastpc::MCTrack+;

#pragma link C++ class std::vector<gastpc::MCParticle*>;
#pragma link C++ class std::vector<gastpc::NuInteraction*>;
#pragma link C++ class std::vector<gastpc::MCTrack*>;
#pragma link C++ class std::vector<gastpc::MCHit*>;

#pragma link C++ function operator<<(ostream&, const gastpc::MCParticle&);

#endif