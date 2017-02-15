
### CMakeLists.txt -- gastpc/apps

##############################

# add_executable(VertexStats VertexStats.cxx)

# target_link_libraries(VertexStats ${GENIE_LIBRARIES}
# 				                          ${Log4cpp_LIBRARIES}
# 				                          ${LIBXML2_LIBRARIES}
#                                   ${ROOT_LIBRARIES})

##############################

add_executable(GasTPCG4Sim GasTPCG4Sim.cxx)

target_link_libraries(GasTPCG4Sim G4Sim
                                  ${Geant4_LIBRARIES}
                                  ${ROOT_LIBRARIES}
                                  ${GENIE_LIBRARIES}
                                  ${Log4cpp_LIBRARIES}
                                  ${LIBXML2_LIBRARIES})

##############################

# add_executable(TestDstWriter TestDstWriter.cxx)

# target_link_libraries(TestDstWriter GasTPCEvtRec NdtfDst
#                                     ${ROOT_LIBRARIES}
#                                     ${GENIE_LIBRARIES}
#                                     ${Log4cpp_LIBRARIES}
#                                     ${LIBXML2_LIBRARIES})

##############################

# add_executable(EvtRecDump EvtRecDump.cxx)
#
# target_link_libraries(EvtRecDump  GasTPCEvtRec
#                                   ${ROOT_LIBRARIES}
#                                   ${GENIE_LIBRARIES}
#                                   ${Log4cpp_LIBRARIES}
#                                   ${LIBXML2_LIBRARIES})

##############################

# add_executable(DstDump DstDump.cxx)
#
# target_link_libraries(DstDump ${ROOT_LIBRARIES}
#                               ${GENIE_LIBRARIES}
#                               ${Log4cpp_LIBRARIES}
#                               ${LIBXML2_LIBRARIES})

##############################

# add_executable(GasTPCReco GasTPCReco.cxx)
#
# target_link_libraries(GasTPCReco  GasTPCEvtRec NdtfDst
#                                   ${ROOT_LIBRARIES}
#                                   ${GENIE_LIBRARIES}
#                                   ${Log4cpp_LIBRARIES}
#                                   ${LIBXML2_LIBRARIES})

##############################

# add_executable(TestGasTPCG4Sim TestGasTPCG4Sim.cxx)

# target_link_libraries(TestGasTPCG4Sim  GasTPCEvtRec NdtfDst
#                                   ${ROOT_LIBRARIES}
#                                   ${GENIE_LIBRARIES}
#                                   ${Log4cpp_LIBRARIES}
#                                   ${LIBXML2_LIBRARIES})

##############################

install(TARGETS GasTPCG4Sim #EvtRecDump DstDump GasTPCReco
        RUNTIME DESTINATION bin)
