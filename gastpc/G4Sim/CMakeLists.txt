### CMakeLists.cxx -- gastpc/G4Sim

file(GLOB G4Sim_SRCS "*.cxx")

add_executable(GasTPCSimulation ${G4Sim_SRCS})

target_link_libraries(GasTPCSimulation ${ROOT_LIBRARIES}
                                       ${Geant4_LIBRARIES}
                                       ${GENIE_LIBRARIES}
				       ${LIBXML2_LIBRARIES}
				       ${Log4cpp_LIBRARIES})

