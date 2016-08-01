### CMakeLists.txt -- gastpc/Apps

##############################

add_executable(VertexStats VertexStats.cxx)

target_link_libraries(VertexStats ${GENIE_LIBRARIES}
				                          ${Log4cpp_LIBRARIES}
				                          ${LIBXML2_LIBRARIES}
                                  ${ROOT_LIBRARIES})

##############################

add_executable(GasTPCG4Sim GasTPCG4Sim.cxx)

target_link_libraries(GasTPCG4Sim G4Sim
                                  ${Geant4_LIBRARIES}
                                  ${ROOT_LIBRARIES}
                                  ${GENIE_LIBRARIES}
                                  ${Log4cpp_LIBRARIES}
                                  ${LIBXML2_LIBRARIES})

##############################

add_executable(TestIOEvtRec TestIOEvtRec.cxx)

target_link_libraries(TestIOEvtRec GasTPCEvtRec
                                   ${ROOT_LIBRARIES})

##############################

install(TARGETS VertexStats GasTPCG4Sim TestIOEvtRec
        RUNTIME DESTINATION bin)