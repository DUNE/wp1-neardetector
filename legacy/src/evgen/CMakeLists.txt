message(STATUS "evgen ... ")

############### build the library #####################
add_library (evgen                   GenieNuEventAlgorithm.cc
				     NuEventFileLoadAlgorithm.cc
                                     LbnoGenieFlux.cc
				     LbnoGenieGeom.cc)

target_link_libraries(evgen          utils
				     geom
                                     datastruct
                                     ${ROOT_LIBRARIES}
                                     ${GENIE_LIBRARIES}
                                     ${PYTHIA6_LIBRARIES}
                                     ${CLHEP_LIBRARIES}
				     ${LOG4CPP_LIBRARIES})


