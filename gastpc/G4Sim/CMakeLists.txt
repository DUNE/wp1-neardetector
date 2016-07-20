### CMakeLists.cxx -- gastpc/Simulation

add_executable(GasTPCSimulation GasTPCSimulation.cxx
                                RunManager.cxx
                                MaterialsList.cxx
                                DuneGArNDDetConstr.cxx
                                DuneGArNDMagnetYokeGeometry.cxx
                                DuneGArNDMagnetCoilGeometry.cxx
                                DuneGArNDVesselGeometry.cxx
                                DuneGArNDEcalModuleGeometry.cxx
                                BeamSpillGenerator.cxx
                                GHepReader.cxx)

target_link_libraries(GasTPCSimulation ${ROOT_LIBRARIES}
                                       ${Geant4_LIBRARIES})

