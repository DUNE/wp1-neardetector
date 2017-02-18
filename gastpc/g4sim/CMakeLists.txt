### CMakeLists.cxx -- gastpc/g4sim

file(GLOB G4Sim_SRCS "*.cxx")

add_library(G4Sim ${G4Sim_SRCS})

target_link_libraries(G4Sim   GasTPCEvtRec
                              ${ROOT_LIBRARIES}
                              ${Geant4_LIBRARIES}
                              ${GENIE_LIBRARIES}
				                      ${LIBXML2_LIBRARIES}
				                      ${Log4cpp_LIBRARIES})

install(TARGETS G4Sim
        ARCHIVE DESTINATION lib)
