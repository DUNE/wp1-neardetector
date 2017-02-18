#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ namespace gastpc;

#pragma link C++ class gastpc::EventRecord+;
#pragma link C++ class gastpc::MCGenInfo+;
#pragma link C++ class gastpc::MCParticle+;
#pragma link C++ class gastpc::MCTrack+;
#pragma link C++ class gastpc::MCHit+;
#pragma link C++ class gastpc::RecoParticle+;
#pragma link C++ class gastpc::Vector3D+;
#pragma link C++ class gastpc::Vector4D+;

#pragma link C++ class std::vector<gastpc::MCGenInfo*>;
#pragma link C++ class std::vector<gastpc::MCParticle*>;
#pragma link C++ class std::vector<gastpc::MCTrack*>;
#pragma link C++ class std::vector<gastpc::MCHit*>;
#pragma link C++ class std::vector<gastpc::RecoParticle*>;

#pragma link C++ function operator<<(ostream&, const gastpc::EventRecord&);
#pragma link C++ function operator<<(ostream&, const gastpc::MCParticle&);
#pragma link C++ function operator<<(ostream&, const gastpc::Vector3D&);
#pragma link C++ function operator<<(ostream&, const gastpc::Vector4D&);

#endif
