### CMakeLists.cxx -- gastpc/psreco

add_library(GasTPCPsReco SHARED ELossMeasurement.cxx
                                InteractionFinder.cxx
                                MomentumSmearer.cxx
                                ParticleIdentification.cxx
                                EcalPid.cxx
                                Utils.cxx)

target_link_libraries(GasTPCPsReco  GasTPCEvtRec
                                    ${ROOT_LIBRARIES}
                                    ${GENIE_LIBRARIES}
                                    ${LIBXML2_LIBRARIES}
                                    ${Log4cpp_LIBRARIES})

install(TARGETS GasTPCPsReco
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
