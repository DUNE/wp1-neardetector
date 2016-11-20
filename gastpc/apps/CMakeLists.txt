### CMakeLists.txt -- gastpc/apps

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

add_executable(TestIOEvtRecWithGenie TestIOEvtRecWithGenie.cxx)

target_link_libraries(TestIOEvtRecWithGenie GasTPCEvtRec
                                     	    ${ROOT_LIBRARIES}
                                     	    ${GENIE_LIBRARIES}
                                     	    ${Log4cpp_LIBRARIES}
                                     	    ${LIBXML2_LIBRARIES})

##############################

add_executable(EvtRecDump EvtRecDump.cxx)

target_link_libraries(EvtRecDump  GasTPCEvtRec
                                  ${ROOT_LIBRARIES}
                                  ${GENIE_LIBRARIES}
                                  ${Log4cpp_LIBRARIES}
                                  ${LIBXML2_LIBRARIES})

##############################

#add_executable(GasTPCReco GasTPCReco.cxx)

#target_link_libraries(GasTPCReco  GasTPCEvtRec NdtfDst
#                                  ${ROOT_LIBRARIES}
#                                  ${GENIE_LIBRARIES}
#                                  ${Log4cpp_LIBRARIES}
#                                  ${LIBXML2_LIBRARIES})

##############################

install(TARGETS VertexStats GasTPCG4Sim TestIOEvtRec TestIOEvtRecWithGenie
                EvtRecDump
        RUNTIME DESTINATION bin)
