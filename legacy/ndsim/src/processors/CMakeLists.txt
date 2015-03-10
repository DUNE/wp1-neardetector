message(STATUS "processors ... ")

# get list of all source files in this directory.
AUX_SOURCE_DIRECTORY(.              PROCESSORS_SOURCE_FILES)

############### build the library #####################
add_library (processors             ${PROCESSORS_SOURCE_FILES})

target_link_libraries(processors    utils
                                    datastruct
                                    fluxgen
                                    evgen
                                    simul
                                    emcalc
                                    recon
                                    ${ROOT_LIBRARIES}
                                    ${CLHEP_LIBRARIES})

